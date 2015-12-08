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
  \file terralib_layout/qt/core/tools/PanMapTool.h

  \brief This class implements a concrete tool to geographic pan operation.
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PAN_MAP_TOOL_H
#define __TERRALIB_LAYOUT_INTERNAL_PAN_MAP_TOOL_H

// TerraLib
#include "../../../core/Config.h"
#include "terralib/qt/widgets/tools/Pan.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class MapDisplay;
    }
  }
  namespace layout
  {
    /*!
    \class PanMapTool

    \brief This class implements a concrete tool to geographic pan operation.

    \ingroup widgets
    */
    class TELAYOUTEXPORT PanMapTool : public te::qt::widgets::Pan
    {
    public:

      /*
      *\brief
      */
      PanMapTool(te::qt::widgets::MapDisplay* display, const QCursor& cursor, const QCursor& actionCursor = Qt::BlankCursor, QObject* parent = 0);

      /*! \brief Destructor. */
      ~PanMapTool();

      bool mousePressEvent(QMouseEvent* e);

      bool mouseReleaseEvent(QMouseEvent* e);

    };
  }   
}     

#endif  // __TERRALIB_LAYOUT_INTERNAL_PAN_MAP_TOOL_H
