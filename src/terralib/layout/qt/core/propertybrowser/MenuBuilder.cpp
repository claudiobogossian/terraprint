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
  \file MenuItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MenuBuilder.h"
#include "../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../outside/GridSettingsModel.h"
#include "../../outside/GridSettingsController.h"
#include "../../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../outside/GridSettingsOutside.h"
#include "../../../core/enum/Enums.h"
#include "../../../outside/TextGridSettingsModel.h"
#include "../../../outside/TextGridSettingsController.h"
#include "../../outside/TextGridSettingsOutside.h"
#include "../Scene.h"
#include "PropertiesUtils.h"
#include "../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../core/pattern/mvc/AbstractItemModel.h"
#include "../View.h"
#include "../ItemUtils.h"

//STL
#include <string>

// Qt
#include <QAction>
#include <QGraphicsItem>
#include <QImageReader>
#include <QString>
#include <QByteArray>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QMenu>
#include <QWidget>
#include <QPoint>
#include <QFontDialog>
#include <QFont>
#include <QColorDialog>
#include <QColor>
#include <QApplication>
#include <QRect>
#include <QDesktopWidget>

te::layout::MenuBuilder::MenuBuilder(Scene* scene, AbstractProxyProject* proxyProject, QObject* parent) :
  DialogPropertiesBrowser(scene, proxyProject, parent),
  m_menu(0),
  m_propUtils(0)
{
  m_propUtils = new PropertiesUtils; 
  connect(this, SIGNAL(changeDlgProperty(const Property&)), this, SLOT(onChangeDlgProperty(const Property&)));
}

te::layout::MenuBuilder::~MenuBuilder()
{
  if(m_propUtils)
  {
    delete m_propUtils;
    m_propUtils = 0;
  }

  if(m_menu)
  {
    delete m_menu;
    m_menu = 0;
  }
}

void te::layout::MenuBuilder::createMenuItem(QMenu* menu, const std::vector<te::layout::Property>& vecProperties)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  foreach(const Property& prop, vecProperties)
  {
    const std::vector<te::layout::Property>& vecSubProperties = prop.getSubProperty();
    if (vecSubProperties.empty() == false)
    {
      createMenuItem(m_menu, vecSubProperties);
    }

    if (!prop.isMenu() || !prop.isVisible())
      continue;

    std::string stdLabel = prop.getLabel();
    if (stdLabel.compare("") == 0)
      stdLabel = prop.getName();

    std::string name = prop.getName();
    std::string icon = prop.getIcon();

    QString qLabel = ItemUtils::convert2QString(stdLabel);
    QString qName = ItemUtils::convert2QString(stdLabel);
    QString qIcon = ItemUtils::convert2QString(stdLabel);

    QAction* action = createAction(qLabel, qName, qIcon);
    m_menu->addAction(action);
    if (prop.getType() == dataType->getDataTypeBool())
    {
      action->setCheckable(true);
      action->setChecked(te::layout::Property::GetValueAs<bool>(prop));
    }
  }
}

void te::layout::MenuBuilder::createMenu( const QList<QGraphicsItem*>& items )
{
  m_graphicsItems = items;
  
  if(m_graphicsItems.empty())
    return;

  bool window = false;
  m_properties = m_propUtils->intersection(items, window);
  
  if(m_properties.getProperties().empty())
    return;

  if(m_menu)
  {
    delete m_menu;
    m_menu = 0;
  }

  m_menu = new QMenu();
  connect(m_menu, SIGNAL(triggered(QAction*)), this, SLOT(onMenuTriggered(QAction*)));

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  te::layout::EnumType* PaperType = Enums::getInstance().getEnumObjectType()->getPaperItem();

  setAllProperties(m_properties);

  if (!(m_properties.getTypeObj() == PaperType))
  {
    QAction* actionCopy = createAction(tr("Copy"), "Copy", "Copy");
    m_menu->addAction(actionCopy);

    QAction* actionCut = createAction(tr("Cut"), "Cut", "Cut");
    m_menu->addAction(actionCut);
  }

  QAction* actionPaste = createAction(tr("Paste"), "Paste", "Paste");
  m_menu->addAction(actionPaste);
  
  if (!(m_properties.getTypeObj() == PaperType))
  {
    QAction* actionRemove = createAction(tr("Remove"), "Remove", "Remove");
    m_menu->addAction(actionRemove);
  }
  m_menu->addSeparator();

  createMenuItem(m_menu, m_properties.getProperties());
}

QAction* te::layout::MenuBuilder::createAction(const QString& text, const QString& objName, const QString& icon, const QString& tooltip)
{
  QAction *actionMenu = new QAction(text, this);
  actionMenu->setObjectName(objName);

  actionMenu->setIcon(QIcon::fromTheme(icon));
  actionMenu->setToolTip(tooltip);

  return actionMenu;
}

void te::layout::MenuBuilder::onMenuTriggered( QAction* action )
{
  if(!m_menu)
    return;

  QString qObjectName = action->objectName();
  std::string objectName = ItemUtils::convert2StdString(qObjectName);

  if (objectName == "Copy")
  {
    m_scene->getView()->copyToClipboard();
  }

  if (objectName == "Cut")
  {
    m_scene->getView()->cutSelectedItens();
  }

  if (objectName == "Paste")
  {
    m_scene->getView()->paste();
  }

  if (objectName == "Remove")
  {
    m_scene->getView()->removeSelectedItem();
  }

  m_currentPropertyClicked = findMnuProperty(objectName);

  if (m_currentPropertyClicked.isNull())
  {
    return;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if (m_currentPropertyClicked.getType() == dataType->getDataTypePageSetup())
  {
    onShowPageSetupDlg();
  }
  else
  {
    menuPropertyClicked(m_currentPropertyClicked);
  }
}

void te::layout::MenuBuilder::checkedBool( bool checked )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property property = m_currentPropertyClicked;

  if(property.getType() != dataType->getDataTypeBool())
    return;

  property.setValue(checked, dataType->getDataTypeBool());
  changePropertyValue(property);
}

void te::layout::MenuBuilder::onChangeDlgProperty( const Property& property )
{
  changePropertyValue(property);
}

void te::layout::MenuBuilder::changePropertyValue( const Property& property )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() == dataType->getDataTypeNone())
    return;
  
  Properties properties;
  properties.addProperty(property);

  m_scene->addChangePropertiesCommandToStack(m_graphicsItems, properties);
  
  m_scene->update();
}

te::layout::Property te::layout::MenuBuilder::findMnuProperty( const std::string& name )
{
  std::vector<te::layout::Property> vecProperties(m_properties.getProperties());

  for(std::size_t i = 0; i < vecProperties.size(); ++i)
  {
    const Property& property = vecProperties[i];

    if (property.getName().compare(name) == 0 || property.getLabel().compare(name) == 0)
    {
      return property;
    }

    const std::vector<te::layout::Property>& vecSubProperties = property.getSubProperty();
    if (vecSubProperties.empty() == false)
    {
      vecProperties.insert(vecProperties.end(), vecSubProperties.begin(), vecSubProperties.end());
    }
  }

  Property emptyProp;
  return emptyProp;
}

void te::layout::MenuBuilder::menuExec( int x, int y )
{ 
  if(!m_menu)
    return;

  QPoint pt(x, y);
  m_menu->exec(pt);
}

void te::layout::MenuBuilder::setSelectedGraphicsItems(QList<QGraphicsItem*>& items)
{
  m_graphicsItems = items;
}

void te::layout::MenuBuilder::setCurrentProperty(std::string name)
{
  bool window = false;
  m_properties = m_propUtils->intersection(m_graphicsItems, window);
  m_currentPropertyClicked = findMnuProperty(name);
}

void te::layout::MenuBuilder::onShowPageSetupDlg()
{
  if (!m_scene)
  {
    return;
  }

  View* view = m_scene->getView();
  if (!view)
  {
    return;
  }

  view->showPageSetup();
}
