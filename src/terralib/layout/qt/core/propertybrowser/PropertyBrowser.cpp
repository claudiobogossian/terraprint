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
#include "../../core/ItemUtils.h"
#include "../../core/Scene.h"
#include "DialogPropertiesBrowser.h"
#include "ItemObserverFactory.h"
#include "ItemObserverManager.h"
#include "VariantPropertiesBrowser.h"
#include "ColorEditorFactory.h"

// QtPropertyBrowser
#include <QtPropertyBrowser/QtStringPropertyManager>
#include <QtPropertyBrowser/QtColorPropertyManager>
#include <QtPropertyBrowser/QtProperty>
#include <QtPropertyBrowser/QtBrowserItem>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/qteditorfactory.h>
#include <QtPropertyBrowser/QtGroupPropertyManager>
#include <QRegExpValidator>
#include <QRegExp>
#include <QWidget>
#include <QVariant>
#include <QFont>
#include <QColor>

// STL
#include <algorithm>    // std::find

te::layout::PropertyBrowser::PropertyBrowser(Scene* scene, AbstractProxyProject* proxyProject, QObject* parent) :
  QObject(parent),
  m_propertyEditor(0),
  m_variantPropertiesBrowser(0),
  m_dialogPropertiesBrowser(0),
  m_scene(scene),
  m_hasWindows(false),
  m_changeQtPropertyVariantValue(false),
  m_ignoreExternalUpdates(false)
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
  m_variantPropertiesBrowser = new VariantPropertiesBrowser(m_scene);
  connect(m_variantPropertiesBrowser->getVariantPropertyManager(), SIGNAL(valueChanged(QtProperty*, const QVariant &)),
    this, SLOT(propertyEditorValueChanged(QtProperty *, const QVariant &)));

  // Dialog properties
  m_dialogPropertiesBrowser = new DialogPropertiesBrowser(scene, proxyProject, this->parent());
  connect(m_dialogPropertiesBrowser, SIGNAL(changeDlgProperty(const Property&)), this, SLOT(onChangeDlgProperty(const Property&)));

  // Item Observer properties
  m_itemObserverManager = new ItemObserverManager(scene);

  connect(m_itemObserverManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)),
    this, SLOT(propertyEditorValueChanged(QtProperty *, const QVariant &)));

  connect(m_dialogPropertiesBrowser->getColorPropertyManager(), SIGNAL(valueChanged(QtProperty*, const QColor&)),
    this, SLOT(propertyEditorValueChanged(QtProperty *, const QColor&)));

  ColorEditorFactory* colorEditorFactory = new ColorEditorFactory;
  ItemObserverFactory* itemObserverFactory = new ItemObserverFactory;

  m_propertyEditor->setFactoryForManager(m_dialogPropertiesBrowser->getColorPropertyManager(), colorEditorFactory);
  m_propertyEditor->setFactoryForManager(m_dialogPropertiesBrowser->getStringPropertyManager(), m_dialogPropertiesBrowser->getDlgEditorFactory());
  m_propertyEditor->setFactoryForManager(m_variantPropertiesBrowser->getVariantPropertyManager(), m_variantPropertiesBrowser->getVariantEditorFactory());
  m_propertyEditor->setFactoryForManager(m_itemObserverManager, itemObserverFactory);
  m_propertyEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_propertyEditor->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
}

void te::layout::PropertyBrowser::propertyEditorValueChanged( QtProperty *property, const QVariant &value )
{
  if(m_changeQtPropertyVariantValue)
  {
    return;  
  }

  QString qPropertyName = property->propertyName();
  std::string propertyName = qPropertyName.toStdString();

  QString qCurrentValue = property->valueText();
  std::string cCurrentValue = qCurrentValue.toStdString();

  QString qNewValue = value.toString();
  std::string cNewValue = qNewValue.toStdString();

  if (m_qtpropertyToProperty.find(property) == m_qtpropertyToProperty.end())
  {
    return; 
  }

  Property prop = m_variantPropertiesBrowser->getProperty(property);

  if(prop.isNull())
  {
    prop = m_dialogPropertiesBrowser->getProperty(property);
  }

  if (prop.isNull())
  {
    prop = m_itemObserverManager->getProperty(property);
  }
  
  m_ignoreExternalUpdates = true;
  emit changePropertyValue(prop);
  m_ignoreExternalUpdates = false;
}

void te::layout::PropertyBrowser::propertyEditorValueChanged(QtProperty *property, const QColor &value)
{
  QVariant variant(value);
  propertyEditorValueChanged(property, variant);
}

void te::layout::PropertyBrowser::onChangeDlgProperty( const Property& property )
{
  if(m_changeQtPropertyVariantValue)
  {
    return;  
  }
  emit changePropertyValue(property);
}

void te::layout::PropertyBrowser::clearAll()
{
  QMap<QtProperty *, Property>::ConstIterator itProp = m_qtpropertyToProperty.constBegin();
  while (itProp != m_qtpropertyToProperty.constEnd()) {
    delete itProp.key();
    itProp++;
  }

  m_qtpropertyToProperty.clear();

  m_variantPropertiesBrowser->clearAll();
  m_dialogPropertiesBrowser->clearAll();
}

void te::layout::PropertyBrowser::addPropertyItem(QtProperty *property, const QString &id, const QString &label)
{
  QtBrowserItem *item = m_propertyEditor->addProperty(property);
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
  QtProperty* pproperty = 0;

  if(!property.isVisible())
  {
    return pproperty;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  EnumObjectType* objType = Enums::getInstance().getEnumObjectType();

  if(!dataType || !objType)
  {
    return 0;
  }

  m_changeQtPropertyVariantValue = true;
  
  pproperty = addVariantProperty(property); // add variant property 
  if (!pproperty)
  {
    if (property.getType() != dataType->getDataTypeItemObserver()
      && property.getType() != dataType->getDataTypeGroupProperties())
    {
      pproperty = addDialogProperty(property); // add dialog property
    }
    else if (property.getType() == dataType->getDataTypeItemObserver())
    {
      pproperty = addItemProperty(property); // add item observer property
    }
  }

  if (pproperty)
  {
    if (property.getType() != dataType->getDataTypeGroupProperties())
    {
      gatherProperties(pproperty, property); // gather properties (has a root property with a value and subproperties)
    }
  }
  else
  {
    if (property.getType() == dataType->getDataTypeGroupProperties())
    {
      createGroup(pproperty, property); // group subproperties (has a root property without a value (has subproperties). For group of types, styles, etc.)
    }
  }

  associateProperty(pproperty, property);

  m_changeQtPropertyVariantValue = false;
  return pproperty;
}

void te::layout::PropertyBrowser::addProperties(const Properties& properties)
{
  std::vector<Property> vecProperties = properties.getProperties();
  foreach(Property prop, vecProperties)
  {
    if (!prop.isVisible())
      continue;

    addProperty(prop);
  }

  m_variantPropertiesBrowser->setAllProperties(properties);
  m_dialogPropertiesBrowser->setAllProperties(properties);
}

QtProperty* te::layout::PropertyBrowser::addVariantProperty(const Property& property)
{
  QtVariantProperty* vproperty = 0;

  QtProperty* pproperty = m_variantPropertiesBrowser->addProperty(property);
  if (!pproperty)
    return vproperty;

  vproperty = dynamic_cast<QtVariantProperty*>(pproperty);
  if (vproperty)
  {
    bool is_readOnly = !property.isEditable();

    std::string label = property.getLabel();
    QString qLabel = ItemUtils::convert2QString(label);

    std::string name = property.getName();
    QString qName = ItemUtils::convert2QString(name);

    QString readOnly("readOnly");
    vproperty->setAttribute(readOnly, is_readOnly);
    addPropertyItem(vproperty, qName, qLabel);
  }
  return vproperty;
}

QtProperty* te::layout::PropertyBrowser::addDialogProperty(const Property& property)
{
  QtProperty* pproperty = 0;

  pproperty = m_dialogPropertiesBrowser->addProperty(property);

  if (pproperty)
  {
    std::string label = property.getLabel();
    QString qLabel = ItemUtils::convert2QString(label);

    std::string name = property.getName();
    QString qName = ItemUtils::convert2QString(name);

    addPropertyItem(pproperty, qName, qLabel);

    QtVariantProperty* variantItem = dynamic_cast<QtVariantProperty*>(pproperty);
    if (variantItem != 0)
    {
      variantItem->setAttribute("parentClass", ItemUtils::convert2QString(property.getParent()));
    }
  }

  return pproperty;
}

QtProperty* te::layout::PropertyBrowser::addItemProperty(const Property& property)
{
  QtProperty* pproperty = 0;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  EnumObjectType* objType = Enums::getInstance().getEnumObjectType();

  if (!dataType || !objType)
  {
    return pproperty;
  }

  if (property.getType() != dataType->getDataTypeItemObserver())
    return pproperty;

  std::string stdLabel = property.getLabel();
  if (stdLabel.compare("") == 0)
    stdLabel = property.getName();

  QString label = ItemUtils::convert2QString(stdLabel);
  
  pproperty = m_itemObserverManager->addProperty(label);
  if (!pproperty)
    return pproperty;

  QString val("");
  if (!property.isNull())
  {
    const AbstractItemView* view = m_scene->getItem(te::layout::Property::GetValueAs<std::string>(property));
    if (view)
    {
      const Property& property = view->getProperty("name");
      std::string value = te::layout::Property::GetValueAs<std::string>(property);

      val = ItemUtils::convert2QString(value);
    }
  }

  stdLabel = property.getLabel();
  QString qLabel = ItemUtils::convert2QString(stdLabel);

  std::string name = property.getName();
  QString qName = ItemUtils::convert2QString(name);

  m_itemObserverManager->setValue(pproperty, val);
  m_itemObserverManager->setTypeForSearch(pproperty, objType->getMapItem());
  m_itemObserverManager->setPropertyLabel(pproperty, qName, qLabel, property); // add label
  addPropertyItem(pproperty, qName, qLabel);

  return pproperty;
}

void te::layout::PropertyBrowser::gatherProperties(QtProperty* qproperty, Property property)
{
  if (property.getSubProperty().empty())
    return;
    
  const std::vector<Property>& vecProperties = property.getSubProperty();
  foreach(Property prop, vecProperties)
  {
    if (!prop.isVisible())
      continue;

    QtProperty* subProperty = addProperty(prop); // create and add property
    addSubProperty(qproperty, subProperty); // change property to sub property
  }
}

void te::layout::PropertyBrowser::createGroup(QtProperty* qproperty, Property property)
{
  std::string stdLabel = property.getLabel();
  if (stdLabel.compare("") == 0)
    stdLabel = property.getName();

  QString label = ItemUtils::convert2QString(stdLabel);

  QtGroupPropertyManager*  groupManager = new QtGroupPropertyManager;
  QtProperty* groupProperty = groupManager->addProperty(label);

  if (!property.getSubProperty().empty())
  {
    const std::vector<Property>& vecProperties = property.getSubProperty();
    foreach(Property prop, vecProperties)
    {
      if (!prop.isVisible())
        continue;

      QtProperty* subProperty = addProperty(prop); // create and add property
      groupProperty->addSubProperty(subProperty); // change property to group
    }
  }
  
  stdLabel = property.getLabel();
  QString qLabel = ItemUtils::convert2QString(stdLabel);

  std::string name = property.getName();
  QString qName = ItemUtils::convert2QString(name);

  //add group property to the property browser tree
  addPropertyItem(groupProperty, qName, qLabel);
}

void te::layout::PropertyBrowser::setHasWindows( bool hasWindows )
{
  m_hasWindows = hasWindows;
  blockOpenWindows(!hasWindows);
}

void te::layout::PropertyBrowser::blockOpenWindows( bool block )
{

}

void te::layout::PropertyBrowser::selectProperty(std::string label)
{
  QList<QtProperty*> props = m_propertyEditor->properties();
  foreach( QtProperty* prop, props) 
  {
    if(prop)
    {
      QString qLabel = ItemUtils::convert2QString(label);
      if (qLabel.compare(prop->propertyName()) == 0)
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
  if (m_ignoreExternalUpdates == true)
  {
    return false;
  }

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
  if (m_ignoreExternalUpdates == true)
  {
    return;
  }

  foreach( Property prop, props.getProperties()) 
  {
    updateProperty(prop);
    foreach(Property sub, prop.getSubProperty())
    {
      updateProperty(sub);
    }
  }

  m_variantPropertiesBrowser->setAllProperties(props);
  m_dialogPropertiesBrowser->setAllProperties(props);
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
    Property property = m_variantPropertiesBrowser->getProperty(prop);

    if(property.isNull())
    {
      property = m_dialogPropertiesBrowser->getProperty(prop);
    }

    if (property.isNull())
    {
      property = m_itemObserverManager->getProperty(prop);
    }

    properties.addProperty(property);
  }

  return properties;
}

QtProperty* te::layout::PropertyBrowser::findProperty(const std::string& propertyName, const std::string& parentClass)
{
  QtProperty* prop = 0;

  prop = m_variantPropertiesBrowser->findProperty(propertyName, parentClass);
  
  if(!prop)
  {
    prop = m_dialogPropertiesBrowser->findProperty(propertyName, parentClass);
  }
  
  if (!prop)
  {
    prop = m_itemObserverManager->findProperty(propertyName, parentClass);
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
      QtProperty* qtprop = findProperty(prop.getName(), prop.getParent());
      if(!qtprop)
      {
        result = false;
      }
    }
  }
  
  return result;
}

void te::layout::PropertyBrowser::associateProperty(QtProperty* qtProperty, const te::layout::Property& property)
{
  if (qtProperty == 0)
  {
    return;
  }
  if (property.isNull() == true)
  {
    return;
  }
  m_qtpropertyToProperty.insert(qtProperty, property);
}