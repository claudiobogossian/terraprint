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
    \file ItemObserverManager.h

    \brief

    \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_OBSERVER_MANAGER_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_OBSERVER_MANAGER_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../core/property/Property.h"
#endif

// Qt
#include <QtPropertyBrowser/qtpropertybrowser.h>
#include <QMap>
#include <QString>
#include <QVariant>

class QGraphicsItem;
class QGraphicsItemGroup;

namespace te
{
  namespace layout
  {
    class Scene;
    class AbstractItemView;
    class EnumType;

    class ItemObserverManager : public QtAbstractPropertyManager
    {
      Q_OBJECT

      public:
        ItemObserverManager(Scene* scene, QObject *parent = 0);

        virtual ~ItemObserverManager();

        QString value(const QtProperty *property) const;

        QStringList names(const QtProperty *property) const;

        AbstractItemView* item(const QtProperty *property) const;

        virtual Property getProperty(QtProperty* qtProperty);

        virtual bool updateProperty(const Property& property);

        virtual QtProperty* findProperty(const std::string& propertyName, const std::string& parentClass);

        void setTypeForSearch(QtProperty *property, const EnumType* type);

        QString propertyLabel(const QtProperty *property) const;

        void setPropertyLabel(QtProperty *qtProperty, const QString &name, const QString &label, const te::layout::Property& property);

      public slots:

        void setValue(QtProperty *property, const QString &val);
        
      signals:

        void valueChanged(QtProperty *property, const QVariant &val);

        void listNamesChanged(QtProperty *property, const QStringList &names);

        void itemChanged(QtProperty *property, const AbstractItemView* item);

      protected:

        virtual QString valueText(const QtProperty *property) const;

        virtual void initializeProperty(QtProperty *property);

        virtual void uninitializeProperty(QtProperty *property);

        virtual AbstractItemView* findItem(const QString &name);

        virtual QStringList getItemNames(const EnumType* type);

      private:

        struct Data
        {
          QString           currentName;
          QStringList       names;
          AbstractItemView* item;
          EnumType*         typeSearchNames;
        };

        QMap<const QtProperty *, Data>  m_values;
        Scene*                          m_scene;
        QMap<QString, QString>          m_nameToLabel;
        QMap<QtProperty*, Property>     m_mapProperty;
    };

  }
}

#endif
