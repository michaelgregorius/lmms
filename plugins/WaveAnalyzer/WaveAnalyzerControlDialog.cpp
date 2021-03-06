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
#include <QWidget>

#include "WaveAnalyzerControlDialog.h"

#include "embed.h"
#include "PixmapButton.h"
#include "WaveAnalyzerControls.h"

WaveAnalyzerControlDialog::WaveAnalyzerControlDialog(WaveAnalyzerControls* controls) :
	EffectControlDialog(controls)
{
	// Layout to separate controls from the Wave Analyzer
	QHBoxLayout* mainLayout = new QHBoxLayout;
	setLayout(mainLayout);

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

	// Freeze button
	PixmapButton* freezeButton = new PixmapButton(this, tr("Freeze current wave"));
	QPixmap* freezeOnPixMap = new QPixmap(PLUGIN_NAME::getIconPixmap("play"));
	QPixmap* freezeOffPixMap = new QPixmap(PLUGIN_NAME::getIconPixmap("pause"));
	freezeButton->setActiveGraphic(*freezeOnPixMap);
	freezeButton->setInactiveGraphic(*freezeOffPixMap);
	freezeButton->setCheckable(true);
	freezeButton->setModel(&controls->m_freezeModel);

	controlLayout->addWidget(freezeButton);

	mainLayout->addLayout(controlLayout);
}

WaveAnalyzerControlDialog::~WaveAnalyzerControlDialog()
{
}
