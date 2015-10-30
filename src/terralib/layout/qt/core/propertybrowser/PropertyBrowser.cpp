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
  \file PropertyBrowser.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PropertyBrowser.h"
#include "../../../core/property/Properties.h"
#include "../../../core/enum/Enums.h"
#include "../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../core/property/GenericVariant.h"
#include "VariantPropertiesBrowser.h"
#include "DialogPropertiesBrowser.h"
#include <QRegExpValidator>
#include <QRegExp>
#include <QWidget>
#include <QVariant>
#include <QFont>
#include <QColor>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtStringPropertyManager>
#include <QtPropertyBrowser/QtProperty>
#include <QtPropertyBrowser/QtBrowserItem>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/qteditorfactory.h>

// STL
#include <algorithm>    // std::find
#include "ItemObserverFactory.h"
#include "ItemObserverManager.h"

te::layout::PropertyBrowser::PropertyBrowser(Scene* scene, AbstractProxyProject* proxyProject, QObject* parent) :
  QObject(parent),
  m_propertyEditor(0),
  m_variantPropertiesBrowser(0),
  m_dialogPropertiesBrowser(0),
  m_scene(scene),
  m_hasWindows(false),
  m_changeQtPropertyVariantValue(false)
{
  createManager(scene, proxyProject);
}

te::layout::PropertyBrowser::~PropertyBrowser()
{
  clearAll();

  if(m_variantPropertiesBrowser)
  {
    delete m_variantPropertiesBrowser;
    m_variantPropertiesBrowser = 0;
  }

  if(m_dialogPropertiesBrowser)
  {
    delete m_dialogPropertiesBrowser;
    m_dialogPropertiesBrowser = 0;
  }
  
  if(m_propertyEditor)
  {
    delete m_propertyEditor;
    m_propertyEditor = 0;
  }
}

void te::layout::PropertyBrowser::createManager( Scene* scene, AbstractProxyProject* proxyProject )
{
  //Qt - The Property Browser
  m_propertyEditor = new QtTreePropertyBrowser;
  
  // Variant properties
  m_variantPropertiesBrowser = new VariantPropertiesBrowser;
  connect(m_variantPropertiesBrowser->getVariantPropertyManager(), SIGNAL(valueChanged(QtProperty*, const QVariant &)),
    this, SLOT(propertyEditorValueChanged(QtProperty *, const QVariant &)));

  // Dialog properties
  m_dialogPropertiesBrowser = new DialogPropertiesBrowser;
  connect(m_dialogPropertiesBrowser, SIGNAL(changeDlgProperty(Property)), this, SLOT(onChangeDlgProperty(Property)));
  connect(m_dialogPropertiesBrowser, SIGNAL(changeDlgProperty(std::vector<Property>)), this, SLOT(onChangeDlgProperty(std::vector<Property>)));

  // Item Observer properties
  m_itemObserverManager = new ItemObserverManager(m_scene);
  connect(m_itemObserverManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)),
    this, SLOT(propertyEditorValueChanged(QtProperty *, const QVariant &)));
  ItemObserverFactory* itemObserverFactory = new ItemObserverFactory;
  
  m_propertyEditor->setFactoryForManager(m_dialogPropertiesBrowser->getStringPropertyManager(), m_dialogPropertiesBrowser->getDlgEditorFactory());
  m_propertyEditor->setFactoryForManager(m_variantPropertiesBrowser->getVariantPropertyManager(), m_variantPropertiesBrowser->getVariantEditorFactory());
  m_propertyEditor->setFactoryForManager(m_itemObserverManager, itemObserverFactory);
  m_propertyEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_propertyEditor->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  connect(m_propertyEditor, SIGNAL(currentItemChanged(QtBrowserItem*)), this, SLOT(onCurrentItemChanged(QtBrowserItem*)));
}

void te::layout::PropertyBrowser::propertyEditorValueChanged( QtProperty *property, const QVariant &value )
{
  if(m_changeQtPropertyVariantValue)
  {
    return;  
  }

  QList<QtBrowserItem *> list = m_propertyEditor->items(property);
  emit changePropertyValue(property, list);

  Property prop = m_variantPropertiesBrowser->getProperty(property->propertyName().toStdString());

  if(prop.isNull())
  {
    prop = m_dialogPropertiesBrowser->getProperty(property->propertyName().toStdString());
  }

  if (prop.isNull())
  {
    prop = m_itemObserverManager->getProperty(property->propertyName().toStdString());
  }

  emit changePropertyValue(prop);
}

void te::layout::PropertyBrowser::onChangeDlgProperty( Property property )
{
  if(m_changeQtPropertyVariantValue)
  {
    return;  
  }
  emit changePropertyValue(property);
}

void te::layout::PropertyBrowser::onChangeDlgProperty( std::vector<Property> props )
{
  emit changePropertyValue(props);
}

void te::layout::PropertyBrowser::onCurrentItemChanged(QtBrowserItem* item)
{
  if(item == 0)
  {
    return;
  }
  emit currentItemChanged(item);
}

void te::layout::PropertyBrowser::updateExpandState()
{
  QList<QtBrowserItem *> list = m_propertyEditor->topLevelItems();
  QListIterator<QtBrowserItem *> it(list);
  while (it.hasNext()) {
    QtBrowserItem *item = it.next();
    QtProperty *prop = item->property();
    m_idToExpanded[m_propertyToId[prop]] = m_propertyEditor->isExpanded(item);
  }
}

void te::layout::PropertyBrowser::clearAll()
{
  updateExpandState();

  QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToId.constBegin();
  while (itProp != m_propertyToId.constEnd()) {
    delete itProp.key();
    itProp++;
  }

  m_propertyToId.clear();
  m_idToProperty.clear();
  
  m_variantPropertiesBrowser->clearAll();
  m_dialogPropertiesBrowser->clearAll();
}

void te::layout::PropertyBrowser::addPropertyItem(QtProperty *property, const QString &id)
{
  m_propertyToId[property] = id;
  m_idToProperty[id] = property; 
  QtBrowserItem *item = m_propertyEditor->addProperty(property);
  if (m_idToExpanded.contains(id))
    m_propertyEditor->setExpanded(item, m_idToExpanded[id]);
}

void te::layout::PropertyBrowser::onChangeFilter( const QString& filter )
{
  QRegExp rx;
  QString search_text = filter;
  bool doesContain = false;
  search_text.replace(" ","|"); // to make it possible to look up all words given for the search
  rx.setPattern(search_text);
  rx.setCaseSensitivity(Qt::CaseInsensitive);
  
  QList<QtProperty*> list = m_propertyEditor->properties();
  foreach( QtProperty* prop, list) 
  {
    doesContain = false;
    if(prop) 
    {
      doesContain = prop->propertyName().contains(rx);
      changeVisibility(m_propertyEditor->items(prop), doesContain);
    }
  }
}

void te::layout::PropertyBrowser::changeVisibility( QList<QtBrowserItem*> items, bool visible )
{
  foreach(QtBrowserItem* item, items) 
  {
    if(item)
    {
      m_propertyEditor->setItemVisible(item, visible);
    }
  }
}

QtTreePropertyBrowser* te::layout::PropertyBrowser::getPropertyEditor()
{
  return m_propertyEditor;
}

QtProperty* te::layout::PropertyBrowser::addProperty( const Property& property )
{
  QtVariantProperty* vproperty = 0;

  if(!property.isVisible())
  {
    return vproperty;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  EnumObjectType* objType = Enums::getInstance().getEnumObjectType();

  if(!dataType || !objType)
  {
    return 0;
  }

  m_changeQtPropertyVariantValue = true;
  
  QtProperty* qProperty = m_variantPropertiesBrowser->addProperty(property);
  vproperty = dynamic_cast<QtVariantProperty*>(qProperty);
  if(vproperty) 
  {
    bool is_readOnly = !property.isEditable();
    vproperty->setAttribute(QLatin1String("readOnly"), is_readOnly);
    addPropertyItem(vproperty, QLatin1String(property.getName().c_str()));
  }
  else
  {
    QtProperty* pproperty = 0;
    GenericVariant gv = property.getValue().toGenericVariant();
    if (gv.getType() != dataType->getDataTypeItemObserver())
    {
      pproperty = m_dialogPropertiesBrowser->addProperty(property);
      if (pproperty)
      {
        addPropertyItem(pproperty, QLatin1String(property.getName().c_str()));
      }
      else
      {
        return pproperty;
      }
    }
    else
    {
      pproperty = m_itemObserverManager->addProperty(property.getName().c_str());
      QString val("");
      if (property.getValue().toGenericVariant().toItem() != 0)
      {
        const AbstractItemView* view = property.getValue().toGenericVariant().toItem();
        const Property& property = view->getController()->getProperty("name");
        val = property.getValue().toString().c_str();
      }
      m_itemObserverManager->setValue(pproperty, val);
      m_itemObserverManager->setTypeForSearch(pproperty, objType->getMapItem());
      addPropertyItem(pproperty, QLatin1String(property.getName().c_str()));
    }
  }
  m_changeQtPropertyVariantValue = false;
  return vproperty;
}

QMap<QString, QtProperty*> te::layout::PropertyBrowser::addProperties(const Properties& properties)
{
  std::vector<Property> vecProperties = properties.getProperties();
  foreach(Property prop, vecProperties)
  {
    if (!prop.isVisible())
      continue;

    addProperty(prop);
  }

  Properties props = properties;

  m_variantPropertiesBrowser->setAllProperties(props);
  m_dialogPropertiesBrowser->setAllProperties(props);

  return m_idToProperty;
}

bool te::layout::PropertyBrowser::removeProperty( Property property )
{
  QtProperty* removeProp = 0;
  QList<QtProperty*> list = m_propertyEditor->properties();
  foreach( QtProperty* prop, list) 
  {
    if(property.getName().compare(prop->propertyName().toStdString()) == 0)
    {
      removeProp = prop;
    }
  }
  
  if(!removeProp)
    return false;

  m_propertyToId.remove(removeProp);
  m_idToProperty.remove(removeProp->propertyName());

  m_variantPropertiesBrowser->removeProperty(removeProp);
  m_dialogPropertiesBrowser->removeProperty(removeProp);

  m_propertyEditor->removeProperty(removeProp);

  if(removeProp)
  {
    delete removeProp;
    removeProp = 0;
  }

  return true;
}

void te::layout::PropertyBrowser::setHasWindows( bool hasWindows )
{
  m_hasWindows = hasWindows;
  blockOpenWindows(!hasWindows);
}

void te::layout::PropertyBrowser::blockOpenWindows( bool block )
{

}

void te::layout::PropertyBrowser::selectProperty( std::string name )
{
  QList<QtProperty*> props = m_propertyEditor->properties();
  foreach( QtProperty* prop, props) 
  {
    if(prop)
    {
      if(name.compare(prop->propertyName().toStdString()) == 0)
      {
        QList<QtBrowserItem *> list = m_propertyEditor->items(prop);
        QtBrowserItem* item = list.first();
        if(item)
        {
          m_propertyEditor->setCurrentItem(item);
        }
      }
    }
  }
}

bool te::layout::PropertyBrowser::updateProperty( Property property )
{
  m_changeQtPropertyVariantValue = true;
  
  bool result = m_variantPropertiesBrowser->updateProperty(property);
  
  if (!result)
  {
    bool resultItemObserver = m_itemObserverManager->updateProperty(property);
  }

  m_changeQtPropertyVariantValue = false;
  
  return result;
}

void te::layout::PropertyBrowser::updateProperties( Properties props )
{
  foreach( Property prop, props.getProperties()) 
  {
    updateProperty(prop);
  }
}

te::layout::VariantPropertiesBrowser* te::layout::PropertyBrowser::getVariantPropertiesBrowser()
{
  return m_variantPropertiesBrowser;
}

te::layout::DialogPropertiesBrowser* te::layout::PropertyBrowser::getDialogPropertiesBrowser()
{
  return m_dialogPropertiesBrowser;
}

void te::layout::PropertyBrowser::closeAllWindows()
{
  m_dialogPropertiesBrowser->closeAllWindows();
}

te::layout::Properties te::layout::PropertyBrowser::getProperties()
{
  Properties properties("");

  QList<QtProperty*> props = m_propertyEditor->properties();
  foreach( QtProperty* prop, props) 
  {
    Property property = m_variantPropertiesBrowser->getProperty(prop->propertyName().toStdString());

    if(property.isNull())
    {
      property = m_dialogPropertiesBrowser->getProperty(prop->propertyName().toStdString());
    }

    if (property.isNull())
    {
      property = m_itemObserverManager->getProperty(prop->propertyName().toStdString());
    }

    properties.addProperty(property);
  }

  return properties;
}

QtProperty* te::layout::PropertyBrowser::findProperty( std::string name )
{
  QtProperty* prop = 0;

  prop = m_variantPropertiesBrowser->findProperty(name);
  
  if(!prop)
  {
    prop = m_dialogPropertiesBrowser->findProperty(name);
  }
  
  if (!prop)
  {
    prop = m_itemObserverManager->findProperty(name);
  }

  return prop;
}

bool te::layout::PropertyBrowser::addSubProperty( QtProperty* prop, QtProperty* subProp )
{
  if(!prop)
  {
    return false;
  }

  m_propertyEditor->removeProperty(subProp);
  prop->addSubProperty(subProp);
  return true;
}

bool te::layout::PropertyBrowser::addSubProperty( Property prop, Property subProp )
{
  if(prop.isNull() || subProp.isNull())
  {
    return false;
  }

  QtProperty* addProp = addProperty(prop);
  QtProperty* addSubProp = addProperty(subProp);  
  
  return addSubProperty(addProp, addSubProp);
}

bool te::layout::PropertyBrowser::equalsProperties( Properties props )
{
  bool result = true;

  foreach( Property prop, props.getProperties()) 
  {
    if(prop.isVisible())
    {
      QtProperty* qtprop = findProperty(prop.getName());
      if(!qtprop)
      {
        result = false;
      }
    }
  }
  
  return result;
}









