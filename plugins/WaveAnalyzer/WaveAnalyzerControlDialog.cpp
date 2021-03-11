/*
 * WaveAnalyzerControlDialog.cpp - Definition of WaveAnalyzerControlDialog class
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

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

#include "WaveAnalyzerControlDialog.h"
#include "WaveAnalyzerLevelIndicator.h"
#include "WaveAnalyzerClipIndicator.h"
#include "WaveAnalyzerOsc.h"

#include "ComboBox.h"
#include "embed.h"
#include "Knob.h"
#include "PixmapButton.h"
#include "WaveAnalyzerControls.h"

WaveAnalyzerControlDialog::WaveAnalyzerControlDialog(WaveAnalyzerControls* controls) :
	EffectControlDialog(controls)
{
	/*  ____________________________________
	 *  | Level indicator                   |
	 *  |___________________________________|
	 *  | C  |                              |
	 *  | o  |     Oscilloscope             |
	 *  | n  |                              |
	 *  | t  |                              |
	 *  | r  |                              |
	 *  | o  |                              |
	 *  | l  |                              |
	 *  |_s__|______________________________|
	 */

	// Layout to separate controls and oscilloscope from the top bar
	QVBoxLayout* mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	// Layout for the top bar (level indicator)
	QHBoxLayout* topBar = new QHBoxLayout;

	// Add level indicator
	WaveAnalyzerLevelIndicator* levelIndicator = new WaveAnalyzerLevelIndicator(controls, this);
	// Level changes on the controls will trigger updates on the level indicator
	connect(&controls->m_leftLevel, SIGNAL(dataChanged()), levelIndicator, SLOT(update()));
	connect(&controls->m_rightLevel, SIGNAL(dataChanged()), levelIndicator, SLOT(update()));

	// Add clipping indicator
	WaveAnalyzerClipIndicator* clipIndicator = new WaveAnalyzerClipIndicator(controls, this);
	// Clipping models will trigger the leds
	connect(&controls->m_clippedLeft, SIGNAL(dataChanged()), clipIndicator, SLOT(update()));
	connect(&controls->m_clippedRight, SIGNAL(dataChanged()), clipIndicator, SLOT(update()));

	// Adds title
	QLabel* title = new QLabel();
	title->setPixmap(QPixmap(PLUGIN_NAME::getIconPixmap("title")));

	// Adds numberOfFrames Knob
	Knob* framesKnob = new Knob(knobBright_26);
	framesKnob->setLabel("Frames");
	framesKnob->setModel(&controls->m_numberOfFrames);

	// Adds drawing mode Combobox + Label
	QVBoxLayout* comboBoxLayout = new QVBoxLayout;

	QLabel* drawingModeTitle = new QLabel("Drawing mode:");

	ComboBox* drawingMode = new ComboBox(this);
	drawingMode->setModel(&controls->m_drawingMode);
	drawingMode->setFixedSize(130, ComboBox::DEFAULT_HEIGHT);
	drawingMode->setToolTip(tr("Drawing mode"));

	comboBoxLayout->addWidget(drawingModeTitle);
	comboBoxLayout->addWidget(drawingMode);

	topBar->addWidget(levelIndicator);
	topBar->addWidget(clipIndicator);
	topBar->addStretch();
	topBar->addWidget(title);
	topBar->addStretch();
	topBar->addLayout(comboBoxLayout);
	topBar->addWidget(framesKnob);
	mainLayout->addLayout(topBar);

	// Horizontal layout for the controls and oscilloscope
	QHBoxLayout* bottomLayout = new QHBoxLayout;

	// Layout for the controls themselves
	QVBoxLayout* controlLayout = new QVBoxLayout;

	// Start button
	PixmapButton* startButton = new PixmapButton(this, tr("Start acquiring data"));
	QPixmap* startOnPixMap = new QPixmap(PLUGIN_NAME::getIconPixmap("play"));
	QPixmap* startOffPixMap = new QPixmap(PLUGIN_NAME::getIconPixmap("pause"));
	startButton->setActiveGraphic(*startOnPixMap);
	startButton->setInactiveGraphic(*startOffPixMap);
	startButton->setCheckable(true);
	startButton->setModel(&controls->m_startModel);

	controlLayout->addWidget(startButton);

	// Snapshot button
	PixmapButton* snapshotButton = new PixmapButton(this, tr("Snapshot current wave"));
	QPixmap* snapshotOnPixMap = new QPixmap(PLUGIN_NAME::getIconPixmap("play"));
	QPixmap* snapshotOffPixMap = new QPixmap(PLUGIN_NAME::getIconPixmap("pause"));
	snapshotButton->setActiveGraphic(*snapshotOnPixMap);
	snapshotButton->setInactiveGraphic(*snapshotOffPixMap);
	snapshotButton->setCheckable(true);
	snapshotButton->setModel(&controls->m_snapshotModel);

	controlLayout->addWidget(snapshotButton);

	// Layout for the oscilloscope
	QVBoxLayout* oscilloscopeLayout = new QVBoxLayout;

	// Oscilloscope
	WaveAnalyzerOsc* osc = new WaveAnalyzerOsc(controls, this);
	oscilloscopeLayout->addWidget(osc);

	bottomLayout->addLayout(controlLayout);
	bottomLayout->addLayout(oscilloscopeLayout);

	mainLayout->addLayout(bottomLayout);
}

WaveAnalyzerControlDialog::~WaveAnalyzerControlDialog()
{
}
