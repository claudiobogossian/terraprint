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
  \file ScaleItem.h
   
   \brief Class that represents a graphic scale of a map.
   Its coordinate system is the same of scene (millimeters). 
   This is also the son of AbstractItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SCALE_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_SCALE_ITEM_H

// TerraLib
#ifndef Q_MOC_RUN
#include "AbstractItem.h"
#endif
#include "../../core/Config.h"
#include "../../core/Font.h"

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    class AbstractItemModel;
    /*!
    \brief Class that represents a graphic scale of a map.
        Its coordinate system is the same of scene (millimeters). 
        This is also the son of AbstractItem, so it can become observer of a model (Observable).  
    
      \ingroup layout

      \sa te::layout::AbstractItem
    */
    class TELAYOUTEXPORT ScaleItem : public AbstractItem
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        ScaleItem(te::layout::ItemInputProxy* itemInputProxy);

        /*!
          \brief Destructor
         */
        virtual ~ScaleItem();

        virtual void refreshScaleProperties();

      protected:

        virtual AbstractItemModel* createModel() const;

        virtual AbstractItemController* createController() const;

        virtual void drawItem ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        virtual void drawBreaks(QPainter * painter, int numberOfBreaks, double initialX, double initialY, double width, double height);

        virtual void drawTexts(QPainter * painter, int numberOfBreaks, double initialX, double initialY, double width, double height);

        /*!
        \brief Implemented from AbstractItem.
        */
        virtual bool isLimitExceeded(QRectF resizeRect);

        virtual bool validateGaps();

        /*
        \brief Disable all sides of resize.

        \param if true resize will be apply, false otherwise
        */
        void setResize(bool resize);

      protected:

        double  m_scale;
        double  m_gapX;
        double  m_gapY;
        Font    m_font;
        double  m_scaleUnitGapX;
        int     m_numberOfBreaks;
    };
  }
}

#endif
