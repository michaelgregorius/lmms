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
#include "WaveAnalyzerControls.h"

#include <QColor>
#include <QPaintEvent>
#include <QPainter>

WaveAnalyzerOsc::WaveAnalyzerOsc(WaveAnalyzerControls* controls, QWidget* parent) :
	QWidget(parent),
	m_controls(controls)
{
	// Initialize widget
	// Set minimum and maximum size
	int totalWidth = viewportWidth + leftMargin + rightMargin;
	int totalHeight = viewportHeight + topMargin + bottomMargin;

	QSize s(totalWidth, totalHeight);
	setMinimumSize(s);
	setMaximumSize(s);
	resize(s);

	m_wave = new WaveAnalyzerWaveform(controls, this);

	// Connect signal to repaint when buffer changes
	connect(m_controls, SIGNAL(bufferChanged()), m_wave, SLOT(repaint()));
}

WaveAnalyzerOsc::~WaveAnalyzerOsc()
{
}

void WaveAnalyzerOsc::paintEvent(QPaintEvent* pe)
{
	QPainter painter(this);

	// Draw viewport
	paintViewport(painter);

	// Draw labels
	paintLabels(painter);

	// Waveform is drawn by WaveAnalyzerWaveform
}

void WaveAnalyzerOsc::paintViewport(QPainter & p)
{
	// Fill a rect behind the background slightly
	// bigger just to make a small edge
	p.fillRect(
		leftMargin - 2,
		topMargin - 2,
		viewportWidth + 4,
		viewportHeight + 4,
		borderColor
	);
	// Fill the background
	p.fillRect(
		leftMargin,
		topMargin,
		viewportWidth,
		viewportHeight,
		backgroundColor
	);

	// Draw clipping lines
	p.setPen(clippingLineColor);
	// Top clipping line
	p.drawLine(
		leftMargin,
		topMargin + clippingMargin,
		leftMargin + viewportWidth,
		topMargin + clippingMargin
	);
	// Bottom clipping line
	p.drawLine(
		leftMargin,
		topMargin + viewportHeight - clippingMargin,
		leftMargin + viewportWidth,
		topMargin + viewportHeight - clippingMargin
	);

	// Draw center line
	p.setPen(centerLineColor);
	p.drawLine(
		leftMargin,
		topMargin + viewportHeight / 2,
		leftMargin + viewportWidth,
		topMargin + viewportHeight / 2
	);
}

void WaveAnalyzerOsc::paintLabels(QPainter & p)
{
	// Paint the labels from the viewport
	p.setPen(labelColor);
	QFont font;
	font.setPixelSize(labelFontSize);
	p.setFont(font);

	p.drawText(
		0,
		topMargin + (labelFontSize / 2),
		"Amp"
	);
	p.drawText(
		leftMargin / 2,
		topMargin + clippingMargin + (labelFontSize / 2),
		"1"
	);
	p.drawText(
		leftMargin / 2,
		topMargin + viewportHeight / 2 + (labelFontSize / 2),
		"0"
	);
	p.drawText(
		leftMargin / 2,
		topMargin + viewportHeight - clippingMargin + (labelFontSize / 2),
		"-1"
	);
}

WaveAnalyzerWaveform::WaveAnalyzerWaveform(WaveAnalyzerControls* controls, QWidget* parent) :
	QWidget(parent),
	m_controls(controls)
{
	move(leftMargin, topMargin);
	QSize s(viewportWidth, viewportHeight);
	setMinimumSize(s);
	setMaximumSize(s);
	resize(s);
}

WaveAnalyzerWaveform::~WaveAnalyzerWaveform()
{
}

void WaveAnalyzerWaveform::paintEvent(QPaintEvent* pe)
{
	QPainter p(this);

	// Paint the wave shape in the buffer
	int totalFrames = static_cast<int>(m_controls->m_numberOfFrames.value());
	int lastFrame = totalFrames - 1;
	int totalPixels = viewportWidth;
	int framesPerPixel = totalFrames / totalPixels;

	// References for the drawing
	int baseY = viewportHeight / 2;
	int ySpace = (viewportHeight / 2) - clippingMargin;

	p.setPen(waveColor);

	int currentFrame = 0;
	for (int i = 0; i < totalPixels; ++i)
	{
		float valueL = 0;
		float valueR = 0;

		// For now draw the average of left and right channels
		if (currentFrame <= lastFrame)
		{
			valueL = m_controls->m_ampBufferL[currentFrame];
			valueR = m_controls->m_ampBufferR[currentFrame];
			currentFrame += framesPerPixel;
		}

		int leftY = baseY - (valueL * ySpace);
		int rightY = baseY - (valueR * ySpace);
		m_pointsL[i] = QPointF(i, leftY);
		m_pointsR[i] = QPointF(i, rightY);
	}

	p.setRenderHint(QPainter::Antialiasing);
	p.drawPolyline(m_pointsL, totalPixels);
	p.drawPolyline(m_pointsR, totalPixels);
}
