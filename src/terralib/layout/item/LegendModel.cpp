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
#include "terralib/maptools/CanvasConfigurer.h"
#include "terralib/se/Symbolizer.h"
#include "terralib/se/Style.h"
#include "terralib/se/Rule.h"
#include "terralib/geometry/Polygon.h"
#include "terralib/geometry/Geometry.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/maptools/GroupingItem.h"
#include "terralib/maptools/Enums.h"
#include "../core/Utils.h"

#include <terralib/core/translator/Translator.h>

// STL
#include <string>
#include <sstream> 

te::layout::LegendModel::LegendModel()
  : AbstractItemModel()
{
  std::string nameSettings = "Settings";
  std::string mapName = "";
  std::string legendBody = "";
  Font fontLegend;
  fontLegend.setPointSize(8);
  Font fontTitle;
  fontTitle.setPointSize(12);
  te::color::RGBAColor fontLegendColor(0, 0, 0, 255);
  te::color::RGBAColor fontTitleColor(0, 0, 0, 255);
  double borderDisplacement = 3.;
  double displacementBetweenSymbols = 1.;
  double displacementBetweenTitles = 4.;
  double displacementBetweenSymbolsAndText = 1.;
  double symbolSize = 7.;
  std::string itemName = "";
  std::vector<std::string>  vString;
  int rows = 4;
  int columns = 1;
  double offset = 2.;
  double groupingOffset = 0.;
  double lineWidth = Utils::getLineWidthMinimumValue();

  double width = 100.;
  double height = 80.;

  SharedProperties sharedProps;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties

  {
    Property property(0);
    property.setName("line_width");
    property.setLabel(TE_TR("Line Width"));
    property.setVisible(false);
    property.setValue(lineWidth, dataType->getDataTypeDouble());
    this->m_properties.addProperty(property);
  }

  {
    Property property;
    property.setName("layers_uri");
    property.setLabel(TE_TR("URI"));
    property.setValue(vString, dataType->getDataTypeStringVector());
    property.setEditable(false);
    property.setVisible(false);
    property.setSerializable(false);
    m_properties.addProperty(property);
  }
  {
    Property property;
    property.setName("visible_layers_uri");
    property.setLabel(TE_TR("Visible URI"));
    property.setValue(vString, dataType->getDataTypeStringVector());
    property.setEditable(false);
    property.setVisible(false);
    m_properties.addProperty(property);
  }
  // Observer pattern relationship. Associate: != 0 / Dissociate : == 0.
  {
    Property property(0);
    property.setName(sharedProps.getItemObserver());
    property.setLabel(TE_TR("Connection with"));
    property.setComposeWidget(true);
    property.setValue(itemName, dataType->getDataTypeItemObserver());
    m_properties.addProperty(property);
  }

  // Legend Configuration
  
  Property prop_settings(0);
  prop_settings.setName("legendChoice");
  prop_settings.setLabel(TE_TR("Legend Settings"));
  prop_settings.setValue(nameSettings, dataType->getDataTypeLegendChoice());
  prop_settings.setMenu(true);
  prop_settings.setVisible(true);  
  {
    Property property(0);
    property.setName("font_title");
    property.setLabel(TE_TR("Font Title"));
    property.setValue(fontTitle, dataType->getDataTypeFont());
    property.setMenu(true);
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("font_title_color");
    property.setLabel(TE_TR("Font Title Color"));
    property.setValue(fontTitleColor, dataType->getDataTypeColor());
    property.setMenu(true);
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("font_legend");
    property.setLabel(TE_TR("Font Legend"));
    property.setValue(fontLegend, dataType->getDataTypeFont());
    property.setMenu(true);
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("font_legend_color");
    property.setLabel(TE_TR("Font Legend Color"));
    property.setValue(fontLegendColor, dataType->getDataTypeColor());
    property.setMenu(true);
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("border_displacement");
    property.setLabel(TE_TR("Border Displacement"));
    property.setValue(borderDisplacement, dataType->getDataTypeDouble());
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("displacement_between_symbols");
    property.setLabel(TE_TR("Displacement Between Symbols"));
    property.setValue(displacementBetweenSymbols, dataType->getDataTypeDouble());
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("displacement_between_titles");
    property.setLabel(TE_TR("Displacement Between Titles"));
    property.setValue(displacementBetweenTitles, dataType->getDataTypeDouble());
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("displacement_between_symbols_and_texts");
    property.setLabel(TE_TR("Displacement Between Symbols And Texts"));
    property.setValue(displacementBetweenSymbolsAndText, dataType->getDataTypeDouble());
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("symbol_size");
    property.setLabel(TE_TR("Symbol Size"));
    property.setValue(symbolSize, dataType->getDataTypeDouble());
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("rows");
    property.setLabel(TE_TR("Rows"));
    property.setValue(rows, dataType->getDataTypeInt());
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("offset_between_columns");
    property.setLabel(TE_TR("Offset Between Columns"));
    property.setValue(offset, dataType->getDataTypeDouble());
    prop_settings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName("grouping_offset_pair");
    property.setLabel(TE_TR("Grouping Offset Pair"));
    property.setValue(groupingOffset, dataType->getDataTypeDouble());
    prop_settings.addSubProperty(property);
  }
  m_properties.addProperty(prop_settings);

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
    property.setValue<bool>(true, dataType->getDataTypeBool());
    m_properties.updateProperty(property);
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getLegendItem());
}

te::layout::LegendModel::~LegendModel()
{

}
