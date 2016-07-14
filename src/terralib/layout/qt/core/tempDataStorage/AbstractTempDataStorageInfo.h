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
  \file TemplateEditor.h
   
  \brief Calls the factory to create a template of the specified type and keep. Responsible for delete the created template.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_TEMP_DATASTORAGE_INFO_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_TEMP_DATASTORAGE_INFO_H

// TerraLib
#include "../../../core/Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class EnumType;
    class Scene;
    /*!
      \brief Calls the factory to create a temp data storage of the specified type and keep. Responsible for delete the created temp data storage.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT AbstractTempDataStorageInfo
    {
      public:

        AbstractTempDataStorageInfo(Scene* scene);

        virtual ~AbstractTempDataStorageInfo();
        
        Scene* getScene();

      protected:

        Scene*      m_scene;
    };
  }
}

#endif
