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
    \file ItemObserverProperty.h

    \brief 

    \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_OBSERVER_PROPERTY_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_OBSERVER_PROPERTY_H

// Qt
#include <QWidget>
#include <QString>
#include <QStringList>

class QComboBox;

namespace te
{
  namespace layout
  {
    class ItemObserverProperty : public QWidget
    {
      Q_OBJECT

      public:

        ItemObserverProperty(QWidget *parent = 0);

        virtual ~ItemObserverProperty();

        void setCurrentName(const QString &name);

        QString currentName() const;

        void setNameList(const QStringList & names);
        
      signals:

          void currentNameChanged(const QString &name);

      protected:

        void focusInEvent(QFocusEvent *e);

        void focusOutEvent(QFocusEvent *e);

        void keyPressEvent(QKeyEvent *e);

        void keyReleaseEvent(QKeyEvent *e);
        
      private:
        
        QComboBox* m_cbNames;
    };

  }
}

#endif
