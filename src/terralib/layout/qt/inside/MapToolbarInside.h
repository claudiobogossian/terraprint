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
  \file MapToolbarInside.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_TOOLBAR_INSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_MAP_TOOLBAR_INSIDE_H

// TerraLib
#include "../../core/Config.h"
#include "ToolbarItemInside.h"

// Qt
#include <QObject>

class QToolBar;

namespace te
{
  namespace layout
  {
    class AbstractItemView;
    class EnumType;
    class MapToolbarController;

    /*!
    \brief 
    
      \ingroup layout

      \sa te::layout::AbstractItemView
    */
    class TELAYOUTEXPORT MapToolbarInside : public QObject, public ToolbarItemInside
    {
      Q_OBJECT //for slots/signals

    public:

      MapToolbarInside(MapToolbarController* controller, QObject* parent = 0);

      virtual ~MapToolbarInside();

      virtual void clear();

    protected:

      virtual void createToolbar();
      
      virtual QToolButton* createPanButton();

      virtual QToolButton* createZoomInButton();

      virtual QToolButton* createZoomOutButton();

      virtual QToolButton* createSystematicScaleButton();

      virtual QToolButton* createTextGridAsObjectButton();

      virtual QToolButton* createTextMapAsObjectButton();

      virtual QToolButton* createLegendAsObjectButton();

    protected:

      MapToolbarController*  m_controller;
    };
  }
}

#endif

