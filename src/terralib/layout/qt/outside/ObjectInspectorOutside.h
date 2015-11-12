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
  \file ObjectInspectorOutside.h
   
  \brief Tree of names of all the items entered on the scene, MVC components, using Qt to present the name of each item and its class. Object Inspector.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OBJECTINSPECTOR_OUTSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_OBJECTINSPECTOR_OUTSIDE_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractOutsideView.h"

// STL
#include <string>

// Qt
#include <QWidget>

class QGraphicsItem;
class QTreeWidget;
class QContextMenuEvent;
class QAction;
class QMenu;

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;
    class Scene;

    /*!
    \brief Tree of names of all the items entered on the scene, MVC components, using Qt to present the name of each item and its class. Object Inspector.
    
      \ingroup layout

      \sa te::layout::OutsideObserver
    */
    class TELAYOUTEXPORT ObjectInspectorOutside : public QWidget, public AbstractOutsideView
    {
      Q_OBJECT //for slots/signals

    public:

      ObjectInspectorOutside(Scene* scene, AbstractOutsideController* controller);
      
      virtual ~ObjectInspectorOutside();
            
      virtual void setPosition(const double& x, const double& y);
      
      virtual te::gm::Coord2D getPosition();

      virtual void itemsInspector(QList<QGraphicsItem*> graphicsItems);

      virtual void selectItems(QList<QGraphicsItem*> graphicsItems);

    protected slots:
      
      virtual void onRemoveProperties(std::vector<std::string> names);

      virtual void itemSelectionChanged();

      virtual void onMenuTriggered(QAction*);

    signals:

      void currentItemChanged(QGraphicsItem* item);

      void selectionChanged(QList<QGraphicsItem*> graphicsItems);
      
    protected:

      virtual void contextMenuEvent(QContextMenuEvent * event);

      virtual QAction* createAction(std::string text, std::string objName, std::string icon, std::string tooltip = "");

      virtual void createMenu(std::string itemName);
      
      QTreeWidget*            m_treeWidget;
      bool                    m_isChangingSelection;
      QList<QGraphicsItem*>   m_graphicsItems;
      Scene*                  m_scene;
      QMenu*                  m_menu;
    };
  }
}

#endif
