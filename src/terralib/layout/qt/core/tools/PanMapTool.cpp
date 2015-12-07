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
  \file terralib_layout/qt/core/tools/PanMapTool.cpp

  \brief This class implements a concrete tool to geographic pan operation.
*/

// TerraLib
#include "PanMapTool.h"
#include "terralib/qt/widgets/canvas/MapDisplay.h"

// Qt
#include <QMouseEvent>

te::layout::PanMapTool::PanMapTool(te::qt::widgets::MapDisplay* display, const QCursor& cursor, const QCursor& actionCursor, QObject* parent)
  : te::qt::widgets::Pan(display, cursor, actionCursor, parent)
{
  
}

te::layout::PanMapTool::~PanMapTool()
{
}

bool te::layout::PanMapTool::mousePressEvent(QMouseEvent* e)
{
  if (e->button() != Qt::MidButton)
    return false;

  /* The pan is made by default with the left mouse button (MapDisplay),
  so we need to resubmit the event, as if this button had been clicked */
  QMouseEvent resubmit(e->type(), e->pos(), Qt::LeftButton, Qt::LeftButton, e->modifiers());
  te::qt::widgets::Pan::mousePressEvent(&resubmit);

  return true;
}

bool te::layout::PanMapTool::mouseReleaseEvent(QMouseEvent* e)
{
  if (e->button() != Qt::MidButton)
    return false;

  /* The pan is made by default with the left mouse button (MapDisplay),
  so we need to resubmit the event, as if this button had been clicked */
  QMouseEvent resubmit(e->type(), e->pos(), Qt::LeftButton, Qt::LeftButton, e->modifiers());
  te::qt::widgets::Pan::mouseReleaseEvent(&resubmit);

  return true;
}

