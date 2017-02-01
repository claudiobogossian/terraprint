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
  \file TempFile.h
   
  \brief Calls the factory to create a temp file of the specified type and keep. Responsible for delete the created temp file.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEMP_FILE_H 
#define __TERRALIB_LAYOUT_INTERNAL_TEMP_FILE_H

// TerraLib
#include "AbstractTempDataStorage.h"
#include "../../../core/Config.h"
#include "../../../core/property/Properties.h"

// Boost
#ifndef Q_MOC_RUN
#include <boost/thread.hpp>
#endif

// STL
#include <string>

namespace te
{
  namespace layout
  {
    /*!
      \brief Calls the factory to create a template of the specified type and keep. Responsible for delete the created template.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT TempFile: public AbstractTempDataStorage
    {
      public:

        explicit TempFile(AbstractTempDataStorageInfo* info);

        virtual ~TempFile();

        /*!
          \brief Implemented from AbstractTempDataStorage
        */
        virtual bool save();

        /*!
          \brief Implemented from AbstractTempDataStorage
        */
        virtual void readProperties();

        bool deleteDataStorage();

      protected:

        std::vector<te::layout::Properties>                 m_properties;
        std::map< std::string, std::vector<std::string> >   m_mapGroups;
        boost::mutex                                        m_mutex; //!< A mutex to lock the access
    };
  }
}

#endif
