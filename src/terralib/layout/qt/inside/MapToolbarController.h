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
  \file ToolbarItemController.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_TOOLBAR_CONTROLLER_H_H 
#define __TERRALIB_LAYOUT_INTERNAL_MAP_TOOLBAR_CONTROLLER_H_H

// TerraLib
#include "../../core/Config.h"

// Qt
#include <QObject>

class QAction;

namespace te
{
  namespace layout
  {
    class MapToolbarInside;
    class EnumType;

    class TELAYOUTEXPORT MapToolbarController : public QObject
    {
      Q_OBJECT //for slots/signals

      public:

        MapToolbarController();

        virtual ~MapToolbarController();

        void setMapToolbarInside(MapToolbarInside* toolbarInside);

      public slots:

        virtual void onPanToolClicked(bool checked);

        virtual void onZoomInClicked(bool checked);

        virtual void onZoomOutClicked(bool checked);

        virtual void onSystematicScaleClicked(bool checked);

        virtual void onTextGridAsObjectClicked(bool checked);

        virtual void onTextMapAsObjectClicked(bool checked);

        virtual void onLegendAsObjectClicked(bool checked);

      protected:

        void changeMapTool(EnumType* tool);

        MapToolbarInside* m_toolbarItem;
    };
  }
}

#endif

