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
	m_spinBoxesLayout( this ),
	m_infoDisplay( this )
{
	m_spinBoxesLayout.setSpacing( 0 );
	m_spinBoxesLayout.setMargin( 0 );
	m_spinBoxesLayout.addWidget( &m_infoDisplay );

	ToolTip::add( this, tr( "Time units" ) );

	// update labels of LCD spinboxes
	setDisplayMode( m_displayMode );

	connect( gui->mainWindow(), SIGNAL( periodicUpdate() ),
					this, SLOT( updateTime() ) );

	QFont newFont = m_infoDisplay.font();
	//newFont.setPointSize( 20 );
	newFont.setPixelSize( 32 );
	m_infoDisplay.setFont( newFont );
	setStyleSheet( "QLabel { background-color: black; border-radius: 5px; padding: 0px 10px 0px 10px; }" );
}

void TimeDisplayWidget::setDisplayMode( DisplayMode displayMode )
{
	m_displayMode = displayMode;
}

void prependWithZerosUntilTargetLengthReached( QString & stringToFill, int targetLength )
{
	int length = qMin( stringToFill.length(), targetLength );
	QString fillZeros;
	fillZeros.fill( '0', targetLength - length );
	stringToFill.prepend( fillZeros );
}

QString TimeDisplayWidget::format( int value1, int value2, int value3 ) const
{
	if ( m_displayMode == MinutesSeconds )
	{
		QString minutes = QString::number( value1 );
		prependWithZerosUntilTargetLengthReached( minutes, 4 );

		QString seconds = QString::number( value2 );
		prependWithZerosUntilTargetLengthReached( seconds, 2 );

		QString milliseconds = QString::number( value3 );
		prependWithZerosUntilTargetLengthReached( milliseconds, 3 );

		return minutes + " : " + seconds + " . " + milliseconds;
	}

	if ( m_displayMode == BarsTicks )
	{
		QString bars = QString::number( value1 );
		prependWithZerosUntilTargetLengthReached( bars, 4 );

		QString beats = QString::number( value2 );
		prependWithZerosUntilTargetLengthReached( beats, 2 );

		QString ticks = QString::number( value3 );
		prependWithZerosUntilTargetLengthReached( ticks, 2 );

		return bars + " : " + beats + " . " + ticks;
	}

	// Fallback
	return QString::number( value1 ) + " : " + QString::number( value2 ) + " . " + QString::number( value3 );
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

		m_infoDisplay.setText( format( minutes, seconds, milliseconds ) );
	}
	else if ( m_displayMode == BarsTicks )
	{
		int const tick = s->getPlayPos().getTicks();

		int bars = (int)(tick / s->ticksPerTact() ) + 1;
		int beats = ( tick % s->ticksPerTact() ) / ( s->ticksPerTact() / s->getTimeSigModel().getNumerator() ) + 1;
		int ticks = ( tick % s->ticksPerTact() ) % ( s->ticksPerTact() / s->getTimeSigModel().getNumerator() );

		m_infoDisplay.setText( format( bars, beats, ticks ) );
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
