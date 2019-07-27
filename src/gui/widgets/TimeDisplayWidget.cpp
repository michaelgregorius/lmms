/*
 * TimeDisplayWidget.cpp - widget for displaying current playback time
 *
 * Copyright (c) 2014 Tobias Doerffel <tobydox/at/users.sourceforge.net>
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

#include <QMouseEvent>

#include "TimeDisplayWidget.h"
#include "GuiApplication.h"
#include "MainWindow.h"
#include "Engine.h"
#include "ToolTip.h"
#include "Song.h"



TimeDisplayWidget::TimeDisplayWidget() :
	QWidget(),
	m_displayMode( MinutesSeconds ),
	m_gridLayout( this ),
	m_firstValueDisplay(this),
	m_secondValueDisplay(this),
	m_thirdValueDisplay(this),
	m_firstLabelDisplay(this),
	m_secondLabelDisplay(this),
	m_thirdLabelDisplay(this),
	m_firstSeparator(":", this),
	m_secondSeparator(".", this)
{
	m_gridLayout.setSpacing( 0 );
	m_gridLayout.setMargin( 0 );
	m_gridLayout.setRowStretch(0, 1);
	m_gridLayout.addWidget( &m_firstValueDisplay, 0, 0 );
	m_gridLayout.addWidget( &m_firstSeparator, 0, 1 );
	m_gridLayout.addWidget( &m_secondValueDisplay, 0, 2 );
	m_gridLayout.addWidget( &m_secondSeparator, 0, 3 );
	m_gridLayout.addWidget( &m_thirdValueDisplay, 0, 4 );
	m_gridLayout.addWidget( &m_firstLabelDisplay, 1, 0, Qt::AlignRight );
	m_gridLayout.addWidget( &m_secondLabelDisplay, 1, 2, Qt::AlignRight );
	m_gridLayout.addWidget( &m_thirdLabelDisplay, 1, 4, Qt::AlignRight );

	ToolTip::add( this, tr( "Time units" ) );

	// update labels of LCD spinboxes
	setDisplayMode( m_displayMode );

	connect( gui->mainWindow(), SIGNAL( periodicUpdate() ),
					this, SLOT( updateTime() ) );

	QFont valueFont = m_firstValueDisplay.font();
	valueFont.setPointSize(18);
	m_firstValueDisplay.setFont(valueFont);
	m_firstSeparator.setFont(valueFont);
	m_secondValueDisplay.setFont(valueFont);
	m_secondSeparator.setFont(valueFont);
	m_thirdValueDisplay.setFont(valueFont);

	QFont labelFont = m_firstLabelDisplay.font();
	labelFont.setPointSize(6);
	m_firstLabelDisplay.setFont(labelFont);
	m_secondLabelDisplay.setFont(labelFont);
	m_thirdLabelDisplay.setFont(labelFont);
}

void TimeDisplayWidget::setDisplayMode( DisplayMode displayMode )
{
	m_displayMode = displayMode;
	updateLabelsByDisplayMode();
}

void prependWithZerosUntilTargetLengthReached( QString & stringToFill, int targetLength )
{
	int length = qMin( stringToFill.length(), targetLength );
	QString fillZeros;
	fillZeros.fill( '0', targetLength - length );
	stringToFill.prepend( fillZeros );
}

QString toStringWithPrependedZeroes(int value, int targetLength)
{
	QString valueAsString = QString::number(value);
	prependWithZerosUntilTargetLengthReached(valueAsString, targetLength);

	return valueAsString;
}

void TimeDisplayWidget::updateTime()
{
	Song* s = Engine::getSong();

	if ( m_displayMode == MinutesSeconds )
	{
		int const msec(s->getMilliseconds());
		int minutes = msec / 60000;
		int seconds = ( s->getMilliseconds() / 1000 ) % 60;
		int milliseconds = s->getMilliseconds() % 1000;

		m_firstValueDisplay.setText(toStringWithPrependedZeroes(minutes, 4));
		m_secondValueDisplay.setText(toStringWithPrependedZeroes(seconds, 2));
		m_thirdValueDisplay.setText(toStringWithPrependedZeroes(milliseconds, 3));
	}
	else if ( m_displayMode == BarsTicks )
	{
		int const tick = s->getPlayPos().getTicks();

		int bars = (int)(tick / s->ticksPerTact() ) + 1;
		int beats = ( tick % s->ticksPerTact() ) / ( s->ticksPerTact() / s->getTimeSigModel().getNumerator() ) + 1;
		int ticks = ( tick % s->ticksPerTact() ) % ( s->ticksPerTact() / s->getTimeSigModel().getNumerator() );

		m_firstValueDisplay.setText(toStringWithPrependedZeroes(bars, 4));
		m_secondValueDisplay.setText(toStringWithPrependedZeroes(beats, 2));
		m_thirdValueDisplay.setText(toStringWithPrependedZeroes(ticks, 3));
	}
}




void TimeDisplayWidget::mousePressEvent( QMouseEvent* mouseEvent )
{
	if( mouseEvent->button() == Qt::LeftButton )
	{
		if( m_displayMode == MinutesSeconds )
		{
			setDisplayMode( BarsTicks );
		}
		else
		{
			setDisplayMode( MinutesSeconds );
		}
	}
}

void TimeDisplayWidget::updateLabelsByDisplayMode()
{
	if ( m_displayMode == MinutesSeconds )
	{
		m_firstLabelDisplay.setText(tr("MIN"));
		m_secondLabelDisplay.setText(tr("SEC"));
		m_thirdLabelDisplay.setText(tr("MSEC"));
	}
	else if ( m_displayMode == BarsTicks )
	{
		m_firstLabelDisplay.setText(tr("BAR"));
		m_secondLabelDisplay.setText(tr("BEAT"));
		m_thirdLabelDisplay.setText(tr("TICK"));
	}
	else
	{
		m_firstLabelDisplay.setText("");
		m_secondLabelDisplay.setText("");
		m_thirdLabelDisplay.setText("");
	}
}
