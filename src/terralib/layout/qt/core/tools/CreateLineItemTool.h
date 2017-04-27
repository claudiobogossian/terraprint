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
  \file terralib/layout/qt/core/tools/CreateLineItemTool.h

  \brief This class implements a concrete tool to create line item. Drawing lines and only create the item and update with the geometry at the end.
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_CREATE_LINE_ITEM_TOOL_H
#define __TERRALIB_LAYOUT_INTERNAL_CREATE_LINE_ITEM_TOOL_H

//TerraLib
#include "AbstractLayoutTool.h"
#include "terralib/geometry/Point.h"
#include "terralib/geometry/Coord2D.h"

//STL
#include <vector>

//Qt
#include <QGraphicsItem>

class QPainterPath;
class QPointF;
class QPolygonF;
class QPainter;
class QPaintDevice;

namespace te
{
  namespace layout
  {
// Forward declarations
    class View;
    class EnumType;
    class Properties;
    class RenderGeometries;

      /*!
        \class CreateLineItemTool

        \brief This class implements a concrete tool to create line item. Drawing lines and only create the item and update with the geometry at the end.
      */
      class TELAYOUTEXPORT CreateLineItemTool : public AbstractLayoutTool
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a create line item tool associate with a View.

            \param view The tool's view.
            \param parent The tool's parent..
          */
          CreateLineItemTool(View* view, EnumType* itemType = 0, QObject* parent = 0);
          
          /*! \brief Destructor. */
          ~CreateLineItemTool();

          virtual void init();

          //@}

          /** @name AbstractLayoutTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool mousePressEvent(QMouseEvent* e);

          void createItem();

          bool mouseMoveEvent(QMouseEvent* e);

          bool mouseReleaseEvent(QMouseEvent* e);

          bool keyPressEvent(QKeyEvent* e);

          virtual void redraw();

          //@}
        protected:

          virtual void setGeometry();
          
          virtual Properties createProperties(te::gm::LineString* lineString);

          virtual Properties createProperties(const te::gm::Coord2D& coord, double width, double height);

          virtual void finalizeCreation();

        protected:

          RenderGeometries*           m_render;
          std::vector<te::gm::Point>  m_coords;
          QGraphicsItem*              m_item;
          EnumType*                   m_itemType;
      };

  }   // end namespace layout
}     // end namespace te

#endif  // __TERRALIB_LAYOUT_INTERNAL_CREATE_LINE_ITEM_TOOL_H
