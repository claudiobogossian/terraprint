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
  \file BuildGraphicsItem.h
   
  \brief Class responsible for creating or building graphics objects. All objects are children of QGraphicsItem and ItemObserver.
    An object of a type is created from a coordinated. Also an object of a type can be built from the properties saved in a template.
    Only place where the model and the controller is instantiated and the MVC component is created.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_BUILD_GRAPHICS_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_BUILD_GRAPHICS_ITEM_H

//TerraLib
#ifndef Q_MOC_RUN
#include "AbstractBuildGraphicsItem.h"
#include "../../core/property/Properties.h"
#include "../../core/Config.h"
#include "pattern/factory/item/ItemFactoryParamsCreate.h"
#endif

#include <QObject>

class QGraphicsItem;
class QPointF;

namespace te
{
  namespace layout
  {
    class Scene;
  /*!
  \brief Class responsible for creating or building graphics objects. All objects are children of QGraphicsItem and ItemObserver.
    An object of a type is created from a coordinated. Also an object of a type can be built from the properties saved in a template.
    Only place where the model and the controller is instantiated and the MVC component is created.
    
    \ingroup layout

    \sa te::layout::AbstractBuildGraphicsItem
  */
    class TELAYOUTEXPORT BuildGraphicsItem : public QObject, public AbstractBuildGraphicsItem
    {
      Q_OBJECT //for slots/signals

      public:
    /*!
          \brief Constructor
        */
        BuildGraphicsItem(Scene* scene, QObject* parent = 0);

    /*!
          \brief Destructor
        */
        virtual ~BuildGraphicsItem();
        
    /*!
          \brief Method to build a graphics object from the properties.
      
      \param props properties
      \param draw graphics object has or not to be drawing
      
      \return z value
        */
        QGraphicsItem* buildItem(te::layout::Properties props, bool addUndo = true, bool isCopy = false);
    
       /*!
         \brief Method to create a graphic object from the properties.
      
         \param itemType type of the object that will be created
         \param coordinate represent the x,y coordinate
         \param draw graphics object has or not to be drawing
      
         \return z value
        */
        QGraphicsItem* createItem(te::layout::EnumType* itemType, const te::gm::Coord2D& coordinate, 
          double width = 0, double height = 0, bool addUndo = true);

        /*!
          \brief Method to create a graphic object from the type.
      
          \param mode type of the object will created
      
          \return item value
        */
        QGraphicsItem* createItem(te::layout::EnumType* itemType, bool addUndo = true, bool isCopy = false);


        std::string getNewName();


      signals:

        void showDialogWindow(EnumType* type, QList<QGraphicsItem*> itemList);
                
      protected:
    
        /*!
          \brief Creates the name of the new graphic object. Adds the number that corresponds to how many objects of this type have already been created.

          \param name of the class type of the graphic object
          \param type of the object

          \return name
        */
        std::string nameItem(te::layout::EnumType* type);
                
        /*!
          \brief After component construction, this method checks and adds a child, position and zValue. If necessary, also redraws.

          \param item built item
          \param draw if true the component will be redraw, false otherwise
        */
        virtual void afterBuild(QGraphicsItem* item, bool addUndo = true);

        virtual void showImgDlg(QGraphicsItem* item);

        virtual te::layout::Properties convertToProperties(const std::string& name, int id, const te::gm::Coord2D& coord, double width, double height, int zValue);

        virtual Properties collapseProperties(const Properties& properties, const Properties& newProperties);

        virtual Properties createPositionProperties(const QPointF& pos);
        
      protected:

        virtual ItemFactoryParamsCreate createParams(te::layout::EnumType* type, bool isCopy = false);       
    };
  }
}

#endif


