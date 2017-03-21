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
  \file AddCommand.h
   
  \brief Undo/Redo for add one components.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ADD_COORD_COMMAND_H 
#define __TERRALIB_LAYOUT_INTERNAL_ADD_COORD_COMMAND_H

// TerraLib
#include "../../../../core/Config.h"
#include "../../../../core/enum/AbstractType.h"
#include <terralib/geometry/Point.h>

// Qt
#include <QUndoCommand>
#include <QString>

namespace te
{
  namespace layout
  {
    class View;
    /*!
      \brief Undo/Redo for add one components.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT AddCoordCommand : public QUndoCommand
    {
      public:

        /*!
          \brief Constructor

          \param item
          \param parent
        */
        AddCoordCommand(View* view, const std::vector<te::gm::Point>& coords, DrawGeometries type, QUndoCommand *parent = 0);

        /*!
          \brief Destructor
        */
        virtual ~AddCoordCommand();

        /*!
          \brief Reimplemented from QUndoCommand
        */
        virtual void undo();

        /*!
          \brief Reimplemented from QUndoCommand
        */
        virtual void redo();

      protected:

        virtual QString createCommandString(const std::vector<te::gm::Point>& coords);

      protected:

        View*                       m_view;
        std::vector<te::gm::Point>  m_coords;
        DrawGeometries              m_geometryType;
    };
  }
}

#endif

