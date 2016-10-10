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
  \file TextController.h
   
  \brief Class that represents text controller.
  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_CONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_CONTROLLER_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractItemController.h"

namespace te
{
  namespace layout
  {
    /*!
    \brief Class that represents text controller.
    
    \ingroup layout
    \sa te::layout::AbstractItemController
    */
    class TELAYOUTEXPORT TextController : public AbstractItemController
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        TextController(AbstractItemModel* model);

        /*!
          \brief Destructor
        */ 
        virtual ~TextController();

        /*!
        \brief Sets the new value of the given property
        */
        virtual void setProperty(const Property& property);

        /*!
        \brief Sets the new values of the given properties
        */
        virtual void setProperties(const Properties& properties);

        virtual double getDpiForCalculation() const;

        virtual void calculateSize(const te::layout::Properties& properties, QSizeF& sizeMM, double& dx, double& dy);

        virtual bool needUpdateBox(const te::layout::Properties& properties);

    protected:

        double m_dpiForCalculation; //!< Holds the information of the DPI used to calculate the bounding rect (in MM) of the item

    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_TEXT_CONTROLLER_H
