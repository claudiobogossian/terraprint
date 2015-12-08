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
  \file LegendItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendItem.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/singleton/Context.h"
#include "terralib/maptools/GroupingItem.h"
#include "terralib/maptools/Grouping.h"
#include "terralib/maptools/Canvas.h"
#include "terralib/maptools/CanvasConfigurer.h"
#include "terralib/qt/widgets/canvas/Canvas.h"
#include "terralib/se/Symbolizer.h"
#include "terralib/se/PolygonSymbolizer.h"
#include "terralib/se/Fill.h"
#include "../core/Scene.h"

// Qt
#include <QPixmap>
#include <QStyle>
#include <QStyleOption>
#include <QFont>
#include <QPaintDevice>
#include <QColor>
#include <QMatrix>

te::layout::LegendItem::LegendItem(AbstractItemController* controller)
  : AbstractItem<QGraphicsItem>(controller, true)
{
  //The text size or length that exceeds the sides will be cut
  setFlag(QGraphicsItem::ItemClipsToShape);
}

te::layout::LegendItem::~LegendItem()
{

}

void te::layout::LegendItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  if (!scene())
    return;

  Scene* sc = dynamic_cast<Scene*>(scene());
  Utils utils = sc->getUtils();
  
  const Property& pLayers = m_controller->getProperty("layers");  
  const std::list<te::map::AbstractLayerPtr>& layerList = pLayers.getValue().toLayerList();

  double borderDisplacement = 1;
  double displacementBetweenTitleAndSymbols = 7;
  int borderDisplacementInPixels = utils.mm2pixel(borderDisplacement);
  int dispBetweenTitleAndSymbolsInPixels = utils.mm2pixel(displacementBetweenTitleAndSymbols);
  
  if(layerList.empty() == true)
  {
    return;
  }

  QRectF boundRect = this->boundingRect();

  double x1 = boundRect.x() + borderDisplacementInPixels;
  double y1 = boundRect.y() - borderDisplacementInPixels;

  double wtxtInPixels = 0.;
  double htxtInPixels = 0.;

  for (std::list<te::map::AbstractLayerPtr>::const_iterator it = layerList.begin(); it != layerList.end(); ++it)
  {
    const te::map::AbstractLayerPtr layer = (*it);

    std::string title = layer->getTitle();

    QString qTitle(title.c_str());

    utils.textBoundingBox(wtxtInPixels, htxtInPixels, title);

    QRectF rectTitle(x1, y1, wtxtInPixels, htxtInPixels);

    painter->drawText(rectTitle, qTitle);

    y1 += dispBetweenTitleAndSymbolsInPixels;

    drawLegend(painter, layer, x1, y1);
  }
}

void te::layout::LegendItem::drawLegend(QPainter* painter, te::map::AbstractLayerPtr layer, double x1, double y1)
{
  if (!scene())
    return;

  Scene* sc = dynamic_cast<Scene*>(scene());
  Utils utils = sc->getUtils();

  double displacementBetweenSymbols = 7;
  double displacementBetweenSymbolsAndText = 2;
  double symbolsize = 5;

  int dispBetweenSymbolAndTextInPixels = utils.mm2pixel(displacementBetweenSymbolsAndText);
  int dispBetweenSymbolsInPixels = utils.mm2pixel(displacementBetweenSymbols);
  int symbolSizeInPixels = utils.mm2pixel(symbolsize);

  te::map::Grouping* grouping = layer->getGrouping();

  if (grouping != 0 && grouping->isVisible() == true)
  {
    std::string propertyName = grouping->getPropertyName();
    const std::vector<te::map::GroupingItem*>& items = grouping->getGroupingItems();
    te::map::GroupingType type = grouping->getType();

    double labelX1 = x1 + symbolSizeInPixels + dispBetweenSymbolAndTextInPixels;

    for (unsigned int i = 0; i < items.size(); ++i)
    {
      te::map::GroupingItem* item = items[i];

      drawLabel(painter, labelX1, y1, propertyName, type, item);

      const std::vector<te::se::Symbolizer*>& symbolizers = item->getSymbolizers();

      foreach(te::se::Symbolizer* symbol, symbolizers)
      {
        double offset = 2.0;
        QRectF geomRect(x1, y1, symbolSizeInPixels, symbolSizeInPixels);

        te::gm::Geometry* geom = createGeometry(geomRect, symbol);
        if (!geom)
          continue;

        drawGeometry(painter, geomRect, symbol, geom);
      }

      y1 += dispBetweenSymbolsInPixels;
    }
  }
}

te::gm::Geometry* te::layout::LegendItem::createGeometry(QRectF geomRect, te::se::Symbolizer* symbol)
{
  te::gm::Geometry* geom = 0;
  if (symbol->getType() == "PolygonSymbolizer")
  {
    geom = createPolygonSymbolizer(geomRect);
  }
  else if (symbol->getType() == "LineSymbolizer")
  {
    geom = createLineSymbolizer(geomRect);
  }
  else if (symbol->getType() == "PointSymbolizer")
  {
    geom = createPointSymbolizer(geomRect);
  }
  return geom;
}

te::gm::Geometry* te::layout::LegendItem::createPolygonSymbolizer(QRectF geomRect, double offset)
{
  te::gm::Geometry* geom = 0;
  
  double x1 = geomRect.x();
  double y1 = geomRect.y();

  te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
  te::gm::LinearRing* ring = new te::gm::LinearRing(5, te::gm::LineStringType);
  ring->setPoint(0, x1 + offset, y1 + offset);
  ring->setPoint(1, x1 + geomRect.width() - offset, y1 + offset);
  ring->setPoint(2, x1 + geomRect.width() - offset, y1 + geomRect.height() - offset);
  ring->setPoint(3, x1 + offset, y1 + geomRect.height() - offset);
  ring->setPoint(4, x1 + offset, y1 + offset);
  polygon->setRingN(0, ring);
  geom = polygon;

  return geom;
}

te::gm::Geometry* te::layout::LegendItem::createLineSymbolizer(QRectF geomRect, double offset)
{
  te::gm::Geometry* geom = 0;

  double x1 = geomRect.x();
  double y1 = geomRect.y();

  te::gm::LineString* line = new te::gm::LineString(2, te::gm::LineStringType);
  line->setPoint(0, x1 + offset, y1 + geomRect.height() * 0.5);
  line->setPoint(1, x1 + geomRect.width() - offset, y1 + geomRect.height() * 0.5);
  geom = line;

  return geom;
}

te::gm::Geometry* te::layout::LegendItem::createPointSymbolizer(QRectF geomRect)
{
  te::gm::Geometry* geom = 0;

  double x1 = geomRect.x();
  double y1 = geomRect.y();

  geom = new te::gm::Point(x1 + geomRect.width() * 0.5, y1 + geomRect.height() * 0.5);

  return geom;
}

void te::layout::LegendItem::drawGeometry(QPainter* painter, QRectF geomRect, te::se::Symbolizer*symbol, te::gm::Geometry* geom)
{
  int pixmapWidth = geom->getMBR()->getWidth();
  int pixmapHeight = geom->getMBR()->getHeight();

  double x1 = geomRect.x();
  double y1 = geomRect.y();

  QPixmap* pixmap = new QPixmap(pixmapWidth, pixmapHeight);
  //pixmap->fill(Qt::transparent);

  QMatrix matrix = painter->matrix();
  te::qt::widgets::Canvas geomCanvas(pixmap);
  geomCanvas.setMatrix(matrix);

  // Configuring...
  te::map::CanvasConfigurer cc(&geomCanvas);
  cc.config(symbol);

  // Let's draw!
  geomCanvas.draw(geom);
  
  QPointF ptGeom(x1, y1);

  painter->save();
  painter->drawPixmap(ptGeom, *pixmap);
  painter->restore();
}

void te::layout::LegendItem::drawLabel(QPainter* painter, double x1, double y1, std::string propertyName,
  te::map::GroupingType type, te::map::GroupingItem* item)
{
  if (!scene())
    return;

  const Property& pFont = m_controller->getProperty("font");
  const Property& pFontColor = m_controller->getProperty("font_color");
  const Font& font = pFont.getValue().toFont();
  const te::color::RGBAColor& fontColor = pFontColor.getValue().toColor();

  QFont qFont(QString(font.getFamily().c_str()), font.getPointSize());
  QColor qFontColor(fontColor.getRed(), fontColor.getGreen(), fontColor.getBlue(), fontColor.getAlpha());

  double wtxt = 0;
  double htxt = 0;

  painter->save();

  Scene* sc = dynamic_cast<Scene*>(scene());
  Utils utils = sc->getUtils();

  std::string label = propertyName;
  label += ": ";

  if (type == te::map::UNIQUE_VALUE)
  {
    label += item->getValue();
  }
  else
  {
    const std::string& upperLimit = item->getUpperLimit();
    const std::string& lowerLimit = item->getLowerLimit();

    label += lowerLimit;
    label += " ~ ";
    label += upperLimit;
  }

  painter->setFont(qFont);
  painter->setBrush(qFontColor);

  utils.textBoundingBox(wtxt, htxt, label);

  QRectF newLabelRect(x1, y1, wtxt, htxt);
  QString qLabel(label.c_str());
  painter->drawText(newLabelRect, qLabel);

  painter->restore();
}

