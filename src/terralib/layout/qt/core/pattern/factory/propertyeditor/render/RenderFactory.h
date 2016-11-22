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
  \file terralib/layout/qt/pattern/factory/render/RenderFactory.h

  \brief This is the abstract factory for render.
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_RENDER_FACTORY_H 
#define __TERRALIB_LAYOUT_INTERNAL_RENDER_FACTORY_H

// TerraLib
#include <terralib/common/ParameterizedAbstractFactory.h>
#include "../../../../../../core/Config.h"
#include "../../../../propertyeditor/render/AbstractRender.h"
#include "RenderFactoryParamsCreate.h"

namespace te
{
  namespace layout
  {
    /*!
      \class RenderFactory

      \brief This is the abstract factory for renders.

      It will create objects of type AbstractEditor and will pass
      parameters of type EditorFactoryParamsCreate to their factories constructors.

      If you want a new tool you can use a command like:
      <code>
        te::layout::AbstractEditor* pEngine = te::layout::EditorFactory::make("FONTDIALOG");
      </code>
      Or
      <code>
        te::layout::EnumPropertyDataType* propDataType = Enums::getInstance().getEnumPropertyDataType();
        te::layout::EnumType* fontDialog = propDataType->getFontDialog();
        te::layout::AbstractEditor* pEngine = te::layout::ToolFactory::make(fontDialog->getName());
      </code>

      \note The caller of the method make will take the ownership of the returned tool.

      \sa AbstractRender, AbstractFactory
    */
    class TELAYOUTEXPORT RenderFactory : public te::common::ParameterizedAbstractFactory<AbstractRender, std::string, RenderFactoryParamsCreate>
    {
      public:

        /*! \brief Virtual destructor. */
        virtual ~RenderFactory();

      protected:

        /*!
        \brief It creates the factory.

        The key of a ToolFactory is a string.

        \param factoryKey The key that identifies the factory.
        */
        RenderFactory(const std::string& factoryKey);
    };
  }
}

#endif
