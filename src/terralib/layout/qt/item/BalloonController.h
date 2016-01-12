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

#ifndef __TERRALIB_BALLOON_CONTROLLER_H
#define __TERRALIB_BALLOON_CONTROLLER_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "TextController.h"

class QSizeF;

namespace te
{
  namespace layout
  {
    /*!
    \brief Class that represents text controller.

    \ingroup layout
    \sa te::layout::AbstractItemController
    */
    class TELAYOUTEXPORT BalloonController : public TextController
    {
    public:
      BalloonController(AbstractItemModel* model);
      bool sync();
      virtual QSizeF updateView();
      virtual void updateModel(const QSizeF& size);

    };

  }
}


#endif //__TERRALIB_BALLOON_CONTROLLER_H