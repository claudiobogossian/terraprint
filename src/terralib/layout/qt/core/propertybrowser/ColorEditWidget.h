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
 
#ifndef __TERRALIB_LAYOUT_INTERNAL_COLOREDITWIDGET_H
#define __TERRALIB_LAYOUT_INTERNAL_COLOREDITWIDGET_H
 
#include <QColor> 
#include <QString>
#include <QWidget>

class QBrush;
class QLabel;
class QLayout;
class QToolButton;
 
namespace te
{
  namespace layout
  {
    class ColorEditWidget : public QWidget 
    {
      Q_OBJECT

      public:
        ColorEditWidget(QWidget *parent);

        void setTitle(const QString& title);

        bool eventFilter(QObject *obj, QEvent *ev);

      public Q_SLOTS:
        void setValue(const QColor &value);

      Q_SIGNALS:
        void valueChanged(const QColor &value);

      protected:
        void paintEvent(QPaintEvent *);

        QPixmap brushValuePixmap(const QBrush &b);
        QString colorValueText(const QColor &c);
        void setupTreeViewEditorMargin(QLayout *lt);

      private Q_SLOTS:
        void buttonClicked();

      private:
        QColor m_color;
        QLabel *m_pixmapLabel;
        QLabel *m_label;
        QToolButton *m_button;
        QString m_title;
    };
  }
}

#endif // __TERRALIB_LAYOUT_INTERNAL_COLOREDITWIDGET_H
