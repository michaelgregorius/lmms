/*
 * FxLine.cpp - FX line widget
 *
 * Copyright (c) 2009 Andrew Kelley <superjoe30/at/gmail/dot/com>
 * Copyright (c) 2014 Tobias Doerffel <tobydox/at/users.sourceforge.net>
 *
 * This file is part of LMMS - http://lmms.io
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

#include "FxLine.h"

#include <QDebug>
#include <QInputDialog>
#include <QPainter>
#include <QLineEdit>
#include <QWhatsThis>

#include "FxMixer.h"
#include "FxMixerView.h"
#include "embed.h"
#include "Engine.h"
#include "GuiApplication.h"
#include "SendButtonIndicator.h"
#include "gui_templates.h"
#include "CaptionMenu.h"

const int FxLine::FxLineHeight = 287;
QPixmap * FxLine::s_sendBgArrow = NULL;
QPixmap * FxLine::s_receiveBgArrow = NULL;

class ElidedLabel : public QLabel
{
public:
	ElidedLabel(QWidget * parent = 0, Qt::WindowFlags f = 0) : QLabel (parent, f)
	{
		initLabel();
	}

	ElidedLabel(const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0) : QLabel (text, parent, f)
	{
		initLabel();
	}

	void initLabel()
	{
		//setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum);
	}

	void setText(const QString & text)
	{
		QFontMetrics metrics( this->font() );
		QString elidedText = metrics.elidedText( text, Qt::ElideMiddle, width() );

		QLabel::setText(elidedText);
	}
};

FxLine::FxLine( QWidget * _parent, FxMixerView * _mv, int _channelIndex) :
	QWidget( _parent ),
	m_mv( _mv ),
	m_channelIndex( _channelIndex ),
	m_backgroundActive( Qt::SolidPattern )
{
	/*if( ! s_sendBgArrow )
	{
		s_sendBgArrow = new QPixmap( embed::getIconPixmap( "send_bg_arrow", 29, 56 ) );
	}
	if( ! s_receiveBgArrow )
	{
		s_receiveBgArrow = new QPixmap( embed::getIconPixmap( "receive_bg_arrow", 29, 56 ) );
	}*/

	setFixedSize( 70, FxLineHeight );
	//setAttribute( Qt::WA_OpaquePaintEvent, true );
	setCursor( QCursor( embed::getIconPixmap( "hand" ), 3, 3 ) );

	// mixer sends knob
	/*m_sendKnob = new Knob( knobBright_26, this, tr("Channel send amount") );
	//m_sendKnob->move( 3, 22 );
	m_sendKnob->setVisible(false);*/

	// send button indicator
	/*m_sendBtn = new SendButtonIndicator( this, this, m_mv );
	//m_sendBtn->move( 2, 2 );*/

	// channel number
	m_lcd = new LcdWidget( 2, this );
	m_lcd->setValue( m_channelIndex );
	//m_lcd->move( 4, 58 );
	m_lcd->setMarginWidth( 1 );

	m_lcd->hide();

	m_elidedNameLabel = new ElidedLabel(this);
	m_elidedNameLabel->setAlignment(Qt::AlignVCenter);
	m_elidedNameLabel->setStyleSheet("font-size: 8pt; qproperty-alignment: AlignCenter;");

	//m_elidedNameLabel->setStyleSheet("background-color:red;");

	FxMixer * mix = Engine::fxMixer();
	QString channelName = mix->effectChannel( m_channelIndex )->m_name;

	updateChannelName(channelName);
	
	setWhatsThis( tr(
	"The FX channel receives input from one or more instrument tracks.\n "
	"It in turn can be routed to multiple other FX channels. LMMS automatically "
	"takes care of preventing infinite loops for you and doesn't allow making "
	"a connection that would result in an infinite loop.\n\n"
	
	"In order to route the channel to another channel, select the FX channel "
	"and click on the \"send\" button on the channel you want to send to. "
	"The knob under the send button controls the level of signal that is sent "
	"to the channel.\n\n"
	
	"You can remove and move FX channels in the context menu, which is accessed "
	"by right-clicking the FX channel.\n") );
}


FxLine::~FxLine()
{
	delete m_sendKnob;
	delete m_sendBtn;
	delete m_lcd;
}


void FxLine::setChannelIndex(int index) {
	m_channelIndex = index;

	m_lcd->setValue( m_channelIndex );
	m_lcd->update();
}

QWidget* FxLine::getNameWidget()
{
	return m_elidedNameLabel;
}


void FxLine::drawFxLine( QPainter* p, const FxLine *fxLine, const QString& name, bool isActive, bool sendToThis, bool receiveFromThis )
{
	int width = fxLine->rect().width();
	int height = fxLine->rect().height();

	QColor sh_color = QApplication::palette().color( QPalette::Active,
							QPalette::Shadow );
	QColor te_color = p->pen().brush().color();
	QColor bt_color = QApplication::palette().color( QPalette::Active,
							QPalette::BrightText );


	p->fillRect( fxLine->rect(), isActive ? fxLine->backgroundActive() : p->background() );

	p->setPen( QColor( 255, 255, 255, isActive ? 100 : 50 ) );
	p->drawRect( 1, 1, width-3, height-3 );

	p->setPen( isActive ? sh_color : QColor( 0, 0, 0, 50 ) );
	p->drawRect( 0, 0, width-1, height-1 );

	// draw the mixer send background
	/*if( sendToThis )
	{
		p->drawPixmap( 2, 0, 29, 56, *s_sendBgArrow );
	}
	else if( receiveFromThis )
	{
		p->drawPixmap( 2, 0, 29, 56, *s_receiveBgArrow );
	}*/

	if (fxLine->m_channelIndex == 0)
	{
		// Don't paint the index for the master channel?
		return;
	}

	QString indexLabel = QString::number(fxLine->m_channelIndex);

	QFont currentFont = fxLine->font();
	currentFont.setItalic(true);
	currentFont.setPointSizeF(16);
	p->setFont(currentFont);
	//p->setBrush(QColor(255, 255, 255, 255));
	p->setPen(QColor(255, 255, 255, 63));
	QFontMetrics metrics(currentFont);
	QRect textRect = metrics.boundingRect(indexLabel);
	//int pixelWidth = metrics.width(indexLabel);
	//int pixelHeight = metrics.height();
	int pixelWidth = textRect.width();
	int pixelHeight = textRect.height();

	p->drawText( fxLine->width() - pixelWidth - 10, pixelHeight, indexLabel );
	//p->drawText( fxLine->width() - pixelWidth - 10, fxLine->height() - pixelHeight + 10, indexLabel );
}

void FxLine::updateChannelName(const QString & channelName)
{
	m_elidedNameLabel->setText(channelName);
	m_elidedNameLabel->setToolTip(channelName);
}


void FxLine::paintEvent( QPaintEvent * )
{
	FxMixer * mix = Engine::fxMixer();
	bool sendToThis = mix->channelSendModel(
		m_mv->currentFxLine()->m_channelIndex, m_channelIndex ) != NULL;
	bool receiveFromThis = mix->channelSendModel(
		m_channelIndex, m_mv->currentFxLine()->m_channelIndex ) != NULL;
	QPainter painter;
	painter.begin( this );
	drawFxLine( &painter, this,
		mix->effectChannel( m_channelIndex )->m_name,
		m_mv->currentFxLine() == this, sendToThis, receiveFromThis );
	painter.end();
}


void FxLine::mousePressEvent( QMouseEvent * )
{
	m_mv->setCurrentFxLine( this );
}


void FxLine::mouseDoubleClickEvent( QMouseEvent * )
{
	renameChannel();
}


void FxLine::contextMenuEvent( QContextMenuEvent * )
{
	FxMixer * mix = Engine::fxMixer();
	QPointer<CaptionMenu> contextMenu = new CaptionMenu( mix->effectChannel( m_channelIndex )->m_name, this );
	if( m_channelIndex != 0 ) // no move-options in master 
	{
		contextMenu->addAction( tr( "Move &left" ),	this, SLOT( moveChannelLeft() ) );
		contextMenu->addAction( tr( "Move &right" ), this, SLOT( moveChannelRight() ) );
	}
	contextMenu->addAction( tr( "Rename &channel" ), this, SLOT( renameChannel() ) );
	contextMenu->addSeparator();

	if( m_channelIndex != 0 ) // no remove-option in master
	{
		contextMenu->addAction( embed::getIconPixmap( "cancel" ), tr( "R&emove channel" ),
							this, SLOT( removeChannel() ) );
		contextMenu->addSeparator();
	}

	contextMenu->addAction( embed::getIconPixmap( "cancel" ), tr( "Remove &unused channels" ),
						this, SLOT( removeUnusedChannels() ) );
	contextMenu->addSeparator();

	contextMenu->addHelpAction();
	contextMenu->exec( QCursor::pos() );
	delete contextMenu;
}


void FxLine::renameChannel()
{
	bool ok;
	FxMixer * mix = Engine::fxMixer();
	QString new_name = QInputDialog::getText( this,
			FxMixerView::tr( "Rename FX channel" ),
			FxMixerView::tr( "Enter the new name for this "
						"FX channel" ),
			QLineEdit::Normal, mix->effectChannel(m_channelIndex)->m_name, &ok );
	if( ok && !new_name.isEmpty() )
	{
		mix->effectChannel( m_channelIndex )->m_name = new_name;
		update();
		updateChannelName(new_name);
	}
}


void FxLine::removeChannel()
{
	FxMixerView * mix = gui->fxMixerView();
	mix->deleteChannel( m_channelIndex );
}


void FxLine::removeUnusedChannels()
{
	FxMixerView * mix = gui->fxMixerView();
	mix->deleteUnusedChannels();
}


void FxLine::moveChannelLeft()
{
	FxMixerView * mix = gui->fxMixerView();
	mix->moveChannelLeft( m_channelIndex );
}


void FxLine::moveChannelRight()
{
	FxMixerView * mix = gui->fxMixerView();
	mix->moveChannelRight( m_channelIndex );
}


void FxLine::displayHelp()
{
	QWhatsThis::showText( mapToGlobal( rect().bottomRight() ),
								whatsThis() );
}

QBrush FxLine::backgroundActive() const
{
	return m_backgroundActive;
}

void FxLine::setBackgroundActive( const QBrush & c )
{
	m_backgroundActive = c;
}



