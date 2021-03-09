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
	connect(m_controls, SIGNAL(bufferChanged(int)), m_wave, SLOT(updatePoints(int)));
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

void WaveAnalyzerWaveform::updatePoints(int count)
{
	int totalPixels = viewportWidth;
	int totalFrames = static_cast<int>(m_controls->m_numberOfFrames.value());
	int lastFrame = totalFrames - 1;
	int framesPerPixel = totalFrames / totalPixels;

	// Shift existing points
	int pixelsShift = count / framesPerPixel;
	shiftPoints(pixelsShift);

	// Add new points
	// References for the points
	int baseY = viewportHeight / 2;
	int ySpace = (viewportHeight / 2) - clippingMargin;

	// Go to the first altered frame
	int currentFrame = totalFrames - count;
	int currentPixel = totalPixels - (count / framesPerPixel);

	for (; currentPixel < totalPixels; ++currentPixel)
	{
		float valueL = 0;
		float valueR = 0;

		if (currentFrame <= lastFrame)
		{
			valueL = m_controls->m_ampBufferL[currentFrame];
			valueR = m_controls->m_ampBufferR[currentFrame];
			currentFrame += framesPerPixel;
		}

		int leftY = baseY - (valueL * ySpace);
		int rightY = baseY - (valueR * ySpace);
		m_pointsL[currentPixel] = QPointF(currentPixel, leftY);
		m_pointsR[currentPixel] = QPointF(currentPixel, rightY);
	}

	repaint();
}

void WaveAnalyzerWaveform::shiftPoints(int count)
{
	int size = viewportWidth;
	int lastIndex = size - 1;

	for (int i = 0; i < size; ++i)
	{
		// If point being shifted is out of bounds return
		if (i + count > lastIndex) { return; }
		// Else we shift the points
		m_pointsL[i] = QPointF(i, m_pointsL[i + count].y());
		m_pointsR[i] = QPointF(i, m_pointsR[i + count].y());
	}
}

void WaveAnalyzerWaveform::paintEvent(QPaintEvent* pe)
{
	QPainter p(this);

	// Paint the wave shape in the buffer
	int totalPixels = viewportWidth;

	p.setRenderHint(QPainter::Antialiasing);
	p.setPen(waveColor);
	p.drawPolyline(m_pointsL, totalPixels);
	p.drawPolyline(m_pointsR, totalPixels);
}
