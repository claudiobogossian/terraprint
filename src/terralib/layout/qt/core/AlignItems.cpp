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

bool te::layout::AlignItems::bringToFront(bool oneLevel)
{
  bool result = false;

  if (m_scene->selectedItems().isEmpty())
    return result;

  QGraphicsItem* selectedItem = m_scene->selectedItems().first();
  QGraphicsItem* childItem = searchForSubselection(selectedItem);
  if (childItem)
  {
    selectedItem = childItem;
  }

  QList<QGraphicsItem *> items = m_scene->items(Qt::AscendingOrder);

  qreal maxZValue = selectedItem->zValue();
  qreal zValue = selectedItem->zValue();
  qreal beforeZValue = selectedItem->zValue();

  foreach(QGraphicsItem *item, items)
  {
    if(item)
    {
      AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
      if (it)
      {
        if ((item->zValue() >= zValue))
        {
          if (item->zValue() > maxZValue)
          {
            beforeZValue = maxZValue;
            maxZValue = item->zValue();
            item->setZValue(beforeZValue);
            if (oneLevel)
            {
              break;
            }
          }
        }
      }
    }
  }

  selectedItem->setZValue(maxZValue);

  if (maxZValue != zValue)
  {
    result = true;
  }
  return result;
}

bool te::layout::AlignItems::sendToBack(bool oneLevel)
{
  bool result = false;

  if (m_scene->selectedItems().isEmpty())
    return result;

  QGraphicsItem *selectedItem = m_scene->selectedItems().first();
  QGraphicsItem* childItem = searchForSubselection(selectedItem);
  if (childItem)
  {
    selectedItem = childItem;
  }

  QList<QGraphicsItem *> items = m_scene->items(Qt::DescendingOrder);

  qreal minimumZValue = selectedItem->zValue();
  qreal zValue = selectedItem->zValue();
  qreal beforeZValue = selectedItem->zValue();

  foreach(QGraphicsItem *item, items)
  {
    if(item)
    {
      AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
      if(it)
      {
        if (item->zValue() <= zValue && item->zValue() > m_minimunZValue)
        {
          if (item->zValue() < minimumZValue)
          {
            beforeZValue = minimumZValue;
            minimumZValue = item->zValue();
            item->setZValue(beforeZValue);
            if (oneLevel)
            {
              break;
            }
          }
        }
      }
    }
  }

  selectedItem->setZValue(minimumZValue);

  if (minimumZValue != zValue)
  {
    result = true;
  }
  return result;
}

bool te::layout::AlignItems::bringForward()
{
  // one level
  return bringToFront(true);
}

bool te::layout::AlignItems::sendBackward()
{
  // one level
  return sendToBack(true);
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
    //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
    double dx = items.first()->scenePos().x() - items.first()->sceneBoundingRect().x();

    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbLeft = ppbx.getLowerLeftX() + dx;
    QPointF pot(dbLeft, items.first()->scenePos().y());

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
      //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
      double dx = item->scenePos().x() - item->sceneBoundingRect().x();

      QPointF pt(dbLeft + dx, item->scenePos().y());
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
    //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
    double dx = items.first()->scenePos().x() - items.first()->sceneBoundingRect().x();

    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbRight = ppbx.getUpperRightX();
    w = dbRight - items.first()->sceneBoundingRect().width() + dx;
    QPointF pot(w, items.first()->scenePos().y());

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
      //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
      double dx = item->scenePos().x() - item->sceneBoundingRect().x();

      w = dbRight - item->sceneBoundingRect().width() + dx;
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
    //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
    double dy = items.first()->scenePos().y() - items.first()->sceneBoundingRect().y();

    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbBottom = ppbx.getUpperRightY();
    h = dbBottom - items.first()->sceneBoundingRect().height();
    QPointF pot(items.first()->scenePos().x(), h + dy);

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
      //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
      double dy = item->scenePos().y() - item->sceneBoundingRect().y();

      h = dbBottom - item->sceneBoundingRect().height();
      QPointF pt(item->scenePos().x(), h + dy);
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
    //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
    double dy = items.first()->scenePos().y() - items.first()->sceneBoundingRect().y();

    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbTop = ppbx.getLowerLeftY();
    QPointF pot(items.first()->scenePos().x(), dbTop + dy);

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
      //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
      double dy = item->scenePos().y() - item->sceneBoundingRect().y();

      QPointF pt(item->scenePos().x(), dbTop + dy);
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
    //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
    double dx = items.first()->scenePos().x() - items.first()->sceneBoundingRect().x();

    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbCenterHrz = ppbx.getCenter().x;
    w = items.first()->sceneBoundingRect().width() / 2.;
    QPointF pot(dbCenterHrz - w + dx, items.first()->scenePos().y());

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
      //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
      double dx = item->scenePos().x() - item->sceneBoundingRect().x();

      w = item->sceneBoundingRect().width() / 2.;

      QPointF pt(dbCenterHrz - w + dx, item->scenePos().y());
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
    //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
    double dy = items.first()->scenePos().y() - items.first()->sceneBoundingRect().y();

    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbCenterVrt = ppbx.getCenter().y;
    h = items.first()->sceneBoundingRect().height() / 2.;
    QPointF pot(items.first()->scenePos().x(), dbCenterVrt - h + dy);

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
      //some items like the MapItem have their bounding rects translated from its position, and so we must consider the displacement
      double dy = items.first()->scenePos().y() - items.first()->sceneBoundingRect().y();

      h = item->sceneBoundingRect().height() / 2.;

      QPointF pt(item->scenePos().x(), dbCenterVrt - h + dy);
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

QGraphicsItem* te::layout::AlignItems::searchForSubselection(QGraphicsItem* parentItem)
{
  QGraphicsItem* item = 0;
  if (!parentItem)
  {
    return item;
  }

  QList<QGraphicsItem*> list = parentItem->childItems();
  if (list.isEmpty())
  {
    return item;
  }

  foreach (QGraphicsItem* childItem, list)
  {
    AbstractItemView* it = dynamic_cast<AbstractItemView*>(childItem);
    if (it)
    {
      if (it->isSubSelected())
      {
        item = childItem;
        break;
      }
    }
  }
  return item;
}
