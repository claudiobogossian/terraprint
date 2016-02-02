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
  \file MapModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapModel.h"
#include "../core/enum/Enums.h"
#include "../core/enum/EnumScaleType.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"

// STL
#include <vector>

te::layout::MapModel::MapModel()
  : AbstractItemModel()
{
  this->m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getMapItem());

  te::color::RGBAColor backgroundColor(255, 255, 255, 255);
  bool enableChildren = true;
  te::gm::Envelope worldBox;
  int srid = -1;
  bool fixedScale = false;
  double scale = 0;
  std::list<te::map::AbstractLayerPtr> layerList;
  double width = 120.;
  double height = 120.;
  std::vector<std::string>  vString;
  std::string unit = "Millimeter";

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

//adding properties
  {
    std::string value = "Choice";
    Property property(0);
    property.setName("mapChoice");
    property.setLabel(TR_LAYOUT("Select layers"));
    property.setValue(value, dataType->getDataTypeMapChoice());
    property.setSerializable(false);
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    std::string value = TR_LAYOUT("Settings");
    Property property(0);
    property.setName("mapSettings");
    property.setLabel(TR_LAYOUT("Map Settings"));
    property.setValue(value, dataType->getDataTypeMapSettings());
    property.setSerializable(false);
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    Property property;
    property.setName("layers");
    property.setLabel(TR_LAYOUT("Layers"));
    property.setValue(layerList, dataType->getDataTypeLayerList());
    property.setEditable(false);
    property.setVisible(false);
    property.setSerializable(false);
    m_properties.addProperty(property);
  }

  {
    Property property;
    property.setName("layers_uri");
    property.setLabel(TR_LAYOUT("URI"));
    property.setValue(vString, dataType->getDataTypeStringVector());
    property.setEditable(false);
    property.setVisible(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("world_box");
    property.setLabel(TR_LAYOUT("World Box"));
    property.setValue(worldBox, dataType->getDataTypeEnvelope());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("srid");
    property.setLabel(TR_LAYOUT("SRID"));
    property.setValue(srid, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("scale");
    property.setLabel(TR_LAYOUT("Scale"));
    property.setValue(scale, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }

  {
    Property property;
    property.setName("size_unit");
    property.setLabel(TR_LAYOUT("Size Unit"));
    property.setValue(unit, dataType->getDataTypeStringList());
    //property.setEditable(false);
    //property.setVisible(false);
    Variant v;
    v.setValue(std::string("Millimeter"), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    v.setValue(std::string("Centimeter"), dataType->getDataTypeString());
    property.addOption(v);

    property.setSerializable(false);
    m_properties.addProperty(property);
  }


//updating properties
  {
    Property property(0);
    property.setName("background_color");
    property.setValue(backgroundColor, dataType->getDataTypeColor());
    this->m_properties.updateProperty(property);
  }
  {
    Property property(0);
    property.setName("show_frame");
    property.setValue(true, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }

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

  {
    Property property(0);
    property.setName("editable");
    property.setValue(true, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }
}

te::layout::MapModel::~MapModel()
{
}
