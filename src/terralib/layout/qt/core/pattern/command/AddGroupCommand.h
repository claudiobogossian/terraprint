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
   
  \brief Undo/Redo for add group components. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ADD_GROUP_COMMAND_H 
#define __TERRALIB_LAYOUT_INTERNAL_ADD_GROUP_COMMAND_H

// TerraLib
#include "../../../../core/Config.h"
#include "../../../../core/property/Properties.h"

// STL
#include <string>
#include <vector>

// Qt
#include <QUndoCommand>
#include <QString>
#include <QPointF>

class QGraphicsItem;
class QGraphicsScene;

namespace te
{
  namespace layout
  {
    class Properties;
    class EnumType;
    class Scene;
    class ItemGroup;
    /*!
      \brief Undo/Redo for add one components.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT AddGroupCommand : public QUndoCommand
    {
      public:

        /*!
          \brief Constructor

          \param item
          \param parent
        */
        AddGroupCommand(QGraphicsItem* item, QUndoCommand *parent = 0);

        /*!
          \brief Destructor
        */
        virtual ~AddGroupCommand();

        /*!
          \brief Reimplemented from QUndoCommand
        */
        virtual void undo();

        /*!
          \brief Reimplemented from QUndoCommand
        */
        virtual void redo();

      protected:

        virtual void init(QGraphicsItem* item);

        virtual QString createCommandString();

        QList<QGraphicsItem *> childrenItems();
        
      protected:
      
        Scene*                    m_scene;
        QGraphicsItem*            m_group;
        Properties                m_groupProperties;
        std::vector<std::string>  m_childrenNames;
    };
  }
}

#endif


