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
  std::string legendBody = "";
  Font fontLegend;
  fontLegend.setPointSize(8);
  Font fontTitle;
  fontTitle.setPointSize(12);
  te::color::RGBAColor fontLegendColor(0, 0, 0, 255);
  te::color::RGBAColor fontTitleColor(0, 0, 0, 255);
  std::list<te::map::AbstractLayerPtr> layerList;
  double borderDisplacement = 3.;
  double displacementBetweenSymbols = 5.;
  double displacementBetweenTitleAndSymbols = 4.;
  double displacementBetweenSymbolsAndText = 1.;
  double symbolSize = 7.;
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
    property.setName("font_title");
    property.setLabel(TR_LAYOUT("Font Title"));
    property.setValue(fontTitle, dataType->getDataTypeFont());
    property.setMenu(true);
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("font_title_color");
    property.setLabel(TR_LAYOUT("Font Title Color"));
    property.setValue(fontTitleColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  // Legend Configuration

  Property prop_legend_body(0);
  prop_legend_body.setName("legend_body");
  prop_legend_body.setLabel(TR_LAYOUT("Legend Body"));
  prop_legend_body.setValue(legendBody, dataType->getDataTypeGroup());
  {
    Property property(0);
    property.setName("font_legend");
    property.setLabel(TR_LAYOUT("Font Legend"));
    property.setValue(fontLegend, dataType->getDataTypeFont());
    property.setMenu(true);
    prop_legend_body.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("font_legend_color");
    property.setLabel(TR_LAYOUT("Font Legend Color"));
    property.setValue(fontLegendColor, dataType->getDataTypeColor());
    property.setMenu(true);
    prop_legend_body.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("border_displacement");
    property.setLabel(TR_LAYOUT("Border Displacement"));
    property.setValue(borderDisplacement, dataType->getDataTypeDouble());
    prop_legend_body.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("displacement_between_symbols");
    property.setLabel(TR_LAYOUT("Displacement Between Symbols"));
    property.setValue(displacementBetweenSymbols, dataType->getDataTypeDouble());
    prop_legend_body.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("displacement_between_title_and_symbols");
    property.setLabel(TR_LAYOUT("Displacement Between Title And Symbols"));
    property.setValue(displacementBetweenTitleAndSymbols, dataType->getDataTypeDouble());
    prop_legend_body.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("displacement_between_symbols_and_texts");
    property.setLabel(TR_LAYOUT("Displacement Between Symbols And Texts"));
    property.setValue(displacementBetweenSymbolsAndText, dataType->getDataTypeDouble());
    prop_legend_body.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("symbol_size");
    property.setLabel(TR_LAYOUT("Symbol Size"));
    property.setValue(symbolSize, dataType->getDataTypeDouble());
    prop_legend_body.addSubProperty(property);
  }
  m_properties.addProperty(prop_legend_body);

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

