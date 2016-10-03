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
  \file Properties.h
   
  \brief The Properties class represents a persistent set of properties. The Properties can be saved to a file (Ex.: .json) or loaded from a file (Ex.: .json).
    Also used for interaction, with user or other classes of this module, to change state of a MVC Component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_H
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_H

//Terralib
#include "Property.h"
#include "../enum/AbstractType.h"
#include "../Config.h"
#include "../enum/EnumType.h"

//STL
#include <vector>
#include <algorithm>

namespace te
{
  namespace layout
  {
    /*!
      \brief The Properties class represents a persistent set of properties. The Properties can be saved to a file (Ex.: .json) or loaded from a file (Ex.: .json).
        Also used for interaction, with user or other classes of this module, to change state of a MVC Component.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT Properties
    {
      public:

        /*!
        \brief Constructor
        */
        Properties();

        /*!
          \brief Constructor

          \param objectName Object name that owns these properties
          \param type Object type that owns these properties
        */ 
        Properties(const std::string& objectName, EnumType* type = 0, int hashCode = 0);

        /*!
          \brief Destructor
        */
        virtual ~Properties(void);

        /*!
          \brief Adds the specified property to the set of properties for this object.

          \param property specified property
          \return true if add, false otherwise 
        */
        virtual bool addProperty(const Property& property);

        /*!
        \brief Adds the specified sub property to the set of properties for the parent property of this object.

        \param property specified parent property
        \param subProperty specified subproperty
        \return true if add, false otherwise
        */
        virtual bool addSubProperty(const Property& parent, const Property& subProperty);

        /*!
          \brief Removes a property from the set of properties of this object.

          \param property specified property
          \return true if remove, false otherwise 
        */
        virtual bool removeProperty(const std::string& name);

        /*!
          \brief Updates the value of the given property name.

          \param property the property to be updated
          \return true if remove, false otherwise 
        */
        virtual bool updateProperty(const Property& property);

        virtual bool completelyUpdateProperty(const Property& property);

        /*!
          \brief Clear set of properties of this object.

          \return true if clear, false otherwise
        */
        virtual bool clear();

        /*!
          \brief Returns set of all properties

          \return set of all properties
        */
        virtual const std::vector<Property>& getProperties() const;

        /*!
          \brief Returns object name that owns these properties.

          \return object name that owns these properties
        */
        virtual const std::string getObjectName() const;

        /*!
          \brief Sets object name that owns these properties.

          \param object name that owns these properties
        */
        virtual void setObjectName(const std::string& name);

        /*!
          \brief Returns object type that owns these properties.

          \return object type that owns these properties
        */
        virtual EnumType* getTypeObj() const;

        /*!
          \brief Sets object type that owns these properties.

          \param object type that owns these properties
        */
        virtual void setTypeObj(EnumType* type);

        /*!
          \brief 

          \param 
        */
        virtual void setHasWindows(bool windows);

        /*!
          \brief 

          \return 
        */
        virtual bool hasWindows() const;

        /*!
          \brief Checks if the property is contained within the set of properties.

          \param property
          \return true if contained, false otherwise
        */
        virtual bool contains(const Property& property) const;

        /*!
          \brief Checks if the property is contained within the set of properties.

          \param propertyName
          \return true if contained, false otherwise
        */
        virtual bool contains( const std::string& propertyName ) const;

        /*!
          \brief Checks if the name is contained within the set of properties.

          \param name name of the property
          \return true if contained, false otherwise
        */
        virtual const Property& getProperty( const std::string& name ) const;

        /*!
          \brief Returns the hashcode of a MVC component.

          \return hashCode
        */
        virtual int getHashCode();

        /*!
          \brief Sets the hashcode of a MVC component.

          \return hashCode
        */
        virtual void setHashCode(int hashCode);

        virtual void reparentProperties(const std::string& parentClass);

      protected:

        std::vector<Property> m_properties; //!< set of properties for this object
        std::string m_objName; //!< Object name that owns these properties
        EnumType* m_typeObj; //!< Object type that owns these properties
        bool m_hasWindows; //!<
        int  m_hashcode;
        Property m_nullProperty; //!< Represents a null property
    };

  }
}

#endif
