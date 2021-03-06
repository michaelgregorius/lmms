/*
 * WaveAnalyzerControls.h - Declaration of WaveAnalyzerControls class
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

#ifndef WAVEANALYZER_CONTROLS_H
#define WAVEANALYZER_CONTROLS_H

#include "EffectControls.h"
#include "WaveAnalyzerControlDialog.h"

class WaveAnalyzerEffect;

class WaveAnalyzerControls : public EffectControls
{
	Q_OBJECT
public:
	WaveAnalyzerControls(WaveAnalyzerEffect* effect);
	virtual ~WaveAnalyzerControls();

	virtual void saveSettings(QDomDocument & doc, QDomElement & parent);
	virtual void loadSettings(const QDomElement & _this);

	inline virtual QString nodeName() const
	{
		return "WaveAnalyzerControls";
	}

	virtual int controlCount()
	{
		return 2;
	}

	virtual EffectControlDialog* createView()
	{
		return new WaveAnalyzerControlDialog(this);
	}

	inline void setLeftLevel(float l)
	{
		m_leftLevel.setValue(l);
	}
	inline void setRightLevel(float r)
	{
		m_rightLevel.setValue(r);
	}

private:
	WaveAnalyzerEffect* m_effect;

	// Model to start acquiring data for the WaveAnalyzer
	BoolModel m_startModel;
	// Model to freeze the current wave in the background
	BoolModel m_freezeModel;

	// Current input levels
	FloatModel m_leftLevel;
	FloatModel m_rightLevel;

	friend class WaveAnalyzerEffect;
	friend class WaveAnalyzerControlDialog;
	friend class WaveAnalyzerLevelIndicator;
};

#endif
