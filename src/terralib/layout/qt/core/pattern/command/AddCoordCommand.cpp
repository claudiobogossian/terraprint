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

// Qt
#include <QPixmap>

te::layout::AddCoordCommand::AddCoordCommand(View* view, const std::vector<te::gm::Point>& coords, DrawGeometries type, QUndoCommand *parent) :
  QUndoCommand(parent),
  m_view(view),
  m_coords(coords),
  m_geometryType(type)
{
  setText(QObject::tr("Add %1").arg(createCommandString(coords)));
}

te::layout::AddCoordCommand::~AddCoordCommand()
{
  
}

void te::layout::AddCoordCommand::undo()
{
  if (!m_view)
    return;

  QPixmap* draftPixmap = m_view->getDraftPixmap();
  if (draftPixmap)
  {
    draftPixmap->fill(Qt::transparent);
    RenderGeometries render(m_view, m_geometryType);
    render.drawGeometries(draftPixmap, m_coords);
  }
}

void te::layout::AddCoordCommand::redo()
{
  if (!m_view)
    return;

  QPixmap* draftPixmap = m_view->getDraftPixmap();
  if (draftPixmap)
  {
    draftPixmap->fill(Qt::transparent);
    RenderGeometries render(m_view, m_geometryType);
    render.drawGeometries(draftPixmap, m_coords);
  }
}

QString te::layout::AddCoordCommand::createCommandString(const std::vector<te::gm::Point>& coords)
{  
  QString qName = QString::number(coords.size());
  return QObject::tr("%1")
    .arg(qName);
}
