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
  \file AlignItems.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AlignItems.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/geometry/Coord2D.h"
#include "../core/pattern/command/MoveCommand.h"
#include "../core/Scene.h"

// STL
#include <sstream>

// Qt
#include <QGraphicsItem>
#include <QUndoCommand>

te::layout::AlignItems::AlignItems( Scene* scene, PaperConfig* config ):
  m_scene(scene),
  m_config(config),
  m_minimunZValue(0)
{
 
}

te::layout::AlignItems::~AlignItems()
{

}

void te::layout::AlignItems::bringToFront()
{
  if (m_scene->selectedItems().isEmpty())
    return;

  QGraphicsItem* selectedItem = m_scene->selectedItems().first();
  QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

  qreal maxZValue = selectedItem->zValue();
  QGraphicsItem* itemMaxZValue = selectedItem;

  qreal zValue = selectedItem->zValue();
  foreach (QGraphicsItem *item, overlapItems) 
  {
    if(item)
    {
      AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
      if(it)
      {        
        if((item->zValue() >= zValue))
        {
          maxZValue = item->zValue();
          itemMaxZValue = item;     
        }
      }
    }
  }
  selectedItem->setZValue(maxZValue);
  if(itemMaxZValue)
  {
    itemMaxZValue->setZValue(zValue);
  }
}

void te::layout::AlignItems::sendToBack()
{
  if (m_scene->selectedItems().isEmpty())
    return;

  QGraphicsItem *selectedItem = m_scene->selectedItems().first();
  QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

  qreal minimumZValue = selectedItem->zValue();
  QGraphicsItem* itemMinimumZValue = selectedItem;

  qreal zValue = selectedItem->zValue();
  foreach (QGraphicsItem *item, overlapItems) 
  {
    if(item)
    {
      AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
      if(it)
      {
        if (item->zValue() <= zValue)
        {
          if (item->zValue() > m_minimunZValue) 
          {
            minimumZValue = item->zValue();
            itemMinimumZValue = item;
          }
        }
      }
    }
  }
  selectedItem->setZValue(minimumZValue);
  if(itemMinimumZValue)
  {
    itemMinimumZValue->setZValue(zValue);
  }
}

void te::layout::AlignItems::alignLeft()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;
  
  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  if (items.empty())
    return;

  double dbLeft = 0;

  double ww = 0;
  double hh = 0;
  
  m_config->getPaperSize(ww, hh);

  std::map<QGraphicsItem*, QPointF> itemsWithPoints;

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbLeft = ppbx.getLowerLeftX();
    QPointF pot(dbLeft, items.first()->sceneBoundingRect().y());

    itemsWithPoints[items.first()] = items.first()->scenePos();//it will be added to the undo/redo stack

    items.first()->setPos(pot);
    addToUndoRedoStack(itemsWithPoints);
    return;
  }

  dbLeft = sourceRect.left();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      QPointF pt(dbLeft, item->scenePos().y());
      itemsWithPoints[item] = item->scenePos();//it will be added to the undo/redo stack
      item->setPos(pt);
    }
  }

  addToUndoRedoStack(itemsWithPoints);
}

void te::layout::AlignItems::alignRight()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  if (items.empty())
    return;

  double dbRight = 0;
  double w = 0;

  double ww = 0;
  double hh = 0;

  std::map<QGraphicsItem*, QPointF> itemsWithPoints;

  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbRight = ppbx.getUpperRightX();
    w = dbRight - items.first()->sceneBoundingRect().width();
    QPointF pot(w, items.first()->sceneBoundingRect().y());

    itemsWithPoints[items.first()] = items.first()->scenePos(); //it will be added to the undo/redo stack

    items.first()->setPos(pot);
    addToUndoRedoStack(itemsWithPoints);
    return;
  }

  dbRight = sourceRect.right();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      w = dbRight - item->sceneBoundingRect().width();
      QPointF pt(w, item->scenePos().y());
      itemsWithPoints[item] = item->scenePos(); //it will be added to the undo/redo stack
      item->setPos(pt);
    }
  }

  addToUndoRedoStack(itemsWithPoints);
}

void te::layout::AlignItems::alignTop()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  if (items.empty())
    return;

  double dbBottom = 0;
  double h = 0;

  double ww = 0;
  double hh = 0;

  std::map<QGraphicsItem*, QPointF> itemsWithPoints;

  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbBottom = ppbx.getUpperRightY();
    h = dbBottom - items.first()->sceneBoundingRect().height();
    QPointF pot(items.first()->sceneBoundingRect().x(), h);

    itemsWithPoints[items.first()] = items.first()->scenePos(); //it will be added to the undo/redo stack

    items.first()->setPos(pot);
    addToUndoRedoStack(itemsWithPoints);
    return;
  }

  dbBottom = sourceRect.bottom();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      h = dbBottom - item->sceneBoundingRect().height();
      QPointF pt(item->scenePos().x(), h);
      itemsWithPoints[item] = item->scenePos(); //it will be added to the undo/redo stack
      item->setPos(pt);
    }
  }

  addToUndoRedoStack(itemsWithPoints);
}

void te::layout::AlignItems::alignBottom()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  if (items.empty())
    return;

  double dbTop = 0;

  double ww = 0;
  double hh = 0;

  std::map<QGraphicsItem*, QPointF> itemsWithPoints;

  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbTop = ppbx.getLowerLeftY();
    QPointF pot(items.first()->sceneBoundingRect().x(), dbTop);

    itemsWithPoints[items.first()] = items.first()->scenePos(); //it will be added to the undo/redo stack

    items.first()->setPos(pot);
    addToUndoRedoStack(itemsWithPoints);
    return;
  }

  dbTop = sourceRect.top();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      QPointF pt(item->scenePos().x(), dbTop);
      itemsWithPoints[item] = item->scenePos(); //it will be added to the undo/redo stack
      item->setPos(pt);
    }
  }

  addToUndoRedoStack(itemsWithPoints);
}

void te::layout::AlignItems::alignCenterHorizontal()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  if (items.empty())
    return;

  double dbCenterHrz = 0;
  double w = 0;

  double ww = 0;
  double hh = 0;

  std::map<QGraphicsItem*, QPointF> itemsWithPoints;

  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbCenterHrz = ppbx.getCenter().x;
    w = items.first()->sceneBoundingRect().width() / 2.;
    QPointF pot(dbCenterHrz - w, items.first()->sceneBoundingRect().y());

    itemsWithPoints[items.first()] = items.first()->scenePos(); //it will be added to the undo/redo stack

    items.first()->setPos(pot);
    addToUndoRedoStack(itemsWithPoints);
    return;
  }

  dbCenterHrz = sourceRect.center().x();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      w = item->sceneBoundingRect().width() / 2.;

      QPointF pt(dbCenterHrz - w, item->scenePos().y());
      itemsWithPoints[item] = item->scenePos(); //it will be added to the undo/redo stack
      item->setPos(pt);
    }
  }

  addToUndoRedoStack(itemsWithPoints);
}

void te::layout::AlignItems::alignCenterVertical()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  if (items.empty())
    return;

  double dbCenterVrt = 0;
  double h = 0;

  double ww = 0;
  double hh = 0;

  std::map<QGraphicsItem*, QPointF> itemsWithPoints;

  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbCenterVrt = ppbx.getCenter().y;
    h = items.first()->sceneBoundingRect().height() / 2.;
    QPointF pot(items.first()->sceneBoundingRect().x(), dbCenterVrt - h);

    itemsWithPoints[items.first()] = items.first()->scenePos(); //it will be added to the undo/redo stack

    items.first()->setPos(pot);
    addToUndoRedoStack(itemsWithPoints);
    return;
  }

  dbCenterVrt = sourceRect.center().y();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      h = item->sceneBoundingRect().height() / 2.;

      QPointF pt(item->scenePos().x(), dbCenterVrt - h);
      itemsWithPoints[item] = item->scenePos(); //it will be added to the undo/redo stack
      item->setPos(pt);
    }
  }

  addToUndoRedoStack(itemsWithPoints);
}

QRectF te::layout::AlignItems::getSelectionItemsBoundingBox()
{
  QRectF sourceRect; 
  QList<QGraphicsItem *> items = m_scene->selectedItems(); 

  foreach(QGraphicsItem *item, items) 
  { 
    sourceRect = sourceRect.united(item->sceneBoundingRect()); 
  }

  return sourceRect;
}

int te::layout::AlignItems::getMinimumZValue()
{
  return m_minimunZValue;
}

void te::layout::AlignItems::setMinimumZValue(int minimum)
{
  m_minimunZValue = minimum;
}

bool te::layout::AlignItems::addToUndoRedoStack(std::map<QGraphicsItem*, QPointF> items)
{
  bool result = true;

  QUndoCommand* command = new MoveCommand(items);
  m_scene->addUndoStack(command);

  return result;
}

