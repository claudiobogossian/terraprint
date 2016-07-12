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
  \file terralib/layout/core/tempFile/TemplateFactory.h

  \brief This is the abstract factory for tools.
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEMP_DATASTORAGE_FACTORY_H 
#define __TERRALIB_LAYOUT_INTERNAL_TEMP_DATASTORAGE_FACTORY_H

// TerraLib
#include "terralib/common/ParameterizedAbstractFactory.h"
#include "../../../core/Config.h"
#include "TempDataStorageFactoryParamCreate.h"
#include "AbstractTempDataStorage.h"

namespace te
{
  namespace layout
  {
    /*!
      \class TempDataStorageFactory

      \brief This is the abstract factory for templates.

      It will create objects of type AbstractTemplate and will pass
      parameters of type TempDataStorageFactoryParamsCreate to their factories constructors.

      If you want a new tool you can use a command like:
      <code>
        te::layout::AbstractTemplate* pEngine = te::layout::TemplateFactory::make("XML");
      </code>
      Or
      <code>
        te::layout::EnumTemplateType* tool = Enums::getInstance().getEnumTemplateType();
        te::layout::EnumType* template = tool->getToolTypeXml();
        te::layout::AbstractTempDataStorage* pEngine = te::layout::TemplateFactory::make(template->getName());
      </code>

      \note The caller of the method make will take the ownership of the returned tool.

      \sa AbstractTemplate, AbstractFactory
    */
    class TELAYOUTEXPORT TempDataStorageFactory : public te::common::ParameterizedAbstractFactory<AbstractTempDataStorage, std::string, TempDataStorageFactoryParamCreate>
    {
      public:

        /*! \brief Virtual destructor. */
        virtual ~TempDataStorageFactory();

      protected:

        /*!
        \brief It creates the factory.

        The key of a ToolFactory is a string.

        \param factoryKey The key that identifies the factory.
        */
        TempDataStorageFactory(const std::string& factoryKey);
    };
  }
}

#endif
