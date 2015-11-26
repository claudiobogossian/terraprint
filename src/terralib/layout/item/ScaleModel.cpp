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
  \file ScaleModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ScaleModel.h"
#include "../core/enum/Enums.h"
#include "../core/enum/EnumScaleType.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"
#include "../core/property/SharedProperties.h"
#include "../core/pattern/mvc/AbstractItemView.h"

te::layout::ScaleModel::ScaleModel()
  : AbstractItemModel()
  , Observer()
{
  this->m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getScaleItem());

  double width = 70.;
  double height = 30.;
  double scale = 250000.;
  double scaleGapX = 20.;
  double scaleGapY = 5.;
  std::string itemName = "";

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  SharedProperties sharedProps;

//adding properties
  {
    Property property(0);
    property.setName("scale");
    property.setLabel(TR_LAYOUT("Scale"));
    property.setValue(scale, dataType->getDataTypeDouble());
    property.setEditable(false);
    property.setMenu(true);
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("scale_width_rect_gap");
    property.setLabel(TR_LAYOUT("Scale Gap X"));
    property.setValue(scaleGapX, dataType->getDataTypeDouble());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("scale_height_rect_gap");
    property.setLabel(TR_LAYOUT("Scale Gap Y"));
    property.setValue(scaleGapY, dataType->getDataTypeDouble());
    property.setMenu(true);
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

  {
    EnumScaleType enumScaleType;
    EnumType* currentScaleType = enumScaleType.getDoubleAlternatingScaleBarType();

    Property property(0);
    property.setName("scale_type");
    property.setLabel(TR_LAYOUT("Graphic type"));
    property.setValue(currentScaleType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentScaleType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < enumScaleType.size(); ++i)
    {
      EnumType* enumType = enumScaleType.getEnum(i);

      if (enumType == enumScaleType.getNoneType() || enumType == currentScaleType)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("Unit");
    property.setLabel(TR_LAYOUT("Unit"));
    property.setValue(std::string("km"), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(std::string("km"), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    v.setValue(std::string("m"), dataType->getDataTypeString());
    property.addOption(v);

    m_properties.addProperty(property);
  }

//updating properties
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    this->m_properties.updateProperty(property);
  }

  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
    this->m_properties.updateProperty(property);
  }
}

te::layout::ScaleModel::~ScaleModel()
{
}

void te::layout::ScaleModel::update(const Subject* subject)
{
  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if(subjectModel == 0)
  {
    return;
  }

  const Property& pScaleNew = subjectModel->getProperty("scale");
  const Property& pScaleCurrent = this->getProperty("scale");
  double scaleNew = pScaleNew.getValue().toDouble();
  double scaleCurrent = pScaleCurrent.getValue().toDouble();

  if(scaleNew != scaleCurrent)
  {
    setProperty(pScaleNew);
  }
}
