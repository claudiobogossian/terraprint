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
  \file BuildPaintCustomData.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildPaintCustomData.h"
#include "../../../core/enum/Enums.h"

#include <QString>
#include <QVariant>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QPainter>
#include <QRectF>
#include <QFont>

te::layout::BuildPaintCustomData::BuildPaintCustomData()
{
}

te::layout::BuildPaintCustomData::~BuildPaintCustomData()
{

}

void te::layout::BuildPaintCustomData::build()
{

}

bool te::layout::BuildPaintCustomData::paintCustomData(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  QVariant variant;
  variant = index.data(dataType->getDataTypeInt()->getId());
  if (variant.isValid())
  {
    QString value = intToString(variant);
    painter->drawText(option.rect, Qt::AlignLeft, value);
    return true;
  }

  variant = index.data(dataType->getDataTypeDouble()->getId());
  if (variant.isValid())
  {
    QString value = doubleToString(variant);
    painter->drawText(option.rect, Qt::AlignLeft, value);
    return true;
  }

  variant = index.data(dataType->getDataTypeString()->getId());
  if (variant.isValid())
  {
    QString value = stringToString(variant);
    painter->drawText(option.rect, Qt::AlignLeft, value);
    return true;
  }

  variant = index.data(dataType->getDataTypeBool()->getId());
  if (variant.isValid())
  {
    QString value = boolToString(variant);
    painter->drawText(option.rect, Qt::AlignLeft, value);
    return true;
  }

  variant = index.data(dataType->getDataTypeFont()->getId());
  if (variant.isValid())
  {
    QString value = fontToString(variant);
    painter->drawText(option.rect, Qt::AlignLeft, value);
    return true;
  }

  variant = index.data(dataType->getDataTypeEnvelope()->getId());
  if (variant.isValid())
  {
    QString value = rectToString(variant);
    painter->drawText(option.rect, Qt::AlignLeft, value);
    return true;
  }
  return false;
}

QString te::layout::BuildPaintCustomData::intToString(QVariant& value)
{
  int val = value.toInt();
  return QString::number(val);
}

QString te::layout::BuildPaintCustomData::doubleToString(QVariant& value)
{
  double val = value.toDouble();
  return QString::number(val);
}

QString te::layout::BuildPaintCustomData::stringToString(QVariant& value)
{
  return value.toString();
}

QString te::layout::BuildPaintCustomData::boolToString(QVariant& value)
{
  return value.toString();
}

QString te::layout::BuildPaintCustomData::fontToString(QVariant& value)
{
  return value.toString();
}

QString te::layout::BuildPaintCustomData::rectToString(QVariant& value)
{
  QRectF rect = value.toRectF();

  return QString("[(%1, %2), %3 x %4]").arg(QString::number(rect.x()))\
    .arg(QString::number(rect.y()))
    .arg(QString::number(rect.width()))
    .arg(QString::number(rect.height()));
}

QString te::layout::BuildPaintCustomData::colorToString(QVariant& value)
{
  return value.toString();
}

