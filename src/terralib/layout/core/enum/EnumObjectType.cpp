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
  \file EnumObjectType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumObjectType.h"

#include <terralib/core/translator/Translator.h>

te::layout::EnumObjectType::EnumObjectType() :
  m_rectangleItem(0),
  m_mapItem(0),
  m_paperItem(0),
  m_legendItem(0),
  m_propertiesWindow(0),
  m_displayWindow(0),
  m_objectInspectorWindow(0),
  m_toolbar(0),
  m_gridSettings(0),
  m_northSettings(0),
  m_itemGroup(0),
  m_scaleItem(0),
  m_pageSetup(0),
  m_imageItem(0),
  m_systematicScale(0),
  m_pointItem(0),
  m_arrowItem(0),
  m_ellipseItem(0),
  m_titleItem(0),
  m_textGridItem(0),
  m_editTemplate(0),
  m_textGridSettings(0),
  m_legendChildItem(0),
  m_lineItem(0),
  m_polygonItem(0),
  m_balloonItem(0),
  m_barCodeItem(0),
  m_gridMapItem(0),
  m_mapLayerChoice(0),
  m_svgDialog(0),
  m_northItem(0),
  m_mapLocationItem(0),
  m_objectUnknown(0),
  m_colorDialog(0),
  m_fontDialog(0),
  m_legendChoice(0),
  m_starItem(0),
  m_svgItem(0),
  m_exportSettingsDialog(0),
  m_threeNorthItem(0),
  m_mapSettingsDialog(0),
  m_scaleSettingsDialog(0),
  m_printSettingsDialog(0)
{
  init();
}

te::layout::EnumObjectType::~EnumObjectType()
{
  if(m_objectUnknown)
  {
    delete m_objectUnknown;
    m_objectUnknown = 0;
  }

  if(m_rectangleItem)
  {
    delete m_rectangleItem;
    m_rectangleItem = 0;
  }
  
  if(m_paperItem)
  {
    delete m_paperItem;
    m_paperItem = 0;
  }

  if(m_legendItem)
  {
    delete m_legendItem;
    m_legendItem = 0;
  }

  if(m_propertiesWindow)
  {
    delete m_propertiesWindow;
    m_propertiesWindow = 0;
  }

  if(m_displayWindow)
  {
    delete m_displayWindow;
    m_displayWindow = 0;
  }

  if(m_objectInspectorWindow)
  {
    delete m_objectInspectorWindow;
    m_objectInspectorWindow = 0;
  }

  if(m_toolbar)
  {
    delete m_toolbar;
    m_toolbar = 0;
  }

  if(m_gridSettings)
  {
    delete m_gridSettings;
    m_gridSettings = 0;
  }

  if (m_northSettings)
  {
    delete m_northSettings;
    m_northSettings = 0;
  }

  if(m_itemGroup)
  {
    delete m_itemGroup;
    m_itemGroup = 0;
  }

  if(m_scaleItem)
  {
    delete m_scaleItem;
    m_scaleItem = 0;
  }

  if(m_pageSetup)
  {
    delete m_pageSetup;
    m_pageSetup = 0;
  }

  if(m_imageItem)
  {
    delete m_imageItem;
    m_imageItem = 0;
  }

  if(m_systematicScale)
  {
    delete m_systematicScale;
    m_systematicScale = 0;
  }

  if(m_pointItem)
  {
    delete m_pointItem;
    m_pointItem = 0;
  }

  if(m_arrowItem)
  {
    delete m_arrowItem;
    m_arrowItem = 0;
  }

  if(m_ellipseItem)
  {
    delete m_ellipseItem;
    m_ellipseItem = 0;
  }

  if(m_titleItem)
  {
    delete m_titleItem;
    m_titleItem = 0;
  }

  if(m_textGridItem)
  {
    delete m_textGridItem;
    m_textGridItem = 0;
  }

  if(m_editTemplate)
  {
    delete m_editTemplate;
    m_editTemplate = 0;
  }

  if(m_textGridSettings)
  {
    delete m_textGridSettings;
    m_textGridSettings = 0;
  }

  if(m_objectUnknown)
  {
    delete m_objectUnknown;
    m_objectUnknown = 0;
  }

  if(m_legendChildItem)
  {
    delete m_legendChildItem;
    m_legendChildItem = 0;
  }

  if(m_textItem)
  {
    delete m_textItem;
    m_textItem = 0;
  }

  if(m_lineItem)
  {
    delete m_lineItem;
    m_lineItem = 0;
  }

  if(m_polygonItem)
  {
    delete m_polygonItem;
    m_polygonItem = 0;
  }

  if(m_balloonItem)
  {
    delete m_balloonItem;
    m_balloonItem = 0;
  }

  if(m_barCodeItem)
  {
    delete m_barCodeItem;
    m_barCodeItem = 0;
  }

  if(m_gridMapItem)
  {
    delete m_gridMapItem;
    m_gridMapItem = 0;
  }

  if(m_mapLayerChoice)
  {
    delete m_mapLayerChoice;
    m_mapLayerChoice = 0;
  }
  
  if(m_svgDialog)
  {
    delete m_svgDialog;
    m_svgDialog = 0;
  }

  if(m_colorDialog)
  {
    delete m_colorDialog;
    m_colorDialog = 0;
  }

  if(m_fontDialog)
  {
    delete m_fontDialog;
    m_fontDialog = 0;
  }  

  if(m_legendChoice)
  {
    delete m_legendChoice;
    m_legendChoice = 0;
  }  

  if(m_starItem)
  {
    delete m_starItem;
    m_starItem = 0;
  } 

  if(m_svgItem)
  {
    delete m_svgItem;
    m_svgItem = 0;
  } 

  if (m_exportSettingsDialog)
  {
    delete m_exportSettingsDialog;
    m_exportSettingsDialog = 0;
  }

  if(m_threeNorthItem)
  {
    delete m_threeNorthItem;
    m_threeNorthItem = 0;
  } 

  if (m_mapSettingsDialog)
  {
    delete m_mapSettingsDialog;
    m_mapSettingsDialog = 0;
  }

  if (m_scaleSettingsDialog)
  {
    delete m_scaleSettingsDialog;
    m_scaleSettingsDialog = 0;
  }

  if (m_printSettingsDialog)
  {
    delete m_printSettingsDialog;
    m_printSettingsDialog = 0;
  }
}

void te::layout::EnumObjectType::init()
{
  m_objectUnknown = createEnum("None", this, TE_TR("None"));

  m_rectangleItem = createEnum("Rectangle_Item", this, TE_TR("Rectangle Item"));

  m_mapItem = createEnum("Map_Item", this, TE_TR("Map Item"));
  
  m_paperItem = createEnum("Paper_Item", this, TE_TR("Paper Item"));

  m_legendItem = createEnum("Legend_Item", this, TE_TR("Legend Item"));

  m_propertiesWindow = createEnum("Properties_Window", this, TE_TR("Properties Window"));

  m_displayWindow = createEnum("Display_Window", this, TE_TR("Display Window"));

  m_objectInspectorWindow = createEnum("ObjectInspector_Window", this, TE_TR("Object Inspector Window"));

  m_toolbar = createEnum("Toolbar", this, TE_TR("Toolbar"));

  m_gridSettings  = createEnum("Grid_Settings", this, TE_TR("Grid Settings"));
  
  m_northSettings = createEnum("North_Settings", this, TE_TR("North Settings"));

  m_textItem = createEnum("Text_Item", this, TE_TR("Text Item"));

  m_itemGroup = createEnum("Item_Group", this, TE_TR("Item Group"));

  m_scaleItem = createEnum("Scale_Item", this, TE_TR("Scale Item"));

  m_pageSetup = createEnum("Page_Setup", this, TE_TR("Page Setup"));

  m_imageItem = createEnum("Image_Item", this, TE_TR("Image Item"));

  m_systematicScale = createEnum("Systematic_Scale", this, TE_TR("Systematic Scale"));

  m_pointItem = createEnum("Point_Item", this, TE_TR("Point Item"));

  m_arrowItem = createEnum("Arrow_Item", this, TE_TR("Arrow Item"));

  m_ellipseItem = createEnum("Ellipse_Item", this, TE_TR("Ellipse Item"));

  m_titleItem = createEnum("Title_Item", this, TE_TR("Title Item"));

  m_textGridItem = createEnum("TextGrid_Item", this, TE_TR("Text Grid Item"));

  m_editTemplate = createEnum("Edit_Template", this, TE_TR("Edit Template"));

  m_textGridSettings = createEnum("Text_Grid_Settings", this, TE_TR("Text Grid Settings"));

  m_legendChildItem = createEnum("Legend_Child_Item", this, TE_TR("Legend Child Item"));

  m_lineItem = createEnum("Line_Item", this, TE_TR("Line Item")); 

  m_polygonItem = createEnum("Polygon_Item", this, TE_TR("Polygon Item")); 

  m_balloonItem = createEnum("Balloon_Item", this, TE_TR("Balloon Item")); 

  m_barCodeItem = createEnum("BarCode_Item", this, TE_TR("BarCode Item"));

  m_gridMapItem = createEnum("GridMap_Item", this, TE_TR("Grid Map Item"));

  m_mapLayerChoice = createEnum("Map_Layout_Choice", this, TE_TR("Map Layout Choice"));

  m_svgDialog = createEnum("SVG_Dialog", this, TE_TR("SVG Dialog"));

  m_northItem = createEnum("North_Item", this, TE_TR("North Item"));

  m_mapLocationItem = createEnum("Map_Location_Item", this, TE_TR("Map Location Item"));

  m_colorDialog = createEnum("Color_Dialog", this, TE_TR("Color Dialog"));

  m_fontDialog = createEnum("Font_Dialog", this, TE_TR("Font Dialog"));

  m_legendChoice = createEnum("Legend_Choice", this, TE_TR("Legend Choice"));

  m_starItem = createEnum("Star_Item", this, TE_TR("Star Item"));

  m_svgItem = createEnum("SVG_Item", this, TE_TR("SVG Item"));

  m_exportSettingsDialog = createEnum("Export_Settings", this, TE_TR("Export Settings"));

  m_threeNorthItem = createEnum("Three_North_Item", this, TE_TR("Three North Item"));

  m_mapSettingsDialog = createEnum("Map_Settings", this, TE_TR("Map Settings"));

  m_scaleSettingsDialog = createEnum("Scale_Settings", this, TE_TR("Scale Settings"));

  m_printSettingsDialog = createEnum("Print_Settings", this, TE_TR("Print Settings"));

  m_mapLocationSettingsDialog = createEnum("Map_Location_Settings", this, TE_TR("Map Location Settings"));
}

te::layout::EnumType* te::layout::EnumObjectType::getRectangleItem() const
{
  return m_rectangleItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getLineItem() const 
{
  return m_lineItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getPolygonItem() const 
{
  return m_polygonItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getBalloonItem() const 
{
  return m_balloonItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getBarCodeItem() const
{
  return m_barCodeItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getGridMapItem() const
{
  return m_gridMapItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getMapItem() const
{
  return m_mapItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getPaperItem() const
{
  return m_paperItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getLegendItem() const
{
  return m_legendItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getPropertiesWindow() const
{
  return m_propertiesWindow;
}

te::layout::EnumType* te::layout::EnumObjectType::getDisplayWindow() const
{
  return m_displayWindow;
}

te::layout::EnumType* te::layout::EnumObjectType::getObjectInspectorWindow() const
{
  return m_objectInspectorWindow;
}

te::layout::EnumType* te::layout::EnumObjectType::getToolbar() const
{
  return m_toolbar;
}

te::layout::EnumType* te::layout::EnumObjectType::getGridSettings() const
{
  return m_gridSettings;
}

te::layout::EnumType* te::layout::EnumObjectType::getNorthSettings() const
{
  return m_northSettings;
}

te::layout::EnumType* te::layout::EnumObjectType::getItemGroup() const
{
  return m_itemGroup;
}

te::layout::EnumType* te::layout::EnumObjectType::getScaleItem() const
{
  return m_scaleItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getPageSetup() const
{
  return m_pageSetup;
}

te::layout::EnumType* te::layout::EnumObjectType::getImageItem() const
{
  return m_imageItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getSystematicScale() const
{
  return m_systematicScale;
}

te::layout::EnumType* te::layout::EnumObjectType::getPointItem() const
{
  return m_pointItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getArrowItem() const
{
  return m_arrowItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getEllipseItem() const
{
  return m_ellipseItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getTitleItem() const
{
  return m_titleItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getTextGridItem() const
{
  return m_textGridItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getEditTemplate() const
{
  return m_editTemplate;
}

te::layout::EnumType* te::layout::EnumObjectType::getTextGridSettings() const
{
  return m_textGridSettings;
}

te::layout::EnumType* te::layout::EnumObjectType::getLegendChildItem() const
{
  return m_legendChildItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getTextItem() const
{
  return m_textItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getObjectUnknown() const
{
  return m_objectUnknown;
}

te::layout::EnumType* te::layout::EnumObjectType::getMapLayerChoice() const
{
  return m_mapLayerChoice;
}

te::layout::EnumType* te::layout::EnumObjectType::getNorthItem() const
{
  return m_northItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getMapLocationItem() const
{
  return m_mapLocationItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getColorDialog() const
{
  return m_colorDialog;
}

te::layout::EnumType* te::layout::EnumObjectType::getFontDialog() const
{
  return m_fontDialog;
}

te::layout::EnumType* te::layout::EnumObjectType::getLegendChoice() const
{
  return m_legendChoice;
}

te::layout::EnumType* te::layout::EnumObjectType::getStarItem() const
{
  return m_starItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getSVGItem() const
{
  return m_svgItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getSVGDialog() const
{
  return m_svgDialog;
}

te::layout::EnumType* te::layout::EnumObjectType::getExportSettingsDialog() const
{
  return m_exportSettingsDialog;
}

te::layout::EnumType* te::layout::EnumObjectType::getThreeNorthItem() const
{
  return m_threeNorthItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getMapSettingsDialog() const
{
  return m_mapSettingsDialog;
}

te::layout::EnumType* te::layout::EnumObjectType::getScaleSettingsDialog() const
{
  return m_scaleSettingsDialog;
}

te::layout::EnumType* te::layout::EnumObjectType::getPrintSettingsDialog() const
{
  return m_printSettingsDialog;
}


te::layout::EnumType* te::layout::EnumObjectType::getMapLocationSettingsDialog() const
{
  return m_mapLocationSettingsDialog;
}

