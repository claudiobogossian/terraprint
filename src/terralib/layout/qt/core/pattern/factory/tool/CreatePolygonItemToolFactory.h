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
  \file terralib/layout/qt/core/pattern/factory/CreatePolygonItemToolFactory.h

  \brief This is the concrete factory for zoom area tools.
 */

#ifndef __TERRALIB_LAYOUT_INTERNAL_CREATE_POLYGON_ITEM_TOOL_FACTORY_H
#define __TERRALIB_LAYOUT_INTERNAL_CREATE_POLYGON_ITEM_TOOL_FACTORY_H

// TerraLib
#include "../../../../../core/Config.h"
#include "ToolFactory.h"
#include "ToolFactoryParamsCreate.h"

namespace te
{
  namespace layout
  {
    class AbstractLayoutTool;
    /*!
      \class CreatePolygonItemToolFactory

      \brief This is the concrete factory for create polygon tool.

      \sa ToolFactory, ParameterizedAbstractFactory, AbstractLayoutTool
    */
    class TELAYOUTEXPORT CreatePolygonItemToolFactory : public ToolFactory
    {
      public:

        /*! It register the factory in the abstract dictionary. */
        CreatePolygonItemToolFactory();

        /*! \brief Destructor. */
        ~CreatePolygonItemToolFactory();

      protected:

        /*!
          \brief It creates a new create polygon item tool.

          \return It returns create polygon item tool.
        */
        AbstractLayoutTool* build(ToolFactoryParamsCreate params);
    };

  } // end namespace layout
}   // end namespace te


#endif  // __TERRALIB_LAYOUT_INTERNAL_CREATE_POLYGON_ITEM_TOOL_FACTORY_H

