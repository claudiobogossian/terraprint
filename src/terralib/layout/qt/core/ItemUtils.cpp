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
  \file ItemUtils.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemUtils.h"
#include "terralib/maptools/ExternalGraphicRendererManager.h"
#include "terralib/maptools/MarkRendererManager.h"
#include "terralib/se/PointSymbolizer.h"
#include "terralib/se/Graphic.h"
#include "terralib/se/Utils.h" 
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/enum/Enums.h"
#include "../item/MapItem.h"
#include "../item/LegendItem.h"
#include "../../item/LegendModel.h"
#include "../../item/MapModel.h"
#include "../item/TextItem.h"
#include "../../item/TextModel.h"
#include "../item/LegendChildItem.h"
#include "../../item/LegendChildModel.h"
#include "../../item/GridGeodesicModel.h"
#include "../../item/GridPlanarModel.h"
#include "Scene.h"
#include "../item/GridMapItem.h"
#include "View.h"

// STL
#include <stddef.h>  // defines NULL

// Qt
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTextDocument>
#include <QFont>

double te::layout::ItemUtils::m_textDPI = 72.;

te::layout::ItemUtils::ItemUtils( QGraphicsScene* scene ) :
  m_scene(scene)
{

}

te::layout::ItemUtils::~ItemUtils()
{

}

std::vector<te::layout::MapItem*> te::layout::ItemUtils::getMapItemList(bool selected)
{
  std::vector<te::layout::MapItem*> list;

  QList<QGraphicsItem*> graphicsItems = getItems(selected);
  foreach(QGraphicsItem *item, graphicsItems)
  {
    if (!item)
      continue;

    te::layout::MapItem* mit = dynamic_cast<te::layout::MapItem*>(item);
    if (!mit)
      continue;

    list.push_back(mit);
  }

  return list;
}

te::layout::MapItem* te::layout::ItemUtils::getMapItem( std::string name )
{
  te::layout::MapItem* map = 0;

  te::layout::EnumObjectType* objectType = te::layout::Enums::getInstance().getEnumObjectType();

  std::vector<std::string> strList;

  QList<QGraphicsItem*> graphicsItems = getItems(false);
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::AbstractItemView* itemView = dynamic_cast<te::layout::AbstractItemView*>(item);
    if(itemView == 0)
      continue;

    te::layout::AbstractItemController* controller = itemView->getController();

    if(controller == 0)
      continue;

    if(controller->getProperties().getTypeObj() != objectType->getMapItem())
    {
      continue;
    }

    const Property& pName = controller->getProperty("name");
    if(pName.getValue().toString().compare(name) != 0)
      continue;

    map = dynamic_cast<te::layout::MapItem*>(itemView);
    break;
  }

  return map;
}

std::vector<std::string> te::layout::ItemUtils::mapNameList(bool selected)
{
  te::layout::EnumObjectType* objectType = te::layout::Enums::getInstance().getEnumObjectType();

  std::vector<std::string> strList;

  QList<QGraphicsItem*> graphicsItems = getItems(selected);
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::AbstractItemView* itemView = dynamic_cast<te::layout::AbstractItemView*>(item);
    if(itemView == 0)
      continue;

    te::layout::AbstractItemController* controller = itemView->getController();

    if(controller == 0)
      continue;

    if(controller->getProperties().getTypeObj() != objectType->getMapItem())
    {
      continue;
    }

    const Property& pName = controller->getProperty("name");
    strList.push_back(pName.getValue().toString());
  }

  return strList;
}

int te::layout::ItemUtils::countType( te::layout::EnumType* type )
{
  int count = 0;

  QList<QGraphicsItem*> graphicsItems = getItems();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::AbstractItemView* absItem = dynamic_cast<te::layout::AbstractItemView*>(item);
    if(absItem == 0)
      continue;

    te::layout::AbstractItemController* controller = absItem->getController();
    if(controller == 0)
      continue;

    if(controller->getProperties().getTypeObj() == type)
    {
      count+=1;
    }
  }

  return count;
}

int te::layout::ItemUtils::maxTypeId( te::layout::EnumType* type )
{
  int id = -1;

  QList<QGraphicsItem*> graphicsItems = getItems();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::AbstractItemView* absItem = dynamic_cast<te::layout::AbstractItemView*>(item);
    if(absItem == 0)
      continue;

    te::layout::AbstractItemController* controller = absItem->getController();
    if(controller == 0)
      continue;

    int currentId = controller->getProperty("id").getValue().toInt();

    if(controller->getProperties().getTypeObj() == type)
    {
      if(id == -1)
      {
        id = currentId;
      }
      else
      {
        if(currentId > id)
        {
          id = currentId;
        }
      }
    }
  }

  return id;
}

bool te::layout::ItemUtils::isCurrentMapTools()
{
  bool result = false;

  if(!m_scene)
  {
    return result;
  }

  Scene* sc = dynamic_cast<Scene*>(m_scene);
  if(!sc)
  {
    return result;
  }

  ContextObject context = sc->getContext();

  te::layout::EnumType* mode = context.getCurrentMode();
  te::layout::EnumModeType* type = te::layout::Enums::getInstance().getEnumModeType();

  if(mode == type->getModeMapPan())
  {
    result = true;
  }
  else if(mode == type->getModeMapZoomIn())
  {
    result = true;
  }
  else if(mode == type->getModeMapZoomOut())
  {
    result = true;
  }
  return result;
}

QList<QGraphicsItem*> te::layout::ItemUtils::getItems( bool selected )
{
  QList<QGraphicsItem*> graphicsItems;

  if(selected)
  {
    graphicsItems = m_scene->selectedItems();
  }
  else
  {
    graphicsItems = m_scene->items();
  }

  return graphicsItems;
}

void te::layout::ItemUtils::setCurrentToolInSelectedMapItems( EnumType* mode )
{
  
}

void te::layout::ItemUtils::createTextGridAsObject()
{
  //do nothing;
}

void te::layout::ItemUtils::createTextMapAsObject()
{

}

void te::layout::ItemUtils::createTextItemFromObject( std::map<te::gm::Point*, std::string> map, QFont* ft )
{
  
}

void te::layout::ItemUtils::createLegendChildItemFromLegend( std::map<te::gm::Point*, std::string> map, te::layout::MapModel* visitable )
{
 
}

std::vector<te::layout::Properties*> te::layout::ItemUtils::getGridMapProperties()
{
  std::vector<te::layout::Properties*> props;
  return props;
}

std::vector<te::layout::GridMapItem*> te::layout::ItemUtils::getMapChildren()
{
  std::vector<te::layout::GridMapItem*> gridMapItems;

  QGraphicsItem *item = m_scene->selectedItems().first();
  if(!item)
  {
    return gridMapItems;
  }
  
  MapItem* map = dynamic_cast<MapItem*>(item);
  if(!map)
  {
    return gridMapItems;
  }

  QList<QGraphicsItem*> graphicsItems = map->childItems();
  foreach(QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    GridMapItem* grid = dynamic_cast<GridMapItem*>(item);
    if(!grid)
    {
      continue;
    }

    gridMapItems.push_back(grid);
  }
  
  return gridMapItems;
}

QGraphicsItem* te::layout::ItemUtils::intersectionSelectionItem( int x, int y )
{
  QGraphicsItem* intersectionItem = 0;

  if(!m_scene)
  {
    return intersectionItem;
  }

  QList<QGraphicsItem*> items = m_scene->selectedItems();

  QPointF ptScene(x, y);

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      QPointF ptLocal = item->mapFromScene(ptScene);
      if (item->contains(ptLocal) == true)
      {
        intersectionItem = item;
        break;
      }
    }
  }

  return intersectionItem;
}

QRectF te::layout::ItemUtils::getTextBoundary( const std::string& fontName, int fontSize, const std::string& text ) const
{
  if(m_scene == 0)
  {
    return QRectF();
  }

  Utils utils = ((Scene*) m_scene)->getUtils();

  const ContextObject& context = ((Scene *) m_scene)->getContext();

  double correctionFactorY = context.getDpiY() / 72.;

  QFont font(fontName.c_str());
  font.setPixelSize(fontSize * correctionFactorY);

  QFontMetrics fontMetrics(font);
  QRect rect = fontMetrics.boundingRect(text.c_str());
  int width = rect.width();
  int height = rect.height();
  int descend = fontMetrics.descent();

  double widthMM = utils.pixel2mm(width);
  double heightMM = utils.pixel2mm(height);
  double descendMM = utils.pixel2mm(descend);

   QRectF textBoundingRect(0, -descendMM, widthMM, heightMM);
   return textBoundingRect;
}

QRectF te::layout::ItemUtils::getMinimumTextBoundary(const std::string& fontName, int fontSize, const std::string& text) const
{
  if (m_scene == 0)
  {
    return QRectF();
  }

  Utils utils = ((Scene*) m_scene)->getUtils();

  const ContextObject& context = ((Scene*) m_scene)->getContext();

  double correctionFactorY = context.getDpiY() / 72.;

  QFont font(fontName.c_str());
  font.setPixelSize(fontSize * correctionFactorY);

  QFontMetrics fontMetrics(font);
  QRect rect = fontMetrics.tightBoundingRect(text.c_str());
  int width = rect.width();
  int height = rect.height();
  int descend = fontMetrics.descent();

  double widthMM = utils.pixel2mm(width);
  double heightMM = utils.pixel2mm(height);
  double descendMM = utils.pixel2mm(descend);

  QRectF textBoundingRect(0, -descendMM, widthMM, heightMM);
  return textBoundingRect;
}

QPainterPath te::layout::ItemUtils::textToVector(const QString& text, const QFont& font, const QPointF& referencePoint, double rotationAngle)
{
  double textDPI = ItemUtils::getTextDPI();

  //we first calculate the correction factor to be use in the case that the given DPI is not 72.
  double correctionFactor = textDPI / 72.;
  
  //as we are in a CS based in millimeters, we calculate the relation between the millimeters CS and the device CS
  double ptSizeInches = 1. / 72.; //The size of 1 point, in inches
  double ptSizeMM = ptSizeInches * 25.4; //The size of 1 point, in millimeters
  double scale = ptSizeMM * (1. / correctionFactor); //for the calculation of the scale, we consider the pixel size in millimeters and the inverse of the correction factor

  QFont fontCopy(font);
  if (font.pixelSize() > 0)
  {
    fontCopy.setPixelSize(font.pixelSize() * correctionFactor);
  }
  if (font.pointSize() > 0)
  {
    fontCopy.setPointSize(font.pointSize());
  }
  if (font.pointSizeF() > 0)
  {
    fontCopy.setPointSizeF(font.pointSizeF());
  }

  //Now we vectorize the text by addind it to a painter path
  QPainterPath painterPath;
  painterPath.addText(0, 0, fontCopy, text);

  //Then we apply the [optional] rotation
  if(rotationAngle != 0)
  {
    double textWidth = painterPath.boundingRect().width();
    double textHeight = painterPath.boundingRect().height();

    QTransform transform;
    transform.rotate(-rotationAngle);

    painterPath = transform.map(painterPath);
    painterPath.translate(-painterPath.boundingRect().x(), -painterPath.boundingRect().y());
  }

  //and scale, so the vectorized text size can be compatible to the device
  QTransform transform;
  transform.scale(scale, -scale); //here we scale the item so the text size to be considered will be points and not millimeters

  //after the transformation is set, we apply it to the painter path
  painterPath = transform.map(painterPath);
  painterPath.translate(-painterPath.boundingRect().x(), -painterPath.boundingRect().y());

  QRectF currentRect = painterPath.boundingRect();
 
  //we [optionally] translate the painter path to the given reference coordinate
  painterPath.translate(referencePoint);

  //and return it
  return painterPath;
}

void te::layout::ItemUtils::changeViewMode( EnumType* mode )
{
  if(!m_scene)
  {
    return;
  }

  View* view = ((Scene *) m_scene)->getView();
  if(!view)
  {
    return;
  }

  view->changeMode(mode);
}

te::layout::AbstractItemView* te::layout::ItemUtils::getSelectedItem()
{
  AbstractItemView* abstractItem = 0;

  if (m_scene == 0)
  {
    return abstractItem;
  }

  QList<QGraphicsItem*> items = m_scene->items();
  QGraphicsItem* item = items.first();

  if (!item)
  {
    return abstractItem;
  }

  abstractItem = dynamic_cast<AbstractItemView*>(item);
  return abstractItem;
}

void te::layout::ItemUtils::ConfigurePainterForTexts(QPainter* painter, Font txtFont)
{
  QFont fontCopy = convertToQfont(txtFont);
  painter->setFont(fontCopy);
}

QFont  te::layout::ItemUtils::convertToQfont(Font txtFont){

  //converts information about the font
  QFont qft;
  qft.setFamily(txtFont.getFamily().c_str());
  qft.setBold(txtFont.isBold());
  qft.setItalic(txtFont.isItalic());
  qft.setKerning(txtFont.isKerning());
  qft.setStrikeOut(txtFont.isStrikeout());
  qft.setUnderline(txtFont.isUnderline());

  //we need to set the font size from PT to MM

  qft.setPointSizeF(txtFont.getPointSize());

  return qft;
}

double te::layout::ItemUtils::getTextDPI()
{
  return m_textDPI;
}

void te::layout::ItemUtils::setTextDPI(double textDpi)
{
  m_textDPI = textDpi;
}

te::color::RGBAColor** te::layout::ItemUtils::changePointMarkSize(te::se::PointSymbolizer* pointSymbol, std::size_t width, std::size_t height)
{
  te::color::RGBAColor** rgba = 0;

  if (!pointSymbol)
  {
    return rgba;
  }

  const te::se::Graphic* graphic = pointSymbol->getGraphic();    
  const std::vector<te::se::ExternalGraphic*> exgs = graphic->getExternalGraphics();

  // Change the graphic size
  if (!exgs.empty())
    rgba = te::map::ExternalGraphicRendererManager::getInstance().render(exgs[0], height, width);
  else
  {
    const std::vector<te::se::Mark*> marks = graphic->getMarks();
    if (!marks.empty())
      rgba = te::map::MarkRendererManager::getInstance().render(marks[0], height);
  }
  return rgba;
}

QPainterPath te::layout::ItemUtils::lineToQPath(const te::gm::LineString& line)
{
  QPainterPath path;

  size_t numCoords = line.size();
  for (size_t i = 0; i < numCoords; ++i)
  {
    te::gm::Point* point = line.getPointN(i);
    path.lineTo(point->getX(), point->getY());
  }

  return path;
}

std::string te::layout::ItemUtils::convert2StdString(const QString& qString)
{
  std::string output(qString.toLatin1().constData());
  return output;
}

QString te::layout::ItemUtils::convert2QString(const std::string& stdString)
{
  return QString::fromLatin1(stdString.c_str());
}

