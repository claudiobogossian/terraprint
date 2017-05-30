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
  \file AddCommand.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AddCoordCommand.h"
#include "../../View.h"
#include "../../RenderGeometries.h"
#include "../../tools/CreateLineItemTool.h"

// STL
#include <algorithm>

// Qt
#include <QPixmap>

te::layout::AddCoordCommand::AddCoordCommand(View* view, const std::vector<te::gm::Point>& oldCoords, const std::vector<te::gm::Point>& newCoords, QUndoCommand *parent) :
  QUndoCommand(parent),
  m_view(view),
  m_coords(newCoords),
  m_oldCoords(oldCoords)
{
  setText(QObject::tr("Add %1").arg(createCommandString(newCoords)));
}

te::layout::AddCoordCommand::~AddCoordCommand()
{
  
}

void te::layout::AddCoordCommand::undo()
{
  changeCoords(m_oldCoords);
}

void te::layout::AddCoordCommand::redo()
{
  changeCoords(m_coords);
}

void te::layout::AddCoordCommand::changeCoords(const std::vector<te::gm::Point>& coords)
{
  // Undo/Redo of the line/polygon creation tool.

  if (!m_view)
    return;

  QPixmap* draftPixmap = m_view->getDraftPixmap();
  if (draftPixmap)
  {
    AbstractLayoutTool* tool = m_view->getCurrentTool();
    if (tool)
    {
      CreateLineItemTool* lineTool = dynamic_cast<CreateLineItemTool*>(tool);
      if (lineTool)
      {
        lineTool->setCoords(coords);
        /* When the tools need to draw in the pixmap, this function must be called,
        so the View itself will call the update() method and redraw() method of the current tool. */
        m_view->makeDraftPixmapDirty(true);
      }
    }
  }
}

QString te::layout::AddCoordCommand::createCommandString(const std::vector<te::gm::Point>& coords)
{  
  QString qName = QString::number(coords.size());
  return QObject::tr("%1")
    .arg(qName);
}
