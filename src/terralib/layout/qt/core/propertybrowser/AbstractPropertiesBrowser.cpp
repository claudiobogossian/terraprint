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
#include "AbstractPropertiesBrowser.h"
#include "../../../core/property/Properties.h"
#include "../../../core/enum/Enums.h"
#include "../../../core/property/Variant.h"

// Qt
#include <QVariant>
#include <QFont>
#include <QColor>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtVariantProperty>
#include <QtPropertyBrowser/qteditorfactory.h>
#include <QtPropertyBrowser/QtProperty>

// STL
#include <algorithm>    // std::find

te::layout::AbstractPropertiesBrowser::AbstractPropertiesBrowser(Scene* scene, QObject* parent) :
  QObject(parent),
  m_changeProperty(false),
  m_scene(scene)
{
  
}

te::layout::AbstractPropertiesBrowser::~AbstractPropertiesBrowser()
{
 
}

void te::layout::AbstractPropertiesBrowser::addPropertyItem(QtProperty *property, const QString &id, const QString &label)
{
  m_propertyToId[property] = id;
  m_idToProperty[id] = property;

  // If there is no label, then the name will be the label
  QString labelString = label;
  if (labelString.compare("") == 0)
  {
    labelString = id;
  }
  m_nameToLabel[id] = labelString;
}

void te::layout::AbstractPropertiesBrowser::clearAll()
{
  m_propertyToId.clear();
  m_idToProperty.clear();
  m_allProperties.clear();
}

QString te::layout::AbstractPropertiesBrowser::nameProperty(const QString& label)
{
  QList<QString> labelList = m_nameToLabel.values();

  if (!labelList.contains(label))
  {
    return QString();
  }

  int index = labelList.indexOf(label);
  QString value = labelList.value(index);
  QString name = m_nameToLabel.key(value);

  return name;
}

QString te::layout::AbstractPropertiesBrowser::labelProperty(const QString& name)
{
  if (!m_nameToLabel.contains(name))
  {
    return QString();
  }

  QString foundLabel = m_nameToLabel[name];
  return foundLabel;
}

QVariant te::layout::AbstractPropertiesBrowser::findPropertyValue(const QString& label)
{
  QVariant variant;

  QString name = nameProperty(label);
  if (name.compare("") == 0)
  {
    return variant;
  }

  if (!m_idToProperty.contains(name))
  {
    return variant;
  }

  QtVariantProperty* vproperty = 0;
  QtProperty* prop = m_idToProperty[name];

  if(prop)
  {
    vproperty = dynamic_cast<QtVariantProperty*>(prop);
    if(vproperty)
    {
      variant = checkComplexType(vproperty);
    }
    else
    {
      variant.setValue(prop->valueText());
    }
  }

  return variant;
}

QtProperty* te::layout::AbstractPropertiesBrowser::findProperty(const QString& label)
{
  QtProperty* prop = 0;

  QString name = nameProperty(label);
  if (name.compare("") == 0)
  {
    return prop;
  }

  if (!m_idToProperty.contains(name))
  {
    return prop;  
  }

  prop = m_idToProperty[name];
  return prop;
}

QVariant te::layout::AbstractPropertiesBrowser::checkComplexType( QtVariantProperty* property )
{
  QVariant variant;

  if(!property)
    return variant;

  variant = property->value();

  if(QtVariantPropertyManager::enumTypeId() == property->propertyType())
  {
    variant = property->attributeValue("enumNames");
    QStringList list = variant.toStringList();
    variant = QVariant(list);
  }

  return variant;
}

bool te::layout::AbstractPropertiesBrowser::removeProperty( QtProperty* prop )
{
  if(!prop)
  {
    return false;
  }
  
  QString name = nameProperty(prop->propertyName());
  if (name.compare("") == 0)
  {
    return prop;
  }

  if (!m_idToProperty.contains(name))
  {
    return false;  
  }

  m_propertyToId.remove(prop);
  m_idToProperty.remove(name);

  return true;
}

void te::layout::AbstractPropertiesBrowser::setAllProperties(Properties properties)
{
  m_allProperties = properties;
}







