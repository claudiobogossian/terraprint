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
#include "../core/Utils.h"
#include "../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../core/property/GridSettingsConfigProperties.h"
#include "../core/property/PlanarGridSettingsConfigProperties.h"
#include "../core/property/SharedProperties.h"
#include "../core/enum/Enums.h"
#include "../core/enum/EnumTextFormatType.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"

#include <terralib/core/translator/Translator.h>

// STL
#include <vector>

te::layout::MapModel::MapModel()
  : AbstractItemModel()
{
  te::color::RGBAColor backgroundColor(255, 255, 255, 255);
  double width = 120.;
  double height = 120.;
  te::gm::Envelope mapLocalBox(0, 0, width, height);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

//adding properties
  {
    std::string value = TE_TR("Choice");
    Property property(0);
    property.setName("mapChoice");
    property.setLabel(TE_TR("Select layers"));
    property.setValue(value, dataType->getDataTypeMapChoice());
    property.setSerializable(false);
    property.setVisible(false);
    property.setMenu(false);
    m_properties.addProperty(property);
  }

  /* map_local_box property (millimeter) is one to one with the 
    world_box (projection system) when pass by transformation */
  {
    Property property(0);
    property.setName("map_local_box");
    property.setValue(mapLocalBox, dataType->getDataTypeDouble());
    property.setVisible(false);
    m_properties.addProperty(property);
  }

  initializeMapSettings();
  initializePlanarGridSettings();
  initializeGeodesicGridSettings();

//updating properties
  {
    Property property(0);
    property.setName("show_frame");
    property.setValue<bool>(true, dataType->getDataTypeBool());
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
    property.setValue<bool>(true, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }
  reparentProperties(Enums::getInstance().getEnumObjectType()->getMapItem());
}

te::layout::MapModel::~MapModel()
{
}

te::layout::Property te::layout::MapModel::getBasicGridSettings(GridSettingsConfigProperties* settingsConfig) const
{
  if (settingsConfig == 0)
  {
    throw te::common::Exception(TE_TR("Invalid attribute in MapModel"));
  }

  //Reference Settings
  double scale = 0.;

  // Grid 
  bool visible = true;
  double lneHrzGap = 1000.; // 0.
  double lneVrtGap = 1000.; // 0.
  double m_initialGridPointX = 0.;
  double m_initialGridPointY = 0.;

  // Line
  te::color::RGBAColor lineColor(0, 0, 0, 255);
  double lineWidth = te::layout::Utils::getLineWidthMinimumValue();

  // Text: Basic Configuration
  int pointTextSize = 6;
  std::string fontFamily("Arial");
  te::color::RGBAColor textColor(0, 0, 0, 255);

  // Text: Advanced configuration
  Font font;
  bool visibleAllTexts = true;
  bool superscriptText = false;
  double lneVrtDisplacement = 1.;
  double lneHrzDisplacement = 1.;
  bool bottomText = true;
  bool leftText = true;
  bool rightText = true;
  bool topText = true;
  bool bottomRotateText = false;
  bool leftRotateText = false;
  bool rightRotateText = false;
  bool topRotateText = false;

  bool showBouderIntersections = true;

  double crossOffSet = 2.;

  std::string itemName = "";

  int planarSRID = -1;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  SharedProperties sharedProps;

  // Grid settings
  std::string gridValue = TE_TR("Settings");
  Property prop_gridsettings(0);
  prop_gridsettings.setName(settingsConfig->getGridSettings());
  prop_gridsettings.setLabel(TE_TR("Grid Settings"));
  prop_gridsettings.setMenu(true);
  prop_gridsettings.setPublic(true);
  prop_gridsettings.setValue(gridValue, dataType->getDataTypeGridSettings());

  // Grid: sub properties
  {
    Property property(0);
    property.setName(settingsConfig->getVisible());
    property.setLabel(TE_TR("Visible"));
    property.setComposeWidget(true);
    property.setValue(visible, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getHorizontalLineGap());
    property.setLabel(TE_TR("Horizontal Gap"));
    property.setComposeWidget(true);
    property.setValue(lneHrzGap, dataType->getDataTypeDouble());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getVerticalLineGap());
    property.setLabel(TE_TR("Vertical Gap"));
    property.setComposeWidget(true);
    property.setValue(lneVrtGap, dataType->getDataTypeDouble());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getHorizontalLineInitial());
    property.setLabel(TE_TR("Initial Point X"));
    property.setComposeWidget(true);
    property.setValue(m_initialGridPointX, dataType->getDataTypeDouble());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getVerticalLineInitial());
    property.setLabel(TE_TR("Initial Point Y"));
    property.setComposeWidget(true);
    property.setValue(m_initialGridPointY, dataType->getDataTypeDouble());
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig->getBorderIntersections());
    property.setLabel(TE_TR("Show Border Intersection"));
    property.setValue(showBouderIntersections, dataType->getDataTypeBool());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }
  //Just one is visible
  {
    EnumGridStyleType gridStyleType;
    EnumType* currentGridStyle = gridStyleType.getStyleContinuous();

    Property property(0);
    property.setName(settingsConfig->getStyle());
    property.setLabel(TE_TR("Style"));
    property.setComposeWidget(true);
    property.setValue(currentGridStyle->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentGridStyle->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < gridStyleType.size(); ++i)
    {
      EnumType* enumType = gridStyleType.getEnum(i);

      if (enumType == gridStyleType.getStyleNone() || enumType == currentGridStyle)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    prop_gridsettings.addSubProperty(property);
  }

  // Line
  {
    EnumLineStyleType lineStyleType;
    EnumType* currentLineStyle = lineStyleType.getStyleSolid();

    Property property(0);
    property.setName(settingsConfig->getLineStyle());
    property.setLabel(TE_TR("Line Style"));
    property.setComposeWidget(true);
    property.setValue(currentLineStyle->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentLineStyle->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < lineStyleType.size(); ++i)
    {
      EnumType* enumType = lineStyleType.getEnum(i);

      if (enumType == lineStyleType.getStyleNone() || enumType == currentLineStyle)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig->getLineColor());
    property.setLabel(TE_TR("Line Color"));
    property.setComposeWidget(true);
    property.setValue(lineColor, dataType->getDataTypeColor());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig->getLineWidth());
    property.setLabel(TE_TR("Line Width"));
    property.setComposeWidget(true);
    property.setValue(lineWidth, dataType->getDataTypeDouble());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }


  {
    Property property(0);
    property.setName(settingsConfig->getFont());
    property.setLabel(TE_TR("Font"));
    property.setValue(font, dataType->getDataTypeFont());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig->getTextColor());
    property.setLabel(TE_TR("Text Color"));
    property.setComposeWidget(true);
    property.setValue(textColor, dataType->getDataTypeColor());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }

  // Text: Advanced configuration
  {
    Property property(0);
    property.setName(settingsConfig->getVisibleAllTexts());
    property.setLabel(TE_TR("All Texts Visible"));
    property.setComposeWidget(true);
    property.setValue(visibleAllTexts, dataType->getDataTypeBool());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig->getSuperscriptText());
    property.setLabel(TE_TR("Superscript"));
    property.setComposeWidget(true);
    property.setVisible(false); //need review
    property.setValue(superscriptText, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig->getHorizontalLineDisplacement());
    property.setLabel(TE_TR("Horizontal Displacement"));
    property.setComposeWidget(true);
    property.setValue(lneHrzDisplacement, dataType->getDataTypeDouble());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig->getVerticalLineDisplacement());
    property.setLabel(TE_TR("Vertical Displacement"));
    property.setComposeWidget(true);
    property.setValue(lneVrtDisplacement, dataType->getDataTypeDouble());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig->getBottomText());
    property.setLabel(TE_TR("Bottom Text Visible"));
    property.setComposeWidget(true);
    property.setValue(bottomText, dataType->getDataTypeBool());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getLeftText());
    property.setLabel(TE_TR("Left Text Visible"));
    property.setComposeWidget(true);
    property.setValue(leftText, dataType->getDataTypeBool());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getRightText());
    property.setLabel(TE_TR("Right Text Visible"));
    property.setComposeWidget(true);
    property.setValue(rightText, dataType->getDataTypeBool());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getTopText());
    property.setLabel(TE_TR("Top Text Visible"));
    property.setComposeWidget(true);
    property.setValue(topText, dataType->getDataTypeBool());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getBottomRotateText());
    property.setLabel(TE_TR("Bottom Text Rotate"));
    property.setComposeWidget(true);
    property.setValue(bottomRotateText, dataType->getDataTypeBool());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getLeftRotateText());
    property.setLabel(TE_TR("Left Text Rotate"));
    property.setComposeWidget(true);
    property.setValue(leftRotateText, dataType->getDataTypeBool());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getRightRotateText());
    property.setLabel(TE_TR("Right Text Rotate"));
    property.setComposeWidget(true);
    property.setValue(rightRotateText, dataType->getDataTypeBool());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getTopRotateText());
    property.setLabel(TE_TR("Top Text Rotate"));
    property.setComposeWidget(true);
    property.setValue(topRotateText, dataType->getDataTypeBool());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig->getCrossOffset());
    property.setLabel(TE_TR("Cross Offset"));
    property.setComposeWidget(true);
    property.setValue(crossOffSet, dataType->getDataTypeDouble());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }
  {
    /* The grids should not always change position, 
    this could occur if the zone was recalculated at all times. 
    To solve, in the initialization of the grids is found the planar srid and saved. */
    Property property(0);
    property.setName(settingsConfig->getPlanarSRID());
    property.setLabel(TE_TR("Planar SRID"));
    property.setComposeWidget(true);
    property.setValue(planarSRID, dataType->getDataTypeInt());
    property.setVisible(false);
    prop_gridsettings.addSubProperty(property);
  }

  return prop_gridsettings;
}

void te::layout::MapModel::initializeMapSettings()
{
  bool enableChildren = true;
  te::gm::Envelope worldBox;
  int srid = -1;
  bool fixedScale = false;
  double scale = 0;
  std::list<te::map::AbstractLayerPtr> layerList;
  std::vector<std::string>  vString;
  std::string unit = "Millimeter";
  //double map
  double mapWidth = 120.;
  double mapHeight = 120.;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  std::string value = TE_TR("Settings");
  Property pro_mapSettings(0);
  pro_mapSettings.setName("mapSettings");
  pro_mapSettings.setLabel(TE_TR("Map Settings"));
  pro_mapSettings.setValue(value, dataType->getDataTypeMapSettings());
  pro_mapSettings.setMenu(true);

  {
    Property property;
    property.setName("layers");
    property.setLabel(TE_TR("Layers"));
    property.setValue(layerList, dataType->getDataTypeLayerList());
    property.setEditable(false);
    property.setVisible(false);
    property.setSerializable(false);
    pro_mapSettings.addSubProperty(property);
  }

  {
    Property property;
    property.setName("layers_uri");
    property.setLabel(TE_TR("URI"));
    property.setValue(vString, dataType->getDataTypeStringVector());
    property.setEditable(false);
    property.setVisible(false);
    pro_mapSettings.addSubProperty(property);
  }

  /* map_local_box property (millimeter) is one to one with the
    world_box (projection system) when pass by transformation */
  {
    Property property(0);
    property.setName("world_box");
    property.setLabel(TE_TR("World Box"));
    property.setValue(worldBox, dataType->getDataTypeEnvelope());
    pro_mapSettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName("srid");
    property.setEditable(false);
    property.setLabel(TE_TR("SRID"));
    property.setValue(srid, dataType->getDataTypeInt());
    pro_mapSettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName("scale");
    property.setLabel(TE_TR("Scale"));
    property.setValue(scale, dataType->getDataTypeDouble());
    pro_mapSettings.addSubProperty(property);
  }

  {
    Property property;
    property.setName("size_unit");
    property.setLabel(TE_TR("Size Unit"));
    property.setValue(unit, dataType->getDataTypeStringList());
    property.setVisible(false);
    Variant v;
    v.setValue(std::string("Millimeter"), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    v.setValue(std::string("Centimeter"), dataType->getDataTypeString());
    property.addOption(v);

    property.setVisible(false);

    property.setSerializable(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("fixed_scale");
    property.setLabel(TE_TR("Fixed Scale"));
    property.setValue(fixedScale, dataType->getDataTypeBool());
    pro_mapSettings.addSubProperty(property);
  }

  m_properties.addProperty(pro_mapSettings);
}

void te::layout::MapModel::initializePlanarGridSettings()
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  PlanarGridSettingsConfigProperties settingsConfig;

  Property pPlanarGridSettings = getBasicGridSettings(&settingsConfig);
  pPlanarGridSettings.setLabel(TE_TR("Planar Grid Settings"));

  bool superscript = false;

  //adding properties
  {
    Property property(0);
    property.setName(settingsConfig.getUnit());
    property.setLabel(TE_TR("Unit"));
    property.setValue(std::string("km"), dataType->getDataTypeStringList());
    property.setVisible(false);

    Variant v;
    v.setValue(std::string("km"), dataType->getDataTypeString());
    property.addOption(v);

    v.setValue(std::string("m"), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    pPlanarGridSettings.addSubProperty(property);
  }
  
  m_properties.addProperty(pPlanarGridSettings); // add gridsettings property

  {
    Property property(0);
    property.setName(settingsConfig.getSuperscriptText());
    property.setLabel(TE_TR("Superscript"));
    property.setComposeWidget(true);
    property.setVisible(false);
    property.setValue(superscript, dataType->getDataTypeBool());
    m_properties.completelyUpdateProperty(property);
  }
}

void te::layout::MapModel::initializeGeodesicGridSettings()
{
  bool showDegreesText = true;
  bool showMinutesText = true;
  bool showSecondsText = true;

  int secPresicion = 0;
  int geodesicSRID = -1;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  GeodesicGridSettingsConfigProperties settingsConfig;

  Property pGeodesicGridSettings = getBasicGridSettings(&settingsConfig);
  pGeodesicGridSettings.setLabel(TE_TR("Geodesic Grid Settings"));

  {
    Property property(0);
    property.setName(settingsConfig.getDegreesText());
    property.setLabel(TE_TR("Show Degrees Text"));
    property.setValue(showDegreesText, dataType->getDataTypeBool());
    property.setVisible(false);
    pGeodesicGridSettings.addSubProperty(property); // update gridsettings property
  }
  {
    Property property(0);
    property.setName(settingsConfig.getMinutesText());
    property.setLabel(TE_TR("Show Minutes Text"));
    property.setValue(showMinutesText, dataType->getDataTypeBool());
    property.setVisible(false);
    pGeodesicGridSettings.addSubProperty(property); // update gridsettings property
  }
  {
    Property property(0);
    property.setName(settingsConfig.getSecondsText());
    property.setLabel(TE_TR("Show Seconds Text"));
    property.setValue(showSecondsText, dataType->getDataTypeBool());
    property.setVisible(false);
    pGeodesicGridSettings.addSubProperty(property); // update gridsettings property
  }
  {
    //seconds precision
    Property property(0);
    property.setName(settingsConfig.getSecondsPrecisionText());
    property.setLabel(TE_TR("Seconds Text Precision"));
    property.setValue(secPresicion, dataType->getDataTypeInt());
    property.setVisible(false);
    pGeodesicGridSettings.addSubProperty(property); // update gridsettings property
  }
  {
    /* The grids should not always change position,
    this could occur if the zone was recalculated at all times.
    To solve, in the initialization of the grids is found the geographic srid and saved. */
    Property property(0);
    property.setName(settingsConfig.getGeodesicSRID());
    property.setLabel(TE_TR("Geodesic SRID"));
    property.setComposeWidget(true);
    property.setValue(geodesicSRID, dataType->getDataTypeInt());
    property.setVisible(false);
    pGeodesicGridSettings.addSubProperty(property);
  }

  // Text Format
  {
    EnumTextFormatType textFormatType;
    EnumType* defaultTextFormat = textFormatType.getDefaultFormat();

    Property property(0);
    property.setName(settingsConfig.getTextFormat());
    property.setLabel(TE_TR("Text Format"));
    property.setComposeWidget(true);
    property.setValue(defaultTextFormat->getLabel(), dataType->getDataTypeStringList());
    property.setVisible(false);

    for (int i = 0; i < textFormatType.size(); ++i)
    {
      EnumType* enumType = textFormatType.getEnum(i);

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);

      if (defaultTextFormat->getName() == enumType->getName())
      {
        property.setOptionChoice(v);
      }
    }

    pGeodesicGridSettings.addSubProperty(property); // update gridsettings property
  }

  m_properties.addProperty(pGeodesicGridSettings); // add gridsettings property


  //we turn off the geodesic grid
  {
    Property property(0);
    property.setName(settingsConfig.getVisible());
    property.setValue(false, dataType->getDataTypeBool());

    m_properties.updateProperty(property);
  }

}
