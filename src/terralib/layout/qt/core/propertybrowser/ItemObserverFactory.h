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
    \file ItemObserverFactory.h

    \brief

    \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_OBSERVER_FACTORY_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_OBSERVER_FACTORY_H

// TerraLib
#include <QtPropertyBrowser/qtpropertybrowser.h>
#include <QVariant>
#include "ItemObserverManager.h"

namespace te
{
  namespace layout
  {
    class ItemObserverProperty;

    class ItemObserverFactory : public QtAbstractEditorFactory<ItemObserverManager>
    {
      Q_OBJECT

      public:

        ItemObserverFactory(QObject *parent = 0);

        virtual ~ItemObserverFactory();

      protected:

        virtual void connectPropertyManager(ItemObserverManager *manager);

        virtual QWidget *createEditor(ItemObserverManager *manager, QtProperty *property,
          QWidget *parent);

        virtual void disconnectPropertyManager(ItemObserverManager *manager);

      private slots:

        void slotPropertyChanged(QtProperty *property, const QVariant &value);

        void slotListNamesChanged(QtProperty *property, const QStringList &names);

        void slotSetValue(const QString &value);

        void slotEditorDestroyed(QObject *object);

      private:

        QMap<QtProperty *, QList<ItemObserverProperty *> >  m_createdEditors;
        QMap<ItemObserverProperty *, QtProperty *>          m_editorToProperty;
    };
  }
}

#endif
