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

// TODO Remove once the sine example code is removed!
#include <math.h>

#include <spa/param/audio/format-utils.h>

#include <QFormLayout>
#include <QLabel>

// TODO Remove most of this except channels!
#define M_PI_M2 ( M_PI + M_PI )
#define DEFAULT_RATE            48000
#define DEFAULT_CHANNELS        2
#define DEFAULT_VOLUME          0.7

namespace lmms
{

static void on_process(void *userdata)
{
	AudioPipeWire* data = static_cast<AudioPipeWire*>(userdata);
	struct pw_buffer *b;
	struct spa_buffer *buf;
	int i, c, n_frames, stride;
	float *dst, val;

	if ((b = pw_stream_dequeue_buffer(data->stream)) == NULL) {
			pw_log_warn("out of buffers: %m");
			return;
	}

	buf = b->buffer;

	dst = static_cast<float*>(buf->datas[0].data);

	if (dst == nullptr) return;

	stride = sizeof(float) * DEFAULT_CHANNELS;
	n_frames = buf->datas[0].maxsize / stride;
	if (b->requested)
			n_frames = SPA_MIN(b->requested, n_frames);

	for (i = 0; i < n_frames; i++) {
			data->accumulator += M_PI_M2 * 440 / DEFAULT_RATE;
			if (data->accumulator >= M_PI_M2)
					data->accumulator -= M_PI_M2;

			val = sin(data->accumulator) * DEFAULT_VOLUME;
			for (c = 0; c < DEFAULT_CHANNELS; c++)
					*dst++ = val;
	}

	buf->datas[0].chunk->offset = 0;
	buf->datas[0].chunk->stride = stride;
	buf->datas[0].chunk->size = n_frames * stride;

	pw_stream_queue_buffer(data->stream, b);
}

static const struct pw_stream_events stream_events = {
        PW_VERSION_STREAM_EVENTS,
        .process = on_process,
};

AudioPipeWire::AudioPipeWire(bool& successful, AudioEngine* audioEngine) :
	AudioDevice(DEFAULT_CHANNELS, audioEngine)
{
	pw_init(nullptr, nullptr);
 
	loop = pw_main_loop_new(nullptr);

	stream = pw_stream_new_simple(
				pw_main_loop_get_loop(loop),
				"LMMS",
				pw_properties_new(
					PW_KEY_MEDIA_TYPE, "Audio",
					PW_KEY_MEDIA_CATEGORY, "Playback",
					PW_KEY_MEDIA_ROLE, "Music",
					NULL),
				&stream_events,
				this);
	
	const struct spa_pod *params[1];
    struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

	// TODO Find out sample rate from PipeWire and set it for LMMS (see AudioJack for an example)

	auto audioInfoRaw = SPA_AUDIO_INFO_RAW_INIT(
                                .format = SPA_AUDIO_FORMAT_F32,
								.rate = DEFAULT_RATE,
                                .channels = DEFAULT_CHANNELS);

	params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &audioInfoRaw);
 
	pw_stream_connect(stream,
						PW_DIRECTION_OUTPUT,
						PW_ID_ANY,
						static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS | PW_STREAM_FLAG_RT_PROCESS),
						params, 1);

	successful = true;
}

AudioPipeWire::~AudioPipeWire()
{
	stopProcessing();

	pw_stream_destroy(stream);
    pw_main_loop_destroy(loop);
}

void AudioPipeWire::startProcessing()
{
	if (!isRunning())
	{
		start(QThread::HighPriority);
	}
}

void AudioPipeWire::stopProcessing()
{
	// TODO Not tested yet!
	pw_main_loop_quit(loop);

	stopProcessingThread(this);
}

void AudioPipeWire::run()
{
	pw_main_loop_run(loop);
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
