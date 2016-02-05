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
#include "../../../outside/MapSettingsModel.h"
#include "../../outside/ScaleSettingsOutside.h"
#include "../../outside/ScaleSettingsController.h"

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

  m_dlgProps.clear();

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

  QString name = prop->propertyName();

  Property propt = findDlgProperty(name);

  m_currentPropertyClicked = propt;

  if(propt.getType() == dataType->getDataTypeNone())
    return;

  if(propt.getType() == dataType->getDataTypeGridSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowGridSettingsDlg()));
  }
  if(propt.getType() == dataType->getDataTypeImage())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowImageDlg()));
  }
  if(propt.getType() == dataType->getDataTypeTextGridSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowTextGridSettingsDlg()));
  }
  if(propt.getType() == dataType->getDataTypeMapChoice())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowMapLayerChoiceDlg()));
  }
  if(propt.getType() == dataType->getDataTypeLegendChoice())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowLegendChoiceDlg()));
  }
  if(propt.getType() == dataType->getDataTypeSVGView())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowViewDlg()));
  }
  if(propt.getType() == dataType->getDataTypeColor())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowColorDlg()));
  }
  if(propt.getType() == dataType->getDataTypeFont())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowFontDlg()));
  }
  if (propt.getType() == dataType->getDataTypeMapSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowMapSettingsDlg()));
  }
  if (propt.getType() == dataType->getDataTypeScaleSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowScaleSettingsDlg()));
  }
  if (propt.getType() == dataType->getDataTypeNorthSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowNorthSettingsDlg()));
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

bool te::layout::DialogPropertiesBrowser::changeQtPropertyValue(QtProperty* pproperty, const Property& property)
{
  m_changeProperty = true;

  if(!pproperty)
  {
    return false;
  }
    
  m_changeProperty = false;

  return true;
}

QtProperty* te::layout::DialogPropertiesBrowser::addProperty( const Property& property )
{
  QtProperty* qproperty = 0;

  if(!property.isVisible())
  {
    return qproperty;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  if(!dataType)
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

  stdLabel = property.getValue().toString();
  QString value = ItemUtils::convert2QString(stdLabel);

  std::string name = property.getName();
  QString qName = ItemUtils::convert2QString(name);
  
  qproperty = m_strDlgManager->addProperty(label);
  m_strDlgManager->setValue(qproperty, value);
  addPropertyItem(qproperty, qName, label);
  /*The sub properties should not appear in this case, 
    because will be previewed in the dialog window will be opened.*/
  m_dlgProps.insert(std::pair<std::string, Property>(property.getName(),property));

  return qproperty;
}

bool te::layout::DialogPropertiesBrowser::checkDlgType( const Property& prop )
{
  bool result = false;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(prop.getType() == dataType->getDataTypeGridSettings()
    || prop.getType() == dataType->getDataTypeImage()
    || prop.getType() == dataType->getDataTypeTextGridSettings()
    || prop.getType() == dataType->getDataTypeNorthSettings())
  {
    result = true;
  }

  return result;
}

te::layout::Property te::layout::DialogPropertiesBrowser::findDlgProperty(const QString& label)
{
  Property prop;

  QString name = nameProperty(label);
  if (name.compare("") == 0)
  {
    return prop;
  }

  std::string propName;
  std::map<std::string, Property>::iterator it;

  for (it = m_dlgProps.begin(); it != m_dlgProps.end(); ++it) {
    propName = it->first;
    QString qName = ItemUtils::convert2QString(propName);
    if (name.compare(qName) == 0)
    {
      prop = it->second;
      break;
    }
  }

  return prop;
}

te::layout::Property te::layout::DialogPropertiesBrowser::findDlgProperty( EnumType* dataType )
{
  Property prop;

  Property pro;
  std::map<std::string, Property>::iterator it;

  for (it = m_dlgProps.begin(); it != m_dlgProps.end(); ++it) {
    pro = it->second;
    if(pro.getType() == dataType)
    {
      prop = pro;
      break;
    }
  }

  return prop;
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

  model->setGridProperties(m_allProperties);
  
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

  std::vector<te::layout::Properties> properties;
  properties.push_back(m_allProperties);
  model->setPropertiesMaps(properties);

  QWidget* widgetLayerChoice = createOutside(enumObj->getMapLayerChoice());
  if (!widget)
  {
    return;
  }
  
  MapLayerChoiceOutside* layerChoice = dynamic_cast<MapLayerChoiceOutside*>(widgetLayerChoice);
  if (!layerChoice)
  {
    return;
  }

  AbstractOutsideController* abstractControllerLayer = const_cast<AbstractOutsideController*>(layerChoice->getController());
  AbstractOutsideModel* abstractModelLayer = const_cast<AbstractOutsideModel*>(abstractControllerLayer->getModel());
  MapLayerChoiceModel* modelLayerChoice = dynamic_cast<MapLayerChoiceModel*>(abstractModelLayer);
  if (!modelLayerChoice)
  {
    return;
  }

  if (!m_proxyProject)
  {
    return;
  }

  std::list<te::map::AbstractLayerPtr> listLayers = m_proxyProject->getAllLayers();
  modelLayerChoice->setLayers(listLayers);

  std::vector<te::layout::Properties> propertiesLayerChoice;
  propertiesLayerChoice.push_back(m_allProperties);
  modelLayerChoice->setPropertiesMaps(propertiesLayerChoice);
  

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

te::layout::Property te::layout::DialogPropertiesBrowser::getProperty(const QString& label)
{
  Property prop;

  QString name = nameProperty(label);
  if (name.compare("") == 0)
  {
    return prop;
  }

  std::string stdName = ItemUtils::convert2StdString(name);
  prop.setName(stdName);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
  {
    return prop;
  }
  
  QVariant variant = findPropertyValue(label);

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

te::layout::EnumType* te::layout::DialogPropertiesBrowser::getLayoutType(QVariant::Type type, const QString& label /*= ""*/)
{
  Property prop;
  EnumDataType* dtType = Enums::getInstance().getEnumDataType();
  EnumType* dataType = 0;

  if(!dtType)
  {
    return 0;
  }
  
  switch(type)
  {
  case QVariant::String:
    {
      dataType = dtType->getDataTypeString();

      //Custom types: Dialog Window Type
      if(label.compare("") != 0)
      {
        prop = findDlgProperty(label);
        if(!prop.getValue().isNull())
        {
          if(prop.getType() == dtType->getDataTypeGridSettings())
          {
            dataType = dtType->getDataTypeGridSettings();
          }
          if(prop.getType() == dtType->getDataTypeImage())
          {
            dataType = dtType->getDataTypeImage();
          }
          if (prop.getType() == dtType->getDataTypeNorthSettings())
          {
            dataType = dtType->getDataTypeNorthSettings();
          }
        }
      }
    }
    break;
  default:
    prop.setValue(0, dtType->getDataTypeNone());
  }

  return dataType;
}

int te::layout::DialogPropertiesBrowser::getVariantType( EnumType* dataType )
{
  int type = QVariant::Invalid;

  EnumDataType* dtType = Enums::getInstance().getEnumDataType();

  if(!dtType)
  {
    return type;
  }
  
  if(dataType == dtType->getDataTypeGridSettings())
  {
    type = QVariant::String;
  }
  else if(dataType == dtType->getDataTypeImage())
  {
    type = QVariant::String;
  }
  else if (dataType == dtType->getDataTypeNorthSettings())
  {
    type = QVariant::String;
  }
  else
  {
    type = QVariant::Invalid;
  }

  return type;
}

bool te::layout::DialogPropertiesBrowser::updateProperty( const Property& property )
{
  std::string stdLabel = property.getLabel();
  if (stdLabel.compare("") == 0)
    stdLabel = property.getName();

  QString label = ItemUtils::convert2QString(stdLabel);

  QtProperty* qprop = findProperty(label);
  if(!qprop)
  {
    return false;
  }  

  return changeQtPropertyValue(qprop, property);
}

std::map<std::string, te::layout::Property> te::layout::DialogPropertiesBrowser::getDlgProps()
{
  return m_dlgProps;
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

void te::layout::DialogPropertiesBrowser::updateOutside( const std::vector<Property>& props )
{
  emit changeDlgProperty(props);
}

void te::layout::DialogPropertiesBrowser::onDestroyed( QObject* obj )
{
  if(m_dialogs.empty())
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

