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
  \file ColorEditWidget.h
   
   \brief Class 99% based on QtColorEditWidget, from Property Browser.  This was done because for some reason it is not possible to extend this class.

  \ingroup layout
*/
 
#ifndef __TERRALIB_LAYOUT_INTERNAL_COLOREDITOTFACTORY_H
#define __TERRALIB_LAYOUT_INTERNAL_COLOREDITOTFACTORY_H

//Qt includes
#include <QList>
#include <QMap>
#include <QWidget>

#include <QtPropertyBrowser/qteditorfactory.h>
#include <QtPropertyBrowser/qtpropertymanager.h>

namespace te
{
  namespace layout
  {
    class ColorEditWidget;

    class ColorEditorFactory : public QtAbstractEditorFactory<QtColorPropertyManager>
    {
      Q_OBJECT
      public:
        ColorEditorFactory(QObject *parent = 0);
        ~ColorEditorFactory();
      protected:
        void connectPropertyManager(QtColorPropertyManager *manager);
        QWidget *createEditor(QtColorPropertyManager *manager, QtProperty *property, QWidget *parent);
        void disconnectPropertyManager(QtColorPropertyManager *manager);

      private slots :
          void slotPropertyChanged(QtProperty* property, const QColor& value);
          void slotEditorDestroyed(QObject* object);
          void slotSetValue(const QColor& value);

      private:

        QMap<QtProperty *, QList<ColorEditWidget*> >  m_createdEditors;
        QMap<ColorEditWidget *, QtProperty *>         m_editorToProperty;
    };
  }
}

#endif // __TERRALIB_LAYOUT_INTERNAL_COLOREDITOTFACTORY_H
