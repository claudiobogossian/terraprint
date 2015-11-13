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
  \file ToolbarItemInside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapToolbarInside.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/property/Properties.h"
#include "../item/MapItem.h"
#include "MapToolbarController.h"

// Qt
#include <QToolBar>
#include <QToolButton>
#include <QAction>

te::layout::MapToolbarInside::MapToolbarInside(MapToolbarController* controller, QObject* parent) :
    QObject(parent)
  , m_controller(controller)
{
  m_controller->setMapToolbarInside(this);
  createToolbar();
}

te::layout::MapToolbarInside::~MapToolbarInside()
{
  if (m_controller)
  {
    delete m_controller;
    m_controller = 0;
  }
}

void te::layout::MapToolbarInside::createToolbar()
{
  m_toolbar = new QToolBar("Map Component ToolBar");

  createPanButton();
  m_toolbar->addSeparator();

  createZoomInButton();
  m_toolbar->addSeparator();

  createZoomOutButton();
}

void te::layout::MapToolbarInside::clear()
{
  if (m_item)
  {
    MapItem* mapItem = dynamic_cast<MapItem*>(m_item);
    if (mapItem)
    {
      mapItem->removeCurrentTool();
    }
  }
}

QToolButton* te::layout::MapToolbarInside::createPanButton()
{
  QToolButton *btn = 0;

  if (!m_toolbar)
    return btn;

  btn = createToolButton("Pan Tool", "Pan Tool", "layout-map-pan");
  btn->setCheckable(false);

  connect(btn, SIGNAL(clicked(bool)), m_controller, SLOT(onPanToolClicked(bool)));
  
  m_toolbar->addWidget(btn);
  
  return btn;
}

QToolButton* te::layout::MapToolbarInside::createZoomInButton()
{
  QToolButton *btn = 0;

  if (!m_toolbar)
    return btn;
  
  btn = createToolButton("Zoom In", "Zoom In", "layout-map-zoom-in");
  btn->setCheckable(false);

  connect(btn, SIGNAL(clicked(bool)), m_controller, SLOT(onZoomInClicked(bool)));

  m_toolbar->addWidget(btn);

  return btn;
}

QToolButton* te::layout::MapToolbarInside::createZoomOutButton()
{
  QToolButton *btn = 0;

  if (!m_toolbar)
    return btn;
  
  btn = createToolButton("Zoom Out", "Zoom Out", "layout-map-zoom-out");
  btn->setCheckable(false);

  connect(btn, SIGNAL(clicked(bool)), m_controller, SLOT(onZoomOutClicked(bool)));

  m_toolbar->addWidget(btn);

  return btn;
}

QToolButton* te::layout::MapToolbarInside::createSystematicScaleButton()
{
  QToolButton *btn = 0;

  if (!m_toolbar)
    return btn;
  
  btn = createToolButton("Systematic Scale", "Systematic Scale", "layout-systematic-map");
  btn->setCheckable(false);

  connect(btn, SIGNAL(clicked(bool)), m_controller, SLOT(onSystematicScaleClicked(bool)));

  m_toolbar->addWidget(btn);

  return btn;
}

QToolButton* te::layout::MapToolbarInside::createTextGridAsObjectButton()
{
  QToolButton *btn = 0;

  if (!m_toolbar)
    return btn;

  btn = createToolButton("Text Grid as Object", "Text Grid as Object", "layout-createtext-as-obj");
  btn->setCheckable(false);

  connect(btn, SIGNAL(clicked(bool)), m_controller, SLOT(onTextGridAsObjectClicked(bool)));

  m_toolbar->addWidget(btn);

  return btn;
}

QToolButton* te::layout::MapToolbarInside::createTextMapAsObjectButton()
{
  QToolButton *btn = 0;

  if (!m_toolbar)
    return btn;
  
  btn = createToolButton("Text Map as Object", "Text Map as Object", "layout-createmap-text-as-objs");
  btn->setCheckable(false);

  connect(btn, SIGNAL(clicked(bool)), m_controller, SLOT(onTextMapAsObjectClicked(bool)));

  m_toolbar->addWidget(btn);

  return btn;
}

QToolButton* te::layout::MapToolbarInside::createLegendAsObjectButton()
{
  QToolButton *btn = 0;

  if (!m_toolbar)
    return btn;
  
  btn = createToolButton("Legend as Object", "Legend as Object", "layout-legend-child");
  btn->setCheckable(false);

  connect(btn, SIGNAL(clicked(bool)), m_controller, SLOT(onLegendAsObjectClicked(bool)));

  m_toolbar->addWidget(btn);

  return btn;
}

