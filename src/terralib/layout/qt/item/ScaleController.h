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
  \file ScaleController.h
   
  \brief Class that represents scale controller.
  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SCALE_CONTROLLER1_H
#define __TERRALIB_LAYOUT_INTERNAL_SCALE_CONTROLLER1_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/property/Properties.h"

// STL
#include <string>

// Qt
#include <QString>

namespace te
{
  namespace layout
  {
    /*!
    \brief Class that represents scale controller.
    
    \ingroup layout
    \sa te::layout::AbstractItemController
    */
    class TELAYOUTEXPORT ScaleController : public AbstractItemController
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVM component
          \param o "Model" part of MVM component
        */ 
        ScaleController(AbstractItemModel* model);

        /*!
          \brief Destructor
        */ 
        virtual ~ScaleController();

        virtual void update(const Subject* subject);

        virtual double getCurrentUnit(std::string& strUnit);

        virtual double getUnitValue(std::string strUnit);

        virtual double getGap(double& initialGap, Font font, int numberOfBreaks = 0, double scaleWidthX = 0, std::string strUnit = "");

        virtual QPainterPath getLastText();

        virtual QPainterPath getLastTextByBreaks(int numberOfBreaks, Font font, double scaleWidthX = 0, std::string strUnit = "");

        virtual void setProperty(const Property& property);

        virtual void setProperties(const Properties& properties);

      protected:

        virtual Property checkScaleWidthAndUnit(const Properties& properties);

        virtual bool changeScaleWidthAfterConnection();

        virtual Property calculateScaleWidthInMM(const Properties& properties);

        virtual Property calculateScaleWidthInUnit(const Properties& properties);

        virtual Property calculateScaleUnit(const Properties& properties);

        virtual double strToUnit(std::string& strUnit);

        virtual Properties checkByBreaks(const Properties& properties);

        virtual double getFullWidthByBreaks(int numberOfBreaks, double scaleGapX, int scaleUnitGapX, Font font, std::string strUnit = "");
    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_SCALE_CONTROLLER1_H

