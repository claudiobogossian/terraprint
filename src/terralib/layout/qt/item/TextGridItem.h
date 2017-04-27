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
  \file TextGridItem.h
   
  \brief Class daughter of te::layout::TitleItem representing a grid with cells which can be inserted texts.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_GRID_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_GRID_ITEM_H

// TerraLib
#include "TextItem.h"
#include "../../core/Config.h"

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    class AbstractItemModel;

    /*!
      \brief Class that represents a matrix of texts
    
      \ingroup layout

      \sa te::layout::TitleItem
    */
    class TELAYOUTEXPORT TextGridItem : public TextItem
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        TextGridItem(te::layout::ItemInputProxy* itemInputProxy);

        /*!
          \brief Destructor
         */
        virtual ~TextGridItem();

        /*!
        \brief Reimplemented from QGraphicsTextItem
        */
        virtual void keyPressEvent(QKeyEvent * event);

        void documentEditionFinished();

        virtual void updateBlockEditionRange();

      protected:

        virtual AbstractItemModel* createModel() const;

        virtual AbstractItemController* createController() const;
    };
  }
}

#endif
