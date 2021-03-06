/*
 * WaveAnalyzerControls.cpp - Definition of WaveAnalyzerControls class
 *
 * Copyright (c) 2020 Ian Caio <iancaio_dev/at/hotmail.com>
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

#include <QDomElement>

#include "WaveAnalyzerControls.h"

#include "WaveAnalyzer.h"

WaveAnalyzerControls::WaveAnalyzerControls(WaveAnalyzerEffect* effect) :
	EffectControls(effect),
	m_effect(effect),
	m_startModel(false, this, tr("Start Acquiring Data")),
	m_freezeModel(false, this, tr("Freeze current wave")),
	m_leftLevel(0, 0.0f, 1.0f, 0.01f, this, tr("Left Level")),
	m_rightLevel(0, 0.0f, 1.0f, 0.01f, this, tr("Right Level"))
{
}

WaveAnalyzerControls::~WaveAnalyzerControls()
{
}

void WaveAnalyzerControls::loadSettings(const QDomElement & _this)
{
}

void WaveAnalyzerControls::saveSettings(QDomDocument & doc, QDomElement & parent)
{
}
