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
  \file AbstractTempDataStorage.h
   
  \brief Abstract class that represents a temp data storage.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_TEMP_DATASTORAGE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_TEMP_DATASTORAGE_H

// TerraLib
#include "../../../core/Config.h"

namespace te
{
  namespace layout
  {
    class AbstractTempDataStorageInfo;
    /*!
      \brief Abstract class that represents a Template.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT AbstractTempDataStorage
    {
      public:

        /*
          \brief Constructor. 

          \param info information about the temporary data
        */
        AbstractTempDataStorage(AbstractTempDataStorageInfo* info);

        virtual ~AbstractTempDataStorage();

        AbstractTempDataStorageInfo* getInfo();

        virtual bool save() = 0;

        /*
          \brief Reading the properties of items that are in the scene.
            The reading of the properties of the items must be made before the use of multithreading. 
            One reason is that between the execution of the threads can have redraw and changes in Qt 
            graphical components via main thread.
        */
        virtual void readProperties() = 0;

      protected:

        AbstractTempDataStorageInfo* m_info;
    };
  }
}

#endif
