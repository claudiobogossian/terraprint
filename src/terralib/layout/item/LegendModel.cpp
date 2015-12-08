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
  \file LegendModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendModel.h"
#include "../core/enum/Enums.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"
#include "../core/property/SharedProperties.h"


//#include "LegendModel.h"
//#include "MapModel.h"
//#include "../core/property/Property.h"
//#include "../core/property/Properties.h"
//#include "../core/property/SharedProperties.h"
#include "terralib/maptools/CanvasConfigurer.h"
#include "terralib/se/Symbolizer.h"
#include "terralib/se/Style.h"
#include "terralib/se/Rule.h"
#include "terralib/geometry/Polygon.h"
#include "terralib/geometry/Geometry.h"
#include "terralib/geometry/Envelope.h"
//#include "../core/enum/Enums.h"
#include "terralib/maptools/GroupingItem.h"
#include "terralib/maptools/Enums.h"

// STL
#include <string>
#include <sstream> 



te::layout::LegendModel::LegendModel()
  : AbstractItemModel()
  , Observer()
{
  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getLegendItem());

  std::string name = "";
  std::string mapName = "";
  Font font;
  font.setPointSize(8);
  te::color::RGBAColor fontColor(0, 0, 0, 255);
  std::list<te::map::AbstractLayerPtr> layerList;
  double borderDisplacement = 1;
  double displacementBetweenSymbols = 7;
  double displacementBetweenTitleAndSymbols = 7;
  double displacementBetweenSymbolsAndText = 2;
  double symbolsize = 5;
  std::string itemName = "";

  double width = 70.;
  double height = 50.;

  SharedProperties sharedProps;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  {
    Property property(0);
    property.setName("legendChoice");
    property.setLabel(TR_LAYOUT("Select the legend"));
    property.setValue(name, dataType->getDataTypeLegendChoice());
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
    Property property(0);
    property.setName("font");
    property.setLabel(TR_LAYOUT("Font"));
    property.setValue(font, dataType->getDataTypeFont());
    property.setMenu(true);
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("font_color");
    property.setLabel(TR_LAYOUT("Font Color"));
    property.setValue(fontColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  //updating properties
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    m_properties.updateProperty(property);
  }
  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
    m_properties.updateProperty(property);
  }
  {
    Property property(0);
    property.setName("show_frame");
    property.setValue(true, dataType->getDataTypeBool());
    m_properties.updateProperty(property);
  }

}

te::layout::LegendModel::~LegendModel()
{

}

void te::layout::LegendModel::update(const Subject* subject)
{
  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if(subjectModel == 0)
  {
    return;
  }

  const Property& pLayersNew = subjectModel->getProperty("layers");
  const Property& pLayersCurrent = this->getProperty("layers");

  const std::list<te::map::AbstractLayerPtr>& layersNew = pLayersNew.getValue().toLayerList();
  const std::list<te::map::AbstractLayerPtr>& layersCurrent = pLayersCurrent.getValue().toLayerList();

  if(layersNew != layersCurrent)
  {
    setProperty(pLayersNew);
  }
}

