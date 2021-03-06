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
  \file terralib/layout/qt/core/pattern/factory/editor/BoolCheckBoxEditorFactory.cpp

  \brief This is the concrete factory for zoom area tools.
*/

// TerraLib
#include "EnvelopePropertyTreeItemFactory.h"
#include "PropertyTreeItemFactoryParamsCreate.h"
#include "../../../../../../core/enum/Enums.h"
#include "../../../../propertyeditor/tree/EnvelopePropertyTreeItem.h"
#include "../../../../../../core/property/Property.h"
#include "../../../../propertyeditor/tree/PropertyTreeItem.h"

// Qt
#include <QMetaType>
#include <QVariant>

te::layout::PropertyTreeItem* te::layout::EnvelopePropertyTreeItemFactory::build(PropertyTreeItemFactoryParamsCreate params)
{
  Property prop = params.getProperty();
  QTreeWidgetItem* parent = params.getParent();

  return createEnvelopeDataTreeItem(prop, parent);
}

te::layout::EnvelopePropertyTreeItemFactory::EnvelopePropertyTreeItemFactory() :
  PropertyTreeItemFactory(Enums::getInstance().getEnumDataType()->getDataTypeEnvelope()->getName())
{

}

te::layout::EnvelopePropertyTreeItemFactory::~EnvelopePropertyTreeItemFactory()
{

}

te::layout::PropertyTreeItem* te::layout::EnvelopePropertyTreeItemFactory::createEnvelopeDataTreeItem(Property & prop, QTreeWidgetItem* parent)
{
  EnvelopePropertyTreeItem* envelopePropertyTreeItem = new EnvelopePropertyTreeItem(prop, parent);

  te::gm::Envelope env = te::layout::Property::GetValueAs<te::gm::Envelope>(prop);
  if (!env.isValid())
  {
    env = te::gm::Envelope(0, 0, 0, 0);
  }

  QString firstChildName("x1");
  double  childValue = env.getLowerLeftX();
  te::layout::PropertyTreeItem* x1 = createDataTreeItemChild(firstChildName, childValue, envelopePropertyTreeItem);

  QString secondChildName("y1");
  childValue = env.getLowerLeftY();
  createDataTreeItemChild(secondChildName, childValue, envelopePropertyTreeItem);

  QString thirdChildName("x2");
  childValue = env.getUpperRightX();
  createDataTreeItemChild(thirdChildName, childValue, envelopePropertyTreeItem);

  QString fourthChildName("y2");
  childValue = env.getUpperRightY();
  createDataTreeItemChild(fourthChildName, childValue, envelopePropertyTreeItem);
  
  return envelopePropertyTreeItem;
}

te::layout::PropertyTreeItem* te::layout::EnvelopePropertyTreeItemFactory::createDataTreeItemChild(QString name, double value, QTreeWidgetItem* parent)
{
  int propertyType = qMetaTypeId<te::layout::Property>();
  EnumType* type = Enums::getInstance().getEnumDataType()->getDataTypeDouble();

  Property prop;
  prop.setName(name.toStdString());

  double dValue = value;
  prop.setValue(dValue, type);

  PropertyTreeItem* newItem = new PropertyTreeItem(prop, parent); //Detail to generate a hierarchical tree, the parent must be passed in the constructor QTreeWidgetItem
  newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
  newItem->setText(0, name); // Property Name
  newItem->setData(0, Qt::UserRole, QVariant(name)); // Property Name

  // Second Column
  // te::layout::Property to QVariant (Wrapper)
  QVariant variant = QVariant::fromValue<te::layout::Property>(prop);
  newItem->setData(1, propertyType, variant); // Property Value

  return newItem;
}

