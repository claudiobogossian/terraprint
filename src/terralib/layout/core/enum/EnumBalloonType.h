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



#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_BALLOON_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_BALLOON_TYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    class EnumType;

    /*!
      \brief Class to represent a scale star enumeration.
    
      \ingroup layout

      \sa te::layout::AbstractEnum
    */
    class TELAYOUTEXPORT EnumBalloonType : public AbstractEnum
    {
      public:

        /*!
          \brief Constructor
        */
        EnumBalloonType();

        /*!
          \brief Destructor
        */ 
        virtual ~EnumBalloonType();

        /*!
          \brief Returns value that represents none type belonging to enumeration.
      
          \return enum value      
        */
        virtual EnumType* getNoneType() const;
        
        /*!
          \brief Returns value that represents rectangle balloon belonging to enumeration.
      
          \return enum value      
        */
        virtual EnumType* getRectangleBalloon() const;

        /*!
          \brief Returns value that represents rounded rectangle balloon belonging to enumeration.
      
          \return enum value      
        */
        virtual EnumType* getRoundedRectangleBalloon() const;

        /*!
          \brief Returns value that represents Ellipse Balloon belonging to enumeration.
      
          \return enum value      
        */
        virtual EnumType* getEllipseBalloon() const;
    
      protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:

        EnumType* m_noneType; //!< value that represents none type belonging to enumeration
        EnumType* m_balloon1; //!< value that represents rectangle balloon belonging to enumeration
        EnumType* m_balloon2; //!< value that represents rounded rectangle balloon belonging to enumeration
        EnumType* m_balloon3; //!< value that represents Ellipse balloon belonging to enumeration
 
    };
  }
}

#endif
