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
\file ImageItem.cpp

\brief

\ingroup layout
*/

// TerraLib
#include "ImageItem.h"
#include "../../item/ImageModel.h"
#include "ImageController.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
// Qt
#include <QStyleOptionGraphicsItem>

te::layout::ImageItem::ImageItem(te::layout::ItemInputProxy* itemInputProxy)
  : AbstractItem(itemInputProxy)
{

}

te::layout::ImageItem::~ImageItem()
{

}

te::layout::AbstractItemModel* te::layout::ImageItem::createModel() const
{
  return new ImageModel();
}

te::layout::AbstractItemController* te::layout::ImageItem::createController() const
{
  AbstractItemModel* model = createModel();
  return new ImageController(model, (AbstractItemView*)this);
}

const std::string& te::layout::ImageItem::getFileName() const
{
  return m_fileName;
}
void te::layout::ImageItem::setFileName(const std::string& fileName)
{
  m_fileName = fileName;
  if (m_fileName.empty() == true)
  {
    m_image = QImage();
  }
  else
  {
    QString qFileName = ItemUtils::convert2QString(m_fileName);
    if (m_image.load(qFileName) == true)
    {
      m_image = m_image.mirrored();
    }
  }
}

void te::layout::ImageItem::drawItem(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
  const Property& pFrameColor = this->getProperty("frame_color");
  const te::color::RGBAColor& framwColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFrameColor);
  QColor qContourColor(framwColor.getRed(), framwColor.getGreen(), framwColor.getBlue(), framwColor.getAlpha());

  const Property& lineWidth = this->getProperty("line_width");
  double lnew = te::layout::Property::GetValueAs<double>(lineWidth);

  QPen pen(qContourColor, lnew, Qt::SolidLine);

  QRectF boundRect = boundingRect();

  if (m_image.isNull() == true)
  {
    painter->save();

    painter->setPen(pen);

    painter->drawRect(boundRect);

    painter->restore();

    return;
  }

  QRectF sourceRect(0, 0, m_image.width(), m_image.height());

  painter->save();

  painter->setPen(pen);
  painter->setRenderHint(QPainter::Antialiasing, true);

  //draws the item
  painter->drawImage(boundRect, m_image, sourceRect);

  painter->restore();
}

void te::layout::ImageItem::adjustSize()
{
  double width = m_image.width();
  double height = m_image.height();
  double factor = width / height;
  if (factor < 1)
  {
    height = te::layout::Property::GetValueAs<double>(this->getProperty("height"));
    width = height * factor;
  }
  else
  {
    width = te::layout::Property::GetValueAs<double>(this->getProperty("width"));
    height = width / factor;
  }

  Properties properties;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  this->setProperties(properties);
}
