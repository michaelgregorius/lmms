/*
 * AudioPipeWire.h - Audio driver for PipeWire
 *
 * Copyright (c) 2024- Michael Gregorius
 *
 * This file is part of LMMS - https://lmms.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */

#include "AudioPipeWire.h"

#ifdef LMMS_HAVE_PIPEWIRE

#include "AudioEngine.h"

#include <spa/param/audio/format-utils.h>

#include <QFormLayout>
#include <QLabel>

#define DEFAULT_CHANNELS 2


namespace lmms
{

static void on_process(void *userdata)
{
	AudioPipeWire* data = static_cast<AudioPipeWire*>(userdata);
	data->processStreams();
}

static const struct pw_stream_events stream_events = {
        .version = PW_VERSION_STREAM_EVENTS,
        .process = on_process,
};

static void fillWithSilence(float* buffer, unsigned long numberOfFrames)
{
	for (unsigned long i = 0; i < numberOfFrames * DEFAULT_CHANNELS; ++i)
	{
		*buffer++ = 0;
	}
}

AudioPipeWire::AudioPipeWire(bool& successful, AudioEngine* audioEngine) :
	AudioDevice(DEFAULT_CHANNELS, audioEngine)
{
	pw_init(nullptr, nullptr);
 
	m_loop = pw_thread_loop_new("LMMS", nullptr);

	m_context = pw_context_new(pw_thread_loop_get_loop(m_loop), NULL /* properties */, 0 /* user_data size */);
 
	m_core = pw_context_connect(m_context, NULL /* properties */, 0 /* user_data size */);

	// Update the sample rate used by LMMS to the one from PipeWire if possible
	if (m_core)
	{
		struct pw_properties const *props = pw_core_get_properties(m_core);

		// Note: there does not seem to be a define for "default.clock.rate"
		const char* sampleRateStr = pw_properties_get(props, "default.clock.rate");
		if (sampleRateStr)
		{
			const auto sampleRatePipeWire = static_cast<lmms::sample_rate_t>(pw_properties_parse_int(sampleRateStr));
		
			if (sampleRatePipeWire != sampleRate())
			{
				setSampleRate(sampleRatePipeWire);
			}
		}
	}

	m_stream = pw_stream_new_simple(
				pw_thread_loop_get_loop(m_loop),
				"LMMS",
				pw_properties_new(
					PW_KEY_MEDIA_TYPE, "Audio",
					PW_KEY_MEDIA_CATEGORY, "Playback",
					PW_KEY_MEDIA_ROLE, "Music",
					NULL),
				&stream_events,
				this);
	
	const struct spa_pod *params[1];
    struct spa_pod_builder b = SPA_POD_BUILDER_INIT(m_podBuilderBuffer, sizeof(m_podBuilderBuffer));

	auto audioInfoRaw = SPA_AUDIO_INFO_RAW_INIT(
							.format = SPA_AUDIO_FORMAT_F32,
							.rate = sampleRate(),
							.channels = DEFAULT_CHANNELS);

	params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &audioInfoRaw);
 
	pw_stream_connect(m_stream,
						PW_DIRECTION_OUTPUT,
						PW_ID_ANY,
						static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS | PW_STREAM_FLAG_RT_PROCESS),
						params, 1);

	// TODO Negotiate preferred buffer sizes
	// const auto framesPerPeriod = this->audioEngine()->framesPerPeriod();
	// const auto bufferSize = framesPerPeriod * DEFAULT_CHANNELS * sizeof(float);

	// // TODO Rather use a new buffer? It should be copied though I guess.
    // params[0] = static_cast<spa_pod*>(spa_pod_builder_add_object(&b,
    //     SPA_TYPE_OBJECT_ParamBuffers, SPA_PARAM_Buffers,
    //     SPA_PARAM_BUFFERS_size,    SPA_POD_Int(bufferSize)));
    
    // // Update stream params
    // pw_stream_update_params(m_stream, params, 1);

	successful = true;
}

AudioPipeWire::~AudioPipeWire()
{
	stopProcessing();

	pw_stream_destroy(m_stream);
	pw_core_disconnect(m_core);
	pw_context_destroy(m_context);
	pw_thread_loop_destroy(m_loop);
}

void AudioPipeWire::processStreams()
{
	// Fetch the buffer from the stream
	struct pw_buffer* b = pw_stream_dequeue_buffer(m_stream);
	if (b == nullptr)
	{
		pw_log_warn("out of buffers: %m");
		return;
	}

	const struct spa_buffer* buf = b->buffer;

	// Fetch the destination buffer where we will write the result
	float* dst = static_cast<float*>(buf->datas[0].data);
	if (dst == nullptr) return;

	const auto stride = sizeof(float) * DEFAULT_CHANNELS;
	auto n_frames = buf->datas[0].maxsize / stride;
	if (b->requested)
	{
		n_frames = SPA_MIN(b->requested, n_frames);
	}

	if (!AudioDevice::isRunning())
	{
		fillWithSilence(dst, n_frames);
	}
	else
	{
		audioEngine()->renderNextBuffer({dst, channels(), static_cast<f_cnt_t>(n_frames)});
	}

	buf->datas[0].chunk->offset = 0;
	buf->datas[0].chunk->stride = stride;
	buf->datas[0].chunk->size = n_frames * stride;

	pw_stream_queue_buffer(m_stream, b);
}

void AudioPipeWire::startProcessingImpl()
{
	pw_thread_loop_start(m_loop);
}

void AudioPipeWire::stopProcessingImpl()
{
	pw_thread_loop_stop(m_loop);
}

AudioPipeWire::setupWidget::setupWidget( QWidget * _parent ) :
	AudioDeviceSetupWidget( AudioPipeWire::name(), _parent )
{
	QFormLayout* form = new QFormLayout(this);

	m_toBeImplementedLabel = new QLabel("To be implemented", this);
	form->addRow(tr("Device"), m_toBeImplementedLabel);
}

AudioPipeWire::setupWidget::~setupWidget()
{
}

void AudioPipeWire::setupWidget::saveSettings()
{
	// TODO Save settings
}

} // namespace lmms

#endif // LMMS_HAVE_PIPEWIRE
