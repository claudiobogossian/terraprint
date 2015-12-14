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
    \file ItemObserverManager.cpp

    \brief

    \ingroup layout
*/

// TerraLib
#include "ItemObserverManager.h"

#include "../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../core/enum/Enums.h"
#include "../../../core/property/Properties.h"
#include "../Scene.h"

// Qt
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>

te::layout::ItemObserverManager::ItemObserverManager(Scene* scene, QObject *parent /*= 0*/)
  : QtAbstractPropertyManager(parent)
  , m_scene(scene)
{

}

te::layout::ItemObserverManager::~ItemObserverManager()
{

}

QString te::layout::ItemObserverManager::value(const QtProperty *property) const
{
  if (!m_values.contains(property))
    return QString();
  return m_values[property].currentName;
}

QStringList te::layout::ItemObserverManager::names(const QtProperty *property) const
{
  if (!m_values.contains(property))
    return QStringList();
  return m_values[property].names;
}

te::layout::AbstractItemView* te::layout::ItemObserverManager::item(const QtProperty *property) const
{
  if (!m_values.contains(property))
    return 0;
  return m_values[property].item;
}

void te::layout::ItemObserverManager::setValue(QtProperty *property, const QString &val)
{
  if (!m_values.contains(property))
    return;

  Data data = m_values[property];

  if (data.currentName == val)
    return;

  data.currentName = val;
  
  AbstractItemView* view = findItem(val);  
  data.item = view; // Observer pattern relationship. Associate: != 0 / Dissociate : == 0.
  m_values[property] = data;

  emit propertyChanged(property);
  QVariant vCurrentName(data.currentName);
  emit valueChanged(property, vCurrentName);

  emit itemChanged(property, data.item);
}

QString te::layout::ItemObserverManager::valueText(const QtProperty *property) const
{
  return value(property);
}

void te::layout::ItemObserverManager::initializeProperty(QtProperty *property)
{
  Data data = Data();

  QStringList names = getItemNames(0);
  data.names = names;

  m_values[property] = data;

  emit listNamesChanged(property, data.names);
}

void te::layout::ItemObserverManager::uninitializeProperty(QtProperty *property)
{
  m_values.remove(property);
}

te::layout::AbstractItemView* te::layout::ItemObserverManager::findItem(const QString &name)
{
  AbstractItemView* viewFound = 0;

  if (name.compare("") == 0)
    return viewFound;

  QList<QGraphicsItem*> items = m_scene->items();
  foreach (QGraphicsItem* item, items)
  {
    if (item)
    {
      AbstractItemView* view = dynamic_cast<AbstractItemView*>(item);
      if (view)
      {
        const Property& property = view->getController()->getProperty("name");
        if (name.compare(property.getValue().toString().c_str()) == 0)
        {
          viewFound = view;
          break;
        }
      }
    }
  }
  
  return viewFound;
}

QStringList te::layout::ItemObserverManager::getItemNames(const EnumType* type)
{
  QStringList names;

  QList<QGraphicsItem*> items = m_scene->items();
  foreach(QGraphicsItem* item, items)
  {
    if (item)
    {
      AbstractItemView* view = dynamic_cast<AbstractItemView*>(item);
      if (view)
      {
        const Property& prop_name = view->getController()->getProperty("name");
        QString txt(prop_name.getValue().toString().c_str());
        const Properties& prop_type = view->getController()->getProperties();

        if (txt.compare("") != 0 && prop_type.getTypeObj() == type)
        {
          names.append(txt);
        }
      }
    }
  }

  return names;
}

te::layout::Property te::layout::ItemObserverManager::getProperty(const std::string& label)
{
  Property prop;

  QString name = nameProperty(label);
  if (name.compare("") == 0)
  {
    return prop;
  }

  prop.setName(name.toStdString());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if (!dataType)
  {
    return prop;
  }

  QSet<QtProperty*> props = properties();
  foreach(QtProperty* p, props)
  {
    if (p->propertyName() == label.c_str())
    {
      Data data = m_values[p];
      std::string currentName = data.currentName.toStdString();
      prop.setValue(currentName, dataType->getDataTypeItemObserver());
      break;
    }
  }

  return prop;
}

bool te::layout::ItemObserverManager::updateProperty(const Property& property)
{
  bool bupdate_property = true;

  QtProperty* qprop = 0;
  Data data;

  std::string label = property.getLabel();
  if (label.compare("") == 0)
    label = property.getName();

  QSet<QtProperty*> props = properties();
  foreach(QtProperty* p, props)
  {    
    if (p->propertyName() == label.c_str())
    {
      qprop = p;

      data = m_values[p];
      const std::string itemName = property.getValue().toString();
      if (!itemName.empty())
      {
        const Property& property = data.item->getController()->getProperty("name");
        if (property.getValue().toString().c_str() == data.currentName)
        {
          bupdate_property = false;
          break;
        }
        else
        {
          AbstractItemView* item = findItem(itemName.c_str());
          if (item)
          {
            data.item = const_cast<AbstractItemView*>(item);
            data.currentName = property.getValue().toString().c_str();
            setValue(qprop, data.currentName); //change qproperty
          }
          break;
        }
      }
      else
      {
        if (data.item == 0)
        {
          bupdate_property = false;
        }
      }
    }
  }
  
  return bupdate_property;
}

QtProperty* te::layout::ItemObserverManager::findProperty(const std::string& label)
{
  QtProperty* prop = 0;

  QList<QString> labelList = m_nameToLabel.values();

  QString name(label.c_str());
  if (!labelList.contains(name))
  {
    name = nameProperty(label);
    if (name.compare("") == 0)
    {
      return prop;
    }
  }

  QSet<QtProperty*> props = properties();
  foreach(QtProperty* p, props)
  {
    if (p->propertyName().compare(name) == 0)
    {
      prop = p;
      break;
    }
  }

  return prop;
}

void te::layout::ItemObserverManager::setTypeForSearch(QtProperty *property, const EnumType* type)
{
  if (!property)
    return;

  Data data = m_values[property];

  data.typeSearchNames = const_cast<EnumType*>(type);
  
  QStringList names = getItemNames(type);
  data.names = names;

  m_values[property] = data;

  emit listNamesChanged(property, data.names);
}

QString te::layout::ItemObserverManager::propertyLabel(const QtProperty *property) const
{
  if (!m_nameToLabel.contains(property->propertyName()))
    return QString();
  return m_nameToLabel[property->propertyName()];
}

void te::layout::ItemObserverManager::setPropertyLabel(QtProperty *property, const QString &name, const QString &label)
{
  if (!m_values.contains(property))
    return;

  QString labelString = label;
  if (labelString.compare("") == 0)
    labelString = name;

  m_nameToLabel[name] = labelString;
}

QString te::layout::ItemObserverManager::nameProperty(const std::string& label)
{
  QList<QString> labelList = m_nameToLabel.values();

  if (!labelList.contains(label.c_str()))
  {
    return QString();
  }

  int index = labelList.indexOf(label.c_str());
  QString value = labelList.value(index);
  QString name = m_nameToLabel.key(value);

  return name;
}

QString te::layout::ItemObserverManager::labelProperty(const std::string& name)
{
  if (!m_nameToLabel.contains(name.c_str()))
  {
    return QString();
  }

  QString foundLabel = m_nameToLabel[name.c_str()];
  return foundLabel;
}

