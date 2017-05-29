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

void te::layout::AbstractPropertiesBrowser::addPropertyItem(QtProperty *qtProperty, const QString &id, const QString &label, const te::layout::Property& layoutProperty)
{
  m_mapProperty[qtProperty] = layoutProperty;
}

void te::layout::AbstractPropertiesBrowser::clearAll()
{
  m_allProperties.clear();
  m_mapProperty.clear();
}

QVariant te::layout::AbstractPropertiesBrowser::getPropertyValue(QtProperty* qtproperty)
{
  QVariant variant;

  if (qtproperty == 0)
  {
    return variant;
  }

  QtVariantProperty* vproperty = dynamic_cast<QtVariantProperty*>(qtproperty);
  if(vproperty)
  {
    variant = checkComplexType(vproperty);
  }
  else
  {
    QtColorPropertyManager* colorPropertyManager = dynamic_cast<QtColorPropertyManager*>(qtproperty->propertyManager());
    if (colorPropertyManager != 0)
    {
      QColor color = colorPropertyManager->value(qtproperty);
      variant.setValue(color);
    }
    else
    {
      variant.setValue(qtproperty->valueText());
    }
  }
  
  return variant;
}

QtProperty* te::layout::AbstractPropertiesBrowser::findProperty(const std::string& propertyName, const std::string& parentClass)
{
  QMap<QtProperty*, Property>::const_iterator it = m_mapProperty.begin();
  while (it != m_mapProperty.end())
  {
    const Property& property = it.value();
    if (property.getName() == propertyName && property.getParent() == parentClass)
    {
      return it.key();
    }

    ++it;
  }

  return 0;
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

void te::layout::AbstractPropertiesBrowser::setAllProperties(const Properties& properties)
{
  m_allProperties = properties;
}

