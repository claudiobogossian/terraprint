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
  \file Observable.h
   
  \brief Abstract class to represent an observable. "Model" part of MVC component. Each item, at the end of the setProperties method, 
        adds a Property Change command to the Undo/Redo stack, via the scene. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMVIEW_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMVIEW_H

// TerraLib
#include "../../Config.h"
#include "../../../core/ContextObject.h"
#include "../../../core/enum/AbstractType.h"

class QUndoCommand;

namespace te
{
  namespace layout
  {

    class AbstractItemController;
    class AbstractItemModel;
    class ContextObject;
    class AbstractScene;
    class Properties;
    class Property;
    class ItemInputProxy;

    /*!
      \brief Abstract class to represent an observable. "Model" part of MVC component. 
    
      \ingroup layout
    */
    class TELAYOUTEXPORT AbstractItemView
    {
      public:

        /*!
          \brief Constructor
        */ 
        AbstractItemView(te::layout::ItemInputProxy* itemInputProxy);

        /*!
          \brief Destructor
        */ 
        virtual ~AbstractItemView();

        /*!
          \brief Refreshes the drawings of the view
        */
        virtual void refresh() = 0;

        /*!
        \brief This function is called every time the context is updated. It is usually used by the item to configure its drawings due to changes in the zoom factor or in the DPI of the device.
        */
        virtual void contextUpdated(const ContextObject& context) = 0;

        /*!
          \brief Gets the rotation
        */ 
        virtual double getItemRotation() const = 0;

        /*!
          \brief Sets the rotation
        */ 
        virtual void setItemRotation(double rotation) = 0;

        /*!
          \brief Sets the position
        */
        virtual void setItemPosition(double x, double y) = 0;

        virtual void setEditionMode(bool editionMode);

        virtual bool isEditionMode();

        virtual void setSubSelection(bool selected);

        virtual bool isSubSelected() const;

        virtual AbstractScene* getScene() const = 0;

        virtual void prepareGeometryChange() = 0;

        virtual te::layout::ItemAction getCurrentAction() = 0;

        const te::layout::Properties& getProperties() const;

        void setProperties(const te::layout::Properties& properties);

        const te::layout::Property& getProperty(const std::string& propertyName) const;

        void setProperty(const te::layout::Property& property);

        /*!
        \brief Attaches the model of the given controller to the model of this instance
        */
        void attach(AbstractItemView* view);

        /*!
        \brief Detaches the model of the given controller from the model of this instance
        */
        void detach(AbstractItemView* view);

        /* 
          \brief Each item, at the end of the setProperties method, 
          adds a Property Change command to the Undo/Redo stack, via the scene. 
         */
        virtual void addUndoCommandToStack(QUndoCommand* command) = 0;

        /*
        \brief Set properties will or not generate an UndoCommand on the stack.

        \param enabled if true will generate an UndoCommand on the stack, false otherwise.

        */
        virtual void setUndoEnabled(bool enabled);

        /*
        \brief Check if set properties will or not generate an UndoCommand on the stack.

        \return if true will generate an UndoCommand on the stack, false otherwise.

        */
        bool isUndoEnabled();

        virtual ItemInputProxy* getItemInputProxy() const;

    protected:

      virtual void enterEditionMode() = 0;

      virtual void leaveEditionMode() = 0;

      virtual AbstractItemController* createController() const;

      virtual AbstractItemModel* createModel() const = 0;

      /*!
      \brief Gets the controller
      */
      virtual AbstractItemController* getController() const;

    private:

      mutable AbstractItemController* m_controller; //!< The controller

    protected:

        bool                    m_isEditionMode;
        bool                    m_subSelected;
        bool                    m_useResizePixmap; //!< use or not pixmap for resizing
        bool                    m_undoEnabled; // set properties will or not generate an UndoCommand on the stack
        ItemInputProxy*         m_itemInputProxy;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMVIEW_H
