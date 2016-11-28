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
  \file GridSettingsEditor.h
   
   \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_EDITOR_H 
#define __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_EDITOR_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../../core/property/Property.h"
#include "../../../../core/Config.h"
#include "AbstractEditor.h"
#endif

// STL
#include <vector>

// Qt
#include <QWidget>

class QLabel;
class QToolButton;
class QEvent;
class QLayout;

namespace te
{
  namespace layout
  {
    class View;
    class GridSettingsOutside;

    /*!
    \brief 
    
    \ingroup layout
    */
    class TELAYOUTEXPORT GridSettingsEditor : public QWidget, public AbstractEditor
    {
      Q_OBJECT //for slots/signals

      public:

        GridSettingsEditor(const QModelIndex& index, QWidget* parent = 0);

        virtual ~GridSettingsEditor();
        
        virtual QVariant getValue();

        virtual bool	eventFilter(QObject * watched, QEvent * event);
        
      signals:

        void dataValueChanged(QWidget* widget, const Property& prop);

        void showDlg();

      protected slots:

        void onButtonClicked(bool checked = false);

        void onUpdateProperty(Property prop);

      protected:

        virtual void changeEditorData(const QModelIndex& index);

        virtual void createGroupBox();

        virtual void paintEvent(QPaintEvent * event);

        virtual void showSettings();

        void setupTreeViewEditorMargin(QLayout* layout);
        
      protected:

        QLabel*              m_textLabel;
        QToolButton*         m_button;
        GridSettingsOutside* m_gridSettings;
    };
  }
}

#endif

