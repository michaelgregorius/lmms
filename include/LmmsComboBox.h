/*
 * LmmsComboBox.h - A combo box that knows how to wrap ComboBoxModels based on QComboBox
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

#ifndef LMMS_GUI_LMMSCOMBOBOX_H
#define LMMS_GUI_LMMSCOMBOBOX_H

#include <QComboBox>

#include "ComboBoxModel.h"

namespace lmms::gui
{

class LMMS_EXPORT LmmsComboBox : public QComboBox
{
	Q_OBJECT
public:
	LmmsComboBox(QWidget* parent, ComboBoxModel* comboBoxModel);
	~LmmsComboBox() = default;

private slots:
	void onDataChanged();
	void onPropertiesChanged();
	void onIndexChanged(int index);

private:
	ComboBoxModel* m_comboBoxModel;
} ;

} // namespace lmms::gui

#endif // LMMS_GUI_LMMSCOMBOBOX_H
