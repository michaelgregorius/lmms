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

#include <QFormLayout>
#include <QLabel>

namespace lmms
{

AudioPipeWire::AudioPipeWire(bool& successful, AudioEngine* audioEngine) :
	AudioDevice(DEFAULT_CHANNELS, audioEngine)
{
	successful = true;
}

AudioPipeWire::~AudioPipeWire()
{
	stopProcessing();
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
	stopProcessingThread(this);
}

void AudioPipeWire::run()
{
	// TODO
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
