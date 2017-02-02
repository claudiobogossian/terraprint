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
\file EnumDataType.cpp

\brief

\ingroup layout
*/

// TerraLib
#include "EnumDataType.h"

#include <terralib/core/translator/Translator.h>

// Qt
#include <QtCore>

te::layout::EnumDataType::EnumDataType() :
  m_dataTypeNone(0),
  m_dataTypeInt(0),
  m_dataTypeDouble(0),
  m_dataTypeLong(0),
  m_dataTypeFloat(0),
  m_dataTypeString(0),
  m_dataTypeBool(0),
  m_dataTypeColor(0),
  m_dataTypeGridSettings(0),
  m_dataTypeStringList(0),
  m_dataTypeFont(0),
  m_dataTypeEnvelope(0),
  m_dataTypeImage(0),
  m_dataTypeTextGridSettings(0),
  m_dataTypeGroup(0),
  m_dataTypeMapChoice(0),
  m_dataTypeLegendChoice(0),
  m_dataTypeLayerList(0),
  m_dataTypeStringVector(0),
  m_dataTypeStringMatrix(0),
  m_dataTypeSVGView(0),
  m_dataTypeGeometry(0),
  m_dataTypeItemObserver(0),
  m_dataTypeGroupProperties(0),
  m_dataTypeMapSettings(0),
  m_dataTypeScaleSettings(0),
  m_dataTypeNorthSettings(0),
  m_dataTypePageSetup(0),
  m_dataTypePath(0),
  m_dataTypeMapLocationSettings(0)
{
  init();
}

te::layout::EnumDataType::~EnumDataType()
{
  if (m_dataTypeNone)
  {
    delete m_dataTypeNone;
    m_dataTypeNone = 0;
  }
  if (m_dataTypeInt)
  {
    delete m_dataTypeInt;
    m_dataTypeInt = 0;
  }
  if (m_dataTypeDouble)
  {
    delete m_dataTypeDouble;
    m_dataTypeDouble = 0;
  }
  if (m_dataTypeLong)
  {
    delete m_dataTypeLong;
    m_dataTypeLong = 0;
  }
  if (m_dataTypeFloat)
  {
    delete m_dataTypeFloat;
    m_dataTypeFloat = 0;
  }
  if (m_dataTypeString)
  {
    delete m_dataTypeString;
    m_dataTypeString = 0;
  }
  if (m_dataTypeBool)
  {
    delete m_dataTypeBool;
    m_dataTypeBool = 0;
  }
  if (m_dataTypeGridSettings)
  {
    delete m_dataTypeGridSettings;
    m_dataTypeGridSettings = 0;
  }
  if (m_dataTypeStringList)
  {
    delete m_dataTypeStringList;
    m_dataTypeStringList = 0;
  }
  if (m_dataTypeFont)
  {
    delete m_dataTypeFont;
    m_dataTypeFont = 0;
  }
  if (m_dataTypeEnvelope)
  {
    delete m_dataTypeEnvelope;
    m_dataTypeEnvelope = 0;
  }
  if (m_dataTypeImage)
  {
    delete m_dataTypeImage;
    m_dataTypeImage = 0;
  }
  if (m_dataTypeTextGridSettings)
  {
    delete m_dataTypeTextGridSettings;
    m_dataTypeTextGridSettings = 0;
  }
  if (m_dataTypeGroup)
  {
    delete m_dataTypeGroup;
    m_dataTypeGroup = 0;
  }
  if (m_dataTypeMapChoice)
  {
    delete m_dataTypeMapChoice;
    m_dataTypeMapChoice = 0;
  }
  if (m_dataTypeLegendChoice)
  {
    delete m_dataTypeLegendChoice;
    m_dataTypeLegendChoice = 0;
  }
  if (m_dataTypeLayerList)
  {
    delete m_dataTypeLayerList;
    m_dataTypeLayerList = 0;
  }
  if (m_dataTypeStringVector)
  {
    delete m_dataTypeStringVector;
    m_dataTypeStringVector = 0;
  }
  if (m_dataTypeSVGView)
  {
    delete m_dataTypeSVGView;
    m_dataTypeSVGView = 0;
  }
  if (m_dataTypeGeometry)
  {
    delete m_dataTypeGeometry;
    m_dataTypeGeometry = 0;
  }
  if (m_dataTypeItemObserver)
  {
    delete m_dataTypeItemObserver;
    m_dataTypeItemObserver = 0;
  }
  if (m_dataTypeGroupProperties)
  {
    delete m_dataTypeGroupProperties;
    m_dataTypeGroupProperties = 0;
  }
  if (m_dataTypeMapSettings)
  {
    delete m_dataTypeMapSettings;
    m_dataTypeMapSettings = 0;
  }
  if (m_dataTypeScaleSettings)
  {
    delete m_dataTypeScaleSettings;
    m_dataTypeScaleSettings = 0;
  }
  if (m_dataTypeNorthSettings)
  {
    delete m_dataTypeNorthSettings;
    m_dataTypeNorthSettings = 0;
  }
  if (m_dataTypePageSetup)
  {
    delete m_dataTypePageSetup;
    m_dataTypePageSetup = 0;
  }
  if (m_dataTypePath)
  {
    delete m_dataTypePath;
    m_dataTypePath = 0;
  }

  if (m_dataTypeMapLocationSettings)
  {
    delete m_dataTypeMapLocationSettings;
    m_dataTypeMapLocationSettings = 0;
  }
}

void te::layout::EnumDataType::init()
{
  int id = 1;

  id += Qt::UserRole; // Qt::UserRole: The first role that can be used for application-specific purposes.

  m_dataTypeNone = createEnum(id, "None", this, TE_TR("None"));
  
  ++id;
  m_dataTypeInt = createEnum(id, "Int", this, TE_TR("Int"));

  ++id;
  m_dataTypeDouble = createEnum(id, "Double", this, TE_TR("Double"));

  ++id;
  m_dataTypeLong = createEnum(id, "Long", this, TE_TR("Long"));

  ++id;
  m_dataTypeFloat = createEnum(id, "Float", this, TE_TR("Float"));

  ++id;
  m_dataTypeString = createEnum(id, "String", this, TE_TR("String"));

  ++id;
  m_dataTypeBool = createEnum(id, "Bool", this, TE_TR("Bool"));

  ++id;
  m_dataTypeColor = createEnum(id, "Color", this, TE_TR("Color"));

  ++id;
  m_dataTypeGridSettings = createEnum(id, "GridSettings", this, TE_TR("Grid Settings"));

  ++id;
  m_dataTypeStringList = createEnum(id, "StringList", this, TE_TR("String List"));

  ++id;
  m_dataTypeFont = createEnum(id, "Font", this, TE_TR("Font"));

  ++id;
  m_dataTypeEnvelope = createEnum(id, "Envelope", this, TE_TR("Envelope"));

  ++id;
  m_dataTypeImage = createEnum(id, "Image", this, TE_TR("Image"));

  ++id;
  m_dataTypeTextGridSettings = createEnum(id, "TextGridSettings", this, TE_TR("Text Grid Settings"));

  ++id;
  m_dataTypeGroup = createEnum(id, "Group", this, TE_TR("Group"));

  ++id;
  m_dataTypeMapChoice = createEnum(id, "MapChoice", this, TE_TR("Map Choice"));

  ++id;
  m_dataTypeLegendChoice = createEnum(id, "LegendChoice", this, TE_TR("Legend Choice"));
  
  ++id;
  m_dataTypeLayerList = createEnum(id, "Layer", this, TE_TR("None"));

  ++id;
  m_dataTypeStringVector = createEnum(id, "StringVector", this, TE_TR("String Vector"));

  ++id;
  m_dataTypeStringMatrix = createEnum(id, "StringMatrix", this, TE_TR("String Matrix"));
  
  ++id;
  m_dataTypeSVGView = createEnum(id, "SVGView", this, TE_TR("SVG View"));

  ++id;
  m_dataTypeGeometry = createEnum(id, "Geometry", this, TE_TR("Geometry"));

  ++id;
  m_dataTypeItemObserver = createEnum(id, "ItemObserver", this, TE_TR("Item Observer"));

  ++id;
  m_dataTypeGroupProperties = createEnum(id, "GroupProperties", this, TE_TR("Group Properties"));

  ++id;
  m_dataTypeMapSettings = createEnum(id, "MapSettings", this, TE_TR("Map Settings"));

  ++id;
  m_dataTypeScaleSettings = createEnum(id, "ScaleSettings", this, TE_TR("Scale Settings"));

  ++id;
  m_dataTypeNorthSettings = createEnum(id, "NorthSettings", this, TE_TR("North Settings"));

  ++id;
  m_dataTypePageSetup = createEnum(id, "PageSetup", this, TE_TR("Page Setup"));

  ++id;
  m_dataTypePath = createEnum(id, "PathString", this, TE_TR("Path"));

  ++id;
  m_dataTypeMapLocationSettings = createEnum(id, "MapLocationSettings", this, TE_TR("Map Location Settings"));
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeNone() const
{
  return m_dataTypeNone;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeInt() const
{
  return m_dataTypeInt;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeDouble() const
{
  return m_dataTypeDouble;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeLong() const
{
  return m_dataTypeLong;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeFloat() const
{
  return m_dataTypeFloat;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeString() const
{
  return m_dataTypeString;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeBool() const
{
  return m_dataTypeBool;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeColor() const
{
  return m_dataTypeColor;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeGridSettings() const
{
  return m_dataTypeGridSettings;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeStringList() const
{
  return m_dataTypeStringList;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeFont() const
{
  return m_dataTypeFont;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeEnvelope() const
{
  return m_dataTypeEnvelope;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeImage() const
{
  return m_dataTypeImage;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeTextGridSettings() const
{
  return m_dataTypeTextGridSettings;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeGroup() const
{
  return m_dataTypeGroup;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeMapChoice() const
{
  return m_dataTypeMapChoice;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeLegendChoice() const
{
  return m_dataTypeLegendChoice;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeLayerList() const
{
  return m_dataTypeLayerList;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeStringVector() const
{
  return m_dataTypeStringVector;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeStringMatrix() const
{
  return m_dataTypeStringMatrix;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeSVGView() const
{
  return m_dataTypeSVGView;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeGeometry() const
{
  return m_dataTypeGeometry;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeItemObserver() const
{
  return m_dataTypeItemObserver;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeGroupProperties() const
{
  return m_dataTypeGroupProperties;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeMapSettings() const
{
  return m_dataTypeMapSettings;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeScaleSettings() const
{
  return m_dataTypeScaleSettings;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeNorthSettings() const
{
  return m_dataTypeNorthSettings;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypePageSetup() const
{
  return m_dataTypePageSetup;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypePath() const
{
  return m_dataTypePath;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeMapLocationSettings() const
{
  return m_dataTypeMapLocationSettings;
}

