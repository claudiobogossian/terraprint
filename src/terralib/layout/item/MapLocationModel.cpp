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
  \file MapLocationModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapLocationModel.h"
#include "../core/enum/Enums.h"
#include "../core/property/SharedProperties.h"


te::layout::MapLocationModel::MapLocationModel() 
  : MapModel()
{
  this->m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getMapLocationItem());

  SharedProperties sharedProps;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  te::gm::Envelope referenceBox;
  int srid = -1;
  te::color::RGBAColor referenceBoxFillColor(255, 0, 0, 50);
  te::color::RGBAColor referenceBoxContourColor(255, 0, 0, 255);

  std::string itemName;

  //adding properties
  {
    Property property(0);
    property.setName("reference_box");
    property.setVisible(false);
    property.setValue(referenceBox, dataType->getDataTypeEnvelope());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("reference_srid");
    property.setVisible(false);
    property.setValue(srid, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("reference_box_fill_color");
    property.setLabel(TR_LAYOUT("Reference Envelope Fill Color"));
    property.setValue(referenceBoxFillColor, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("reference_box_contour_color");
    property.setLabel(TR_LAYOUT("Reference Envelope Contour Color"));
    property.setValue(referenceBoxContourColor, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }

  // Observer pattern relationship. Associate: != 0 / Dissociate : == 0.
  {
    Property property(0);
    property.setName(sharedProps.getItemObserver());
    property.setLabel(TR_LAYOUT("Connection with"));
    property.setComposeWidget(true);
    property.setValue(itemName, dataType->getDataTypeItemObserver());
    m_properties.addProperty(property);
  }
}

te::layout::MapLocationModel::~MapLocationModel()
{

}

void te::layout::MapLocationModel::update(const Subject* subject)
{
  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if (subjectModel == 0)
  {
    return;
  }

  Property pBoxNew = subjectModel->getProperty("world_box");
  const Property& pBoxCurrent = this->getProperty("reference_box");
  te::gm::Envelope boxNew = pBoxNew.getValue().toEnvelope();
  te::gm::Envelope boxCurrent = pBoxCurrent.getValue().toEnvelope();

  Property pSridNew = subjectModel->getProperty("srid");
  const Property& pSridCurrent = this->getProperty("reference_srid");
  int sridNew = pSridNew.getValue().toInt();
  int sridCurrent = pSridCurrent.getValue().toInt();

  if (sridNew != sridCurrent || boxNew.equals(boxCurrent) == false)
  {
    pSridNew.setName("reference_srid");
    pBoxNew.setName("reference_box");

    Properties properties;
    properties.addProperty(pSridNew);
    properties.addProperty(pBoxNew);

    setProperties(properties);
  }
}
