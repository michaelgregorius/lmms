/*
 * LmmsComboBox.cpp - A combo box that knows how to wrap ComboBoxModels based on QComboBox
 *
 * Copyright (c) 2024 Michael Gregorius
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


#include "LmmsComboBox.h"

namespace lmms::gui
{

LmmsComboBox::LmmsComboBox(QWidget* parent, ComboBoxModel* comboBoxModel) :
	QComboBox(parent),
	m_comboBoxModel(comboBoxModel)
{
	// TODO For now assume that the model is never null
	connect(m_comboBoxModel, &ComboBoxModel::dataChanged, this, &LmmsComboBox::onDataChanged);
	connect(m_comboBoxModel, &ComboBoxModel::propertiesChanged, this, &LmmsComboBox::onPropertiesChanged);
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onIndexChanged(int)));

	onPropertiesChanged();
	onDataChanged();
}

void LmmsComboBox::onDataChanged()
{
	const auto index = m_comboBoxModel->value();

	if (currentIndex() != index)
	{
		setCurrentIndex(index);
	}
}

void LmmsComboBox::onPropertiesChanged()
{
	clear();

	for (int i = 0; i < m_comboBoxModel->size(); ++i)
	{
		const auto text = m_comboBoxModel->itemText(i);
		const auto pixmapLoader = m_comboBoxModel->itemPixmap(i);
		if (pixmapLoader)
		{
			QIcon icon = QIcon(pixmapLoader->pixmap());
			addItem(icon, text);
		}
		else
		{
			addItem(text);
		}
	}

	onDataChanged();
}

void LmmsComboBox::onIndexChanged(int index)
{
	if (index != -1 && index != m_comboBoxModel->value())
	{
		m_comboBoxModel->setValue(index);
	}
}

} // namespace lmms::gui
