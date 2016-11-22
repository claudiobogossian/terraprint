/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file PropertyTree.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnvelopePropertyTreeItem.h"

// Qt
#include <QString>
#include <QVariant>

te::layout::EnvelopePropertyTreeItem::EnvelopePropertyTreeItem(Property & prop, QTreeWidgetItem* parent) :
  PropertyTreeItem(prop, parent),
  m_x1Name("x1"),
  m_y1Name("y1"),
  m_x2Name("x2"),
  m_y2Name("y2")
{
  setFlags(flags() & (~Qt::ItemIsEditable)); // remove editable flag
}

te::layout::EnvelopePropertyTreeItem::~EnvelopePropertyTreeItem()
{

}

void te::layout::EnvelopePropertyTreeItem::refresh(int column, int role, QString name, QVariant value, QTreeWidgetItem* childItem)
{
  if (column == 0)
    return;

  // get childItem value
  int propertyType = qRegisterMetaType<te::layout::Property>("te::layout::Property");
  te::layout::Property prop = qvariant_cast<te::layout::Property>(value);
  double doubleValue = te::layout::Property::GetValueAs<double>(prop);

  int dataRole = getDataRole();

  QVariant currentDataValue = data(1, dataRole).toRectF();
  int count = childCount();

  te::gm::Envelope env = te::layout::Property::GetValueAs<te::gm::Envelope>(m_property);
  double x = env.getLowerLeftX();
  double y = env.getLowerLeftY();
  double width = env.getWidth();
  double height = env.getHeight();

  for (int i = 0; i < count; ++i)
  {
    QTreeWidgetItem* itemChild = child(i);

    if (itemChild == childItem)
    {
      if (itemChild->data(0, Qt::UserRole) == name)
      {
        if (name == m_x1Name)
          env = te::gm::Envelope(doubleValue, y, x + width, y + height);
        if (name == m_y1Name)
          env = te::gm::Envelope(x, doubleValue, x + width, y + height);
        if (name == m_x2Name)
          env = te::gm::Envelope(x, y, doubleValue, y + height);
        if (name == m_y2Name)
          env = te::gm::Envelope(x, y, x + width, doubleValue);

        m_property.setValue(env, m_property.getType());

        // te::layout::Property to QVariant (Wrapper)
        QVariant variant = QVariant::fromValue<te::layout::Property>(m_property);
        setData(1, dataRole, variant);
        break;
      }
    }
  }
}

