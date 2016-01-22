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
  \file ToolbarItemInside.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TOOLBAR_INSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_TOOLBAR_INSIDE_H

// TerraLib
#include "../../core/Config.h"

// STL
#include <string>

// Qt
#include <QString>

class QToolBar;
class QToolButton;
class QPushButton;
class QAction;
class QWidget;

namespace te
{
  namespace layout
  {
    class AbstractItemView;
    class EnumType;

    /*!
    \brief 
    
      \ingroup layout

      \sa te::layout::AbstractItemView
    */
    class TELAYOUTEXPORT ToolbarItemInside
    {

    public:

      ToolbarItemInside();

      virtual ~ToolbarItemInside();

      EnumType* getItemType();

      AbstractItemView* getItem();

      void setItem(AbstractItemView* item);

      QToolBar* getToolbar();

      virtual void clear() = 0;
   
    protected:

      virtual QToolButton* createToolButton(const QString& text, const QString& tooltip, const QString& icon);

      virtual QPushButton* createPushButton(const QString& text, const QString& tooltip, const QString& icon);

      virtual QAction* createAction(const QString& text, const QString& objName, const QString& icon, const QString& tooltip = "", QWidget* parent = 0);

      AbstractItemView*  m_item;
      QToolBar*          m_toolbar;

    };
  }
}

#endif
