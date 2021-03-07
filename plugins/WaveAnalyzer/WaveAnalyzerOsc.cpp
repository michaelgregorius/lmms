/*
 * WaveAnalyzerOsc.cpp - Definition of WaveAnalyzerOsc class
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

#include "WaveAnalyzerOsc.h"

#include <QColor>
#include <QPaintEvent>
#include <QPainter>

WaveAnalyzerOsc::WaveAnalyzerOsc(WaveAnalyzerControls* controls, QWidget* parent) :
	QWidget(parent),
	m_controls(controls)
{
	// Initialize widget
	// Set minimum and maximum size
	QSize s(400, 200);
	setMinimumSize(s);
	setMaximumSize(s);
	resize(s);
}

WaveAnalyzerOsc::~WaveAnalyzerOsc()
{
}

void WaveAnalyzerOsc::paintEvent(QPaintEvent* pe)
{
	QPainter painter(this);

	// Draw background
	painter.fillRect(10, 10, width() - 10, height() - 10, QColor::fromRgb(0, 0, 0, 255));
}
