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
  \file MapCompositionController.h
   
  \brief Class that represents associate item group controller.
  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_COMPOSITION_CONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_COMPOSITION_CONTROLLER_H

// TerraLib
#include "../../core/Config.h"
#include "ItemGroupController.h"
#include "../../core/property/Properties.h"

namespace te
{
  namespace layout
  {
    class AbstractItemModel;
    class AbstractItemView;
    /*!
    \brief Class that represents associate item group controller.
    
    \ingroup layout
    \sa te::layout::AbstractItemController
    */
    class TELAYOUTEXPORT MapCompositionController : public ItemGroupController
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        MapCompositionController(AbstractItemModel* model, AbstractItemView* view);

        /*!
          \brief Destructor
        */ 
        virtual ~MapCompositionController();

        virtual const Properties& getProperties() const;

        virtual void setProperties(const Properties& properties);

        /*!
        \brief Method called by the subject to inform changes in the model
        */
        virtual void update(const Subject* subject);

        virtual std::map<std::string, te::layout::Properties> groupPropertiesByParent(const Properties& properties);

        virtual te::layout::Properties expandHiddenProperties(const Properties& properties);

        virtual void hideProperties(Property& property) const;

        virtual void syncItemSize(std::map<std::string, te::layout::Properties>& mapProperties);

        virtual void updateBoundingRect(QRectF rect);

      protected: 

        mutable te::layout::Properties m_propertiesFullReference;
        bool                           m_ignoreResize;
    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_MAP_COMPOSITION_CONTROLLER_H
