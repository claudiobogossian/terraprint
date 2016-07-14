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
  \file terralib/layout/qt/core/pattern/factory/XmlTemplateFactory.h

  \brief This is the concrete factory for zoom area tools.
 */

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEMP_FILE_FACTORY_H
#define __TERRALIB_LAYOUT_INTERNAL_TEMP_FILE_FACTORY_H

// TerraLib
#include "../../../core/Config.h"
#include "TempDataStorageFactory.h"
#include "TempDataStorageFactoryParamCreate.h"

namespace te
{
  namespace layout
  {
    class AbstractTempDataStorage;
    /*!
      \class XmlTemplateFactory

      \brief This is the concrete factory for xml template.

      \sa TemplateFactory, ParameterizedAbstractFactory, AbstractTemplate
    */
    class TELAYOUTEXPORT TempFileFactory : public TempDataStorageFactory
    {
      public:

        /*! It register the factory in the abstract dictionary. */
        TempFileFactory();

        /*! \brief Destructor. */
        virtual ~TempFileFactory();

      protected:

        /*!
          \brief It creates a new xml template.

          \return It returns xml template.
        */
        AbstractTempDataStorage* build(TempDataStorageFactoryParamCreate params);
    };

  } // end namespace layout
}   // end namespace te


#endif  // __TERRALIB_LAYOUT_INTERNAL_XML_TEMPLATE_FACTORY_H
