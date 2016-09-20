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
  \file DialogPropertiesBrowser.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "DialogPropertiesBrowser.h"
#include "../../../core/property/Properties.h"
#include "../../../core/enum/Enums.h"
#include "../../../core/Font.h"
#include "../../../core/pattern/singleton/Context.h"
#include "../../outside/GridSettingsOutside.h"
#include "../../../outside/GridSettingsModel.h"
#include "../../outside/MapLayerChoiceOutside.h"
#include "../../../outside/MapLayerChoiceModel.h"
#include "../../outside/LegendChoiceOutside.h"
#include "../../../outside/LegendChoiceModel.h"
#include "../../outside/NorthSettingsOutside.h"
#include "../../../outside/NorthSettingsModel.h"
#include "../../../core/pattern/proxy/AbstractProxyProject.h"
#include "../../../item/MapModel.h"
#include "../../../outside/ColorDialogModel.h"
#include "../../../outside/SVGDialogModel.h"
#include "../../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../item/MapItem.h"
#include "../../outside/ColorDialogOutside.h"
#include "../../outside/SVGDialogOutside.h"
#include "../ItemUtils.h"
#include "../BuildGraphicsOutside.h"
#include "../../item/GridMapItem.h"
#include "../../item/NorthItem.h"
#include "../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../core/pattern/mvc/AbstractItemController.h"
#include "../Scene.h"
#include "../../../core/pattern/proxy/AbstractProxyProject.h"
#include "../../outside/MapSettingsOutside.h"
#include "../../outside/MapLocationSettingsOutside.h"
#include "../../../outside/MapSettingsModel.h"
#include "../../outside/ScaleSettingsOutside.h"
#include "../../outside/ScaleSettingsController.h"
#include "../../../outside/MapLocationSettingsModel.h"

// STL
#include <vector>

// Qt
#include <QVariant>
#include <QFont>
#include <QColor>
#include <QImageReader>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QApplication>
#include <QByteArray>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtStringPropertyManager>
#include <QtPropertyBrowser/qteditorfactory.h>
#include <QtPropertyBrowser/QtProperty>

te::layout::DialogPropertiesBrowser::DialogPropertiesBrowser(Scene* scene, AbstractProxyProject* proxyProject, QObject* parent) :
  AbstractPropertiesBrowser(scene, parent),
  m_strDlgManager(0),
  m_dlgEditorFactory(0),
  m_proxyProject(proxyProject)
{
  if (m_dlgEditorFactory)
  {
    connect(m_dlgEditorFactory, SIGNAL(internalDlg(QWidget *, QtProperty *)), this, SLOT(onSetDlg(QWidget *, QtProperty *)));
  }

  createManager();
}

te::layout::DialogPropertiesBrowser::~DialogPropertiesBrowser()
{
  closeAllWindows();

  if(m_strDlgManager)
  {
    delete m_strDlgManager;
    m_strDlgManager = 0;
  }

  if(m_dlgEditorFactory)
  {
    delete m_dlgEditorFactory;
    m_dlgEditorFactory = 0;
  }
}

void te::layout::DialogPropertiesBrowser::createManager()
{
  m_strDlgManager = new QtStringPropertyManager;
  
  m_dlgEditorFactory = new QtDlgEditorFactory;

  // internalDlg is called when an item of property browser tree is clicked
  connect(m_dlgEditorFactory, SIGNAL(internalDlg(QWidget *, QtProperty *)), this, SLOT(onSetDlg(QWidget *, QtProperty *)));
}

void te::layout::DialogPropertiesBrowser::onSetDlg( QWidget *parent, QtProperty * prop )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType || !parent)
  {
    return;
  }

  QMap<QtProperty*, Property>::const_iterator itType = m_mapProperty.find(prop);
  if (itType == m_mapProperty.end())
  {
	  return;
  }

  m_currentPropertyClicked = itType.value();

  te::layout::EnumType* currentType = m_currentPropertyClicked.getType();

  if(currentType == dataType->getDataTypeNone())
    return;

  if(currentType == dataType->getDataTypeGridSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowGridSettingsDlg()));
  }
  if(currentType == dataType->getDataTypeImage())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowImageDlg()));
  }
  if(currentType == dataType->getDataTypeTextGridSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowTextGridSettingsDlg()));
  }
  if(currentType == dataType->getDataTypeMapChoice())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowMapLayerChoiceDlg()));
  }
  if(currentType == dataType->getDataTypeLegendChoice())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowLegendChoiceDlg()));
  }
  if(currentType == dataType->getDataTypeSVGView())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowViewDlg()));
  }
  if(currentType == dataType->getDataTypeColor())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowColorDlg()));
  }
  if(currentType == dataType->getDataTypeFont())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowFontDlg()));
  }
  if (currentType == dataType->getDataTypeMapSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowMapSettingsDlg()));
  }
  if (currentType == dataType->getDataTypeScaleSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowScaleSettingsDlg()));
  }
  if (currentType == dataType->getDataTypeNorthSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowNorthSettingsDlg()));
  }
  if (currentType == dataType->getDataTypePath())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowFolderDlg()));
  }
  if (currentType == dataType->getDataTypeMapLocationSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowMapLocationSettingsDlg()));
  }
}

QtStringPropertyManager* te::layout::DialogPropertiesBrowser::getStringPropertyManager()
{
  return m_strDlgManager;
}

QtDlgEditorFactory* te::layout::DialogPropertiesBrowser::getDlgEditorFactory()
{
  return m_dlgEditorFactory;
}

QtProperty* te::layout::DialogPropertiesBrowser::addProperty( const Property& property )
{
  QtProperty* qproperty = 0;

  if(!property.isVisible())
  {
    return qproperty;
  }

  if(!m_strDlgManager)
  {
    return qproperty;
  }

  std::string stdLabel = property.getLabel();
  if (stdLabel.compare("") == 0)
    stdLabel = property.getName();

  QString label = ItemUtils::convert2QString(stdLabel);

  std::string stdValue = property.getValue().toString();
  QString value = ItemUtils::convert2QString(stdValue);

  std::string name = property.getName();
  QString qName = ItemUtils::convert2QString(name);
  
  qproperty = m_strDlgManager->addProperty(label);
  m_strDlgManager->setValue(qproperty, value);
  addPropertyItem(qproperty, qName, label, property);
  /*The sub properties should not appear in this case, 
    because will be previewed in the dialog window will be opened.*/

  return qproperty;
}

void te::layout::DialogPropertiesBrowser::changeValueQtPropertyDlg(const QString& label, const QVariant& variant)
{
  if(!m_strDlgManager)
  {
    return;
  }

  QSet<QtProperty*> list = m_strDlgManager->properties();
  foreach( QtProperty* prop, list) 
  {
    if(prop->propertyName().compare(label) == 0)
    {
      m_strDlgManager->setValue(prop, variant.toString());
    }
  }
}

void te::layout::DialogPropertiesBrowser::onShowGridSettingsDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if(!enumObj)
  {
    return;
  }
  
  QWidget* widget = createOutside(enumObj->getGridSettings());
  if(!widget)
  {
    return;
  }

  GridSettingsOutside* gridSettings = dynamic_cast<GridSettingsOutside*>(widget);
  if(!gridSettings)
  {
    return;
  }

  appendDialog(gridSettings);

  AbstractOutsideController* abstractController = const_cast<AbstractOutsideController*>(gridSettings->getController());
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(abstractController->getModel());
  GridSettingsModel* model = dynamic_cast<GridSettingsModel*>(abstractModel);
  if(!model)
  {
    return;
  }
    
  gridSettings->load();
  gridSettings->show(); // modeless dialog
  gridSettings->raise(); // top of the parent widget's stack
}

void te::layout::DialogPropertiesBrowser::onShowNorthSettingsDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if (!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getNorthSettings());
  if (!widget)
  {
    return;
  }

  NorthSettingsOutside* northSettings = dynamic_cast<NorthSettingsOutside*>(widget);
  if (!northSettings)
  {
    return;
  }

  appendDialog(northSettings);

  northSettings->load();
  northSettings->show(); // modeless dialog
  northSettings->raise(); // top of the parent widget's stack
}

void te::layout::DialogPropertiesBrowser::onShowFolderDlg()
{

  QString dir = QFileDialog::getExistingDirectory(0, tr("Open Directory"),
    "/home",
    QFileDialog::ShowDirsOnly
    | QFileDialog::DontResolveSymlinks);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  std::string svgDir = ItemUtils::convert2StdString(dir);

  Property prop = m_currentPropertyClicked;
  prop.setValue(svgDir, dataType->getDataTypePath());

  emit changeDlgProperty(prop);

}

void te::layout::DialogPropertiesBrowser::onShowImageDlg()
{
  // Bulding the filter string
  QString filter = tr("Images") + " ( ";
  QList<QByteArray> formats = QImageReader::supportedImageFormats();
  for(int i = 0; i < formats.size() - 1; ++i)
    filter += "*." + formats[i] + " ";
  filter += ")";

  QDesktopWidget* wg = QApplication::desktop();
  QFileDialog dialog;

  //Put the dialog in the screen center
  QRect screen = wg->screenGeometry();
  dialog.move( screen.center() - dialog.rect().center() );

  QString path = dialog.getOpenFileName(wg, tr("Select an Image File"), "", filter);
  if(path.isNull())
    return;

  // Try load image
  QImage img;
  if(!img.load(path))
  {
    QMessageBox::critical(wg, tr("Error"), tr("The selected image cannot be loaded."));
    return;
  }
  else
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    std::string stdPath = ItemUtils::convert2StdString(path);

    Property prop = m_currentPropertyClicked;
    prop.setValue(stdPath, dataType->getDataTypeImage());

    std::string label = prop.getValue().toString();
    QString qLabel = ItemUtils::convert2QString(label);

    QVariant v(path);
    changeValueQtPropertyDlg(qLabel, v);

    emit changeDlgProperty(prop);
  }
}

void te::layout::DialogPropertiesBrowser::onShowTextGridSettingsDlg()
{
  
}

void te::layout::DialogPropertiesBrowser::onShowFontDlg()
{
  QWidget* wdg = dynamic_cast<QWidget*>(parent());

  if(!wdg)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property property = m_currentPropertyClicked;

  if(property.getType() != dataType->getDataTypeFont())
    return;

  bool ok = false;
  Font font;
  QFont qFont;

  std::string fontName = font.getFamily();
  QString qFontName = ItemUtils::convert2QString(fontName);

  font = property.getValue().toFont();
  qFont.setFamily(qFontName);
  qFont.setPointSize(font.getPointSize());
  qFont.setBold(font.isBold());
  qFont.setItalic(font.isItalic());
  qFont.setUnderline(font.isUnderline());
  qFont.setStrikeOut(font.isStrikeout());
  qFont.setKerning(font.isKerning());

  QFontDialog dialog;
  
  //Put the dialog in the screen center
  QRect screen = wdg->geometry();
  dialog.move( screen.center() - dialog.rect().center() );

  QFont newFont = dialog.getFont(&ok, qFont, wdg, tr("Select Font"));

  if (!ok || newFont == qFont) 
    return;

  QString qNewFontName = newFont.family();
  std::string newFontName = ItemUtils::convert2StdString(qNewFontName);

  font.setFamily(newFontName);
  font.setPointSize(newFont.pointSize());
  font.setBold(newFont.bold());
  font.setItalic(newFont.italic());
  font.setUnderline(newFont.underline());
  font.setStrikeout(newFont.strikeOut());
  font.setKerning(newFont.kerning());
  property.setValue(font, dataType->getDataTypeFont());

  emit changeDlgProperty(property);
}

void te::layout::DialogPropertiesBrowser::onShowColorDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if(!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getColorDialog());
  if(!widget)
  {
    return;
  }

  ColorDialogOutside* colorDialog = dynamic_cast<ColorDialogOutside*>(widget);
  if(!colorDialog)
  {
    return;
  }

  appendDialog(colorDialog);

  AbstractOutsideController* abstractController = const_cast<AbstractOutsideController*>(colorDialog->getController());
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(abstractController->getModel());
  ColorDialogModel* model = dynamic_cast<ColorDialogModel*>(abstractModel);
  if(!model)
  {
    return;
  }
    
  model->setColorProperty(m_currentPropertyClicked);

  colorDialog->init();
  colorDialog->show(); // modeless dialog
  colorDialog->raise(); // top of the parent widget's stack
  colorDialog->activateWindow(); // visible top-level window that has the keyboard input focus
}

void te::layout::DialogPropertiesBrowser::onShowMapLayerChoiceDlg()
{

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if (!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getMapLayerChoice());
  if (!widget)
  {
    return;
  }

  MapLayerChoiceOutside* layerChoice = dynamic_cast<MapLayerChoiceOutside*>(widget);
  if (!layerChoice)
  {
    return;
  }

  appendDialog(layerChoice);

  AbstractOutsideController* abstractController = const_cast<AbstractOutsideController*>(layerChoice->getController());
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(abstractController->getModel());
  MapLayerChoiceModel* model = dynamic_cast<MapLayerChoiceModel*>(abstractModel);
  if (!model)
  {
    return;
  }

  if (!m_proxyProject)
  {
    return;
  }

  std::list<te::map::AbstractLayerPtr> listLayers = m_proxyProject->getAllLayers();
  model->setLayers(listLayers);

  std::vector<te::layout::Properties> properties;
  properties.push_back(m_allProperties);
  model->setPropertiesMaps(properties);

  layerChoice->init();
  layerChoice->show();
}

void te::layout::DialogPropertiesBrowser::onShowLegendChoiceDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if(!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getLegendChoice());
  if(!widget)
  {
    return;
  }

  LegendChoiceOutside* legendChoice = dynamic_cast<LegendChoiceOutside*>(widget);
  if(!legendChoice)
  {
    return;
  }

  appendDialog(legendChoice);

  AbstractOutsideController* abstractController = const_cast<AbstractOutsideController*>(legendChoice->getController());
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(abstractController->getModel());
  LegendChoiceModel* model = dynamic_cast<LegendChoiceModel*>(abstractModel);
  if(!model)
  {
    return;
  }
  
  if (!m_proxyProject)
  {
    return;
  }

  std::list<te::map::AbstractLayerPtr> listLayers = m_proxyProject->getAllLayers();
  model->setLayers(listLayers);

  std::vector<te::layout::Properties> properties;
  properties.push_back(m_allProperties);
  model->setPropertiesLegends(properties);

  legendChoice->init();
  legendChoice->show();
}

void te::layout::DialogPropertiesBrowser::onShowViewDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if(!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getSVGDialog());
  if(!widget)
  {
    return;
  }

  SVGDialogOutside* svgOutside = dynamic_cast<SVGDialogOutside*>(widget);
  if(!svgOutside)
  {
    return;
  }

  appendDialog(svgOutside);

  AbstractOutsideController* abstractController = const_cast<AbstractOutsideController*>(svgOutside->getController());
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(abstractController->getModel());
  SVGDialogModel* model = dynamic_cast<SVGDialogModel*>(abstractModel);
  if(!model)
  {
    return;
  }

  if(!m_proxyProject)
  {
    return;
  }

  model->setPathsProperty(m_currentPropertyClicked);
  
  svgOutside->init();
  svgOutside->show();
}

void te::layout::DialogPropertiesBrowser::onShowMapLocationSettingsDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if (!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getMapLocationSettingsDialog());
  if (!widget)
  {
    return;
  }

  MapLocationSettingsOutside* mapLocationSettings = dynamic_cast<MapLocationSettingsOutside*>(widget);
  if (!mapLocationSettings)
  {
    return;
  }

  appendDialog(mapLocationSettings);

  AbstractOutsideController* abstractController = const_cast<AbstractOutsideController*>(mapLocationSettings->getController());
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(abstractController->getModel());

  MapLocationSettingsModel* model = dynamic_cast<MapLocationSettingsModel*>(abstractModel);
  if (!model)
  {
    return;
  }
  
  mapLocationSettings->load();
  mapLocationSettings->show(); // modeless dialog
  mapLocationSettings->raise(); // top of the parent widget's stack
  
  
}


void te::layout::DialogPropertiesBrowser::onShowMapSettingsDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if (!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getMapSettingsDialog());
  if (!widget)
  {
    return;
  }

  MapSettingsOutside* mapSettings = dynamic_cast<MapSettingsOutside*>(widget);
  if (!mapSettings)
  {
    return;
  }

  appendDialog(mapSettings);

  AbstractOutsideController* abstractController = const_cast<AbstractOutsideController*>(mapSettings->getController());
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(abstractController->getModel());

  MapSettingsModel* model = dynamic_cast<MapSettingsModel*>(abstractModel);
  if (!model)
  {
    return;
  }

  mapSettings->load();
  mapSettings->show(); // modeless dialog
  mapSettings->raise(); // top of the parent widget's stack
}



void te::layout::DialogPropertiesBrowser::onShowScaleSettingsDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if (!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getScaleSettingsDialog());
  if (!widget)
  {
    return;
  }

  ScaleSettingsOutside* scaleSettings = dynamic_cast<ScaleSettingsOutside*>(widget);
  if (!scaleSettings)
  {
    return;
  }

  appendDialog(scaleSettings);

  AbstractOutsideController* abstractController = const_cast<AbstractOutsideController*>(scaleSettings->getController());
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(abstractController);
  if (!controller)
  {
    return;
  }
  
  scaleSettings->load();
  scaleSettings->show(); // modeless dialog
  scaleSettings->raise(); // top of the parent widget's stack
}

te::layout::Property te::layout::DialogPropertiesBrowser::getProperty(QtProperty* qtProperty)
{
  Property prop;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
  {
    return prop;
  }

  QMap<QtProperty*, Property>::const_iterator itProperty = m_mapProperty.find(qtProperty);
  if (itProperty == m_mapProperty.end())
  {
    return prop;
  }

  prop = itProperty.value();
  
  QVariant variant = getPropertyValue(qtProperty);

  if(variant.isNull() || !variant.isValid())
  {
    return prop;
  }

  QString qValue = variant.toString();
  std::string valueString = ItemUtils::convert2StdString(qValue);

  if(prop.getType() == dataType->getDataTypeGridSettings())
  {
    prop.setValue(valueString, prop.getType());
  }
  else if(prop.getType() == dataType->getDataTypeImage())
  {
    prop.setValue(valueString, prop.getType());
  }
  else if (prop.getType() == dataType->getDataTypeNorthSettings())
  {
    prop.setValue(variant.toString().toStdString(), prop.getType());
  }

  return prop;
}

bool te::layout::DialogPropertiesBrowser::updateProperty( const Property& property )
{
  QtProperty* qprop = findProperty(property.getName(), property.getName());
  if(!qprop)
  {
    return false;
  }

  return true;
}

void te::layout::DialogPropertiesBrowser::closeAllWindows()
{
  if(m_dialogs.empty())
    return;

  foreach(QWidget* widget, m_dialogs)
  {
    if(m_dialogs.removeAll(widget))
    {
      widget->close();
      delete widget;
      widget = 0;
    }
  }
}

QWidget* te::layout::DialogPropertiesBrowser::createOutside( EnumType* enumType )
{
  QWidget* widget = 0;

  if(!enumType)
  {
    return widget;
  }
  
  BuildGraphicsOutside build;
  widget = build.createOutside(enumType, m_scene, (QWidget*) this->parent(), m_proxyProject);
  return widget;
}

void te::layout::DialogPropertiesBrowser::updateOutside( const Property& prop )
{
  emit changeDlgProperty(prop);
}

void te::layout::DialogPropertiesBrowser::onDestroyed( QObject* obj )
{
  if(m_dialogs.isEmpty())
    return;

  foreach(QWidget* widget, m_dialogs)
  {
    if(widget == obj)
    {
      m_dialogs.removeAll(widget);
    }
  }
}

void te::layout::DialogPropertiesBrowser::clearAll()
{
  AbstractPropertiesBrowser::clearAll();
  closeAllWindows();
}

void te::layout::DialogPropertiesBrowser::appendDialog( QWidget* widget )
{
  m_dialogs.append(widget);

  connect(widget, SIGNAL(updateProperty(Property)), this, SLOT(updateOutside(Property)));
  connect(widget, SIGNAL(destroyed( QObject *)), this, SLOT(onDestroyed(QObject*)));

  widget->setAttribute(Qt::WA_DeleteOnClose);
}

bool te::layout::DialogPropertiesBrowser::hasOpenWindows()
{
  bool result = false;
  if (!m_dialogs.isEmpty())
  {
    result = true;
  }
  return result;
}

void te::layout::DialogPropertiesBrowser::directlyShowWindow(Property prop)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if (!dataType)
  {
    return;
  }

  std::string name = prop.getName();
  if (!m_allProperties.contains(name))
  {
    return;
  }
  
  m_currentPropertyClicked = prop;

  if (prop.getType() == dataType->getDataTypeNone())
    return;

  if (prop.getType() == dataType->getDataTypeGridSettings())
  {
    onShowGridSettingsDlg();
  }
  if (prop.getType() == dataType->getDataTypeImage())
  {
    onShowImageDlg();
  }
  if (prop.getType() == dataType->getDataTypeTextGridSettings())
  {
    onShowTextGridSettingsDlg();
  }
  if (prop.getType() == dataType->getDataTypeMapChoice())
  {
    onShowMapLayerChoiceDlg();
  }
  if (prop.getType() == dataType->getDataTypeLegendChoice())
  {
    onShowLegendChoiceDlg();
  }
  if (prop.getType() == dataType->getDataTypeSVGView())
  {
    onShowViewDlg();
  }
  if (prop.getType() == dataType->getDataTypeColor())
  {
    onShowColorDlg();
  }
  if (prop.getType() == dataType->getDataTypeFont())
  {
    onShowFontDlg();
  }
  if (prop.getType() == dataType->getDataTypeMapSettings())
  {
    onShowMapSettingsDlg();
  }
  if (prop.getType() == dataType->getDataTypeScaleSettings())
  {
    onShowScaleSettingsDlg();
  }
  if (prop.getType() == dataType->getDataTypeNorthSettings())
  {
    onShowNorthSettingsDlg();
  }

 if (prop.getType() == dataType->getDataTypeMapLocationSettings())
  {
    onShowMapLocationSettingsDlg();
  }

}
