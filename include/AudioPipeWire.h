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

#ifndef LMMS_AUDIO_PIPEWIRE_H
#define LMMS_AUDIO_PIPEWIRE_H

#include "lmmsconfig.h"

#ifdef LMMS_HAVE_PIPEWIRE

#include "AudioDevice.h"
#include "AudioDeviceSetupWidget.h"

#include <pipewire/pipewire.h>

#include <QThread>

class QLabel;

namespace lmms
{

class AudioPipeWire : public QThread, public AudioDevice
{
	Q_OBJECT
public:
	AudioPipeWire(bool& successful, AudioEngine* audioEngine);
	~AudioPipeWire() override;

	inline static QString name()
	{
		return QT_TRANSLATE_NOOP( "AudioDeviceSetupWidget", "PipeWire" );
	}

	class setupWidget : public gui::AudioDeviceSetupWidget
	{
	public:
		setupWidget(QWidget* parent);
		~setupWidget() override;

		void saveSettings() override;

	private:
		QLabel* m_toBeImplementedLabel;
	};

private:
	void startProcessing() override;
	void stopProcessing() override;
	void run() override;

	// TODO Make private or encapsulate better!
public:
	// TODO Rename after code works!
	pw_main_loop* loop = nullptr;
    pw_stream* stream = nullptr;

	// TODO Remove once we are done with the sine test implementation
	double accumulator = 0;

	// TODO Rename to something like m_podBuilderBuffer
	// This is a scratch space like buffer that is used by PipeWire to create POD (plain old data) objects on the stack.
	// TODO We might not really need this as a member in case we do not create PODs in realtime critical code.
	uint8_t buffer[1024];
};

} // namespace lmms

#endif // LMMS_HAVE_PIPEWIRE

#endif // LMMS_AUDIO_PIPEWIRE_H
