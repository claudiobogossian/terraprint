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
  \file PropertyBrowser.h
   
   \brief Manage properties variants values. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_PROPERTIES_BROWSER_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_PROPERTIES_BROWSER_H

// TerraLib
#include "../../../core/Config.h"
#include "../../../core/property/Properties.h"

// STL
#include <string>
#include <vector>

// Qt
#include <QObject>
#include <QVariant>
#include <QString>
#include <QMap>

// QtPropertyBrowser
class QtVariantProperty;
class QtProperty;

class QGraphicsItem;
class QWidget;

namespace te
{
  namespace layout
  {
    class Properties;
    class Property;
    class EnumType;
    class Scene;

    /*!
    \brief Manage properties variants values.  
    
    \ingroup layout
    */
    class TELAYOUTEXPORT AbstractPropertiesBrowser : public QObject
    {
      public:

        AbstractPropertiesBrowser(Scene* scene, QObject *parent = 0);

        virtual ~AbstractPropertiesBrowser();

        virtual QtProperty* addProperty(const Property& property) = 0;

        virtual bool updateProperty(const Property& property) = 0;

        virtual void clearAll();
        
        virtual Property getProperty(const QString& label) = 0;

        virtual EnumType* getLayoutType(QVariant::Type type, const QString& name = "") = 0;

        virtual int getVariantType(EnumType* dataType) = 0;
        
        virtual QVariant findPropertyValue(const QString& label);

        virtual QtProperty* findProperty(const QString& label);

        virtual bool removeProperty(QtProperty* prop);

        virtual void setAllProperties(Properties properties);

        QString nameProperty(const QString& label);

        QString labelProperty(const QString& name);
                          
      protected:

        virtual void createManager() = 0;

        virtual void addPropertyItem(QtProperty *property, const QString &id, const QString &label);

        virtual QVariant checkComplexType(QtVariantProperty* property);

        bool                        m_changeProperty;
        QMap<QtProperty*, QString>  m_propertyToId;
        QMap<QString, QtProperty*>  m_idToProperty;
        QMap<QString, QString>      m_nameToLabel;
        Properties                  m_allProperties;
        Scene*                          m_scene;
    };
  }
}

#endif




