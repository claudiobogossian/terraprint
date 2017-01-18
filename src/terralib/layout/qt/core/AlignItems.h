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
  \file AlignItems.h
   
  \brief Class applying the alignment in one or more components. Ex .: send to back, bring to front, align right, align bottom.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ALIGN_ITEMS_H
#define __TERRALIB_LAYOUT_INTERNAL_ALIGN_ITEMS_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/PaperConfig.h"

// STL
#include <map>

// Qt
#include <QRectF>
#include <QPointF>

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class Scene;
    /*!
    \brief Class applying the alignment in one or more components. Ex .: send to back, bring to front, align right, align bottom.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT AlignItems
    {
      public:

        /*!
          \brief Constructor
        */
        AlignItems(Scene* scene, PaperConfig* config);

        /*!
          \brief Destructor
        */
        virtual ~AlignItems();

        /*!
          \brief Move an item in front of several items.
        */
        virtual bool bringToFront(bool oneLevel = false);

        /*!
          \brief Move an item behind of several items.
        */
        virtual bool sendToBack(bool oneLevel = false);

        /*!
        \brief Change the ordering by one level to front.
        */
        virtual bool bringForward();

        /*!
        \brief Change the ordering by one level to back.
        */
        virtual bool sendBackward();

        /*!
          \brief
        */
        virtual void alignLeft();

        /*!
          \brief
        */
        virtual void alignRight();

        /*!
          \brief
        */
        virtual void alignTop();

        /*!
          \brief
        */
        virtual void alignBottom();

        /*!
          \brief
        */
        virtual void alignCenterHorizontal();

        /*!
          \brief
        */
        virtual void alignCenterVertical();

        /*!
          \brief

          \return
        */
        virtual QRectF getSelectionItemsBoundingBox();

        /*!
          \brief

          \return
        */
        int getMinimumZValue();

        /*!
          \brief  

          \param
        */
        void setMinimumZValue(int minimum);

      protected:

        virtual bool addChangePropertiesToUndoRedoStack(std::map<QGraphicsItem*, QPointF> items);

        virtual QGraphicsItem* searchForSubselection(QGraphicsItem* parentItem);

        Scene*          m_scene;
        PaperConfig*    m_config;
        int             m_minimunZValue;
    };
  }
}

#endif 
