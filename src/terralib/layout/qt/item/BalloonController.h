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
  \file BalloonController.h

  \brief Class that represents balloon controller.
  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_BALLOON_CONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_BALLOON_CONTROLLER_H

// TerraLib
#include "../../core/Config.h"
#include "TextController.h"

class QSizeF;

namespace te
{
  namespace layout
  {
    /*!
      \brief Class that represents balloon controller.

      \ingroup layout
      \sa te::layout::TextController
    */
    class TELAYOUTEXPORT BalloonController : public TextController
    {
    public:

      /*!
      \brief Constructor
      */
      BalloonController(AbstractItemModel* model, AbstractItemView* view);

      /*!
      \brief Destructor
      */
      virtual ~BalloonController();

      virtual void calculateSize(const te::layout::Properties& properties, QSizeF& sizeMM, double& dx, double& dy);

      virtual bool needUpdateBox(const te::layout::Properties& properties);

      /*!
      \brief Sets the new values of the given properties
      */
      virtual void setProperties(const Properties& properties);
    };

  }
}


#endif //__TERRALIB_LAYOUT_INTERNAL_BALLOON_CONTROLLER_H