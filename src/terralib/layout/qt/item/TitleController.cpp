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

// TerraLib
#include "TitleController.h"

#include "../../core/enum/EnumAlignmentType.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/property/TextGridSettingsConfigProperties.h"
#include "../../qt/core/Scene.h"
#include "../../qt/item/TitleItem.h"

#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QFontMetricsF>
#include <QTextCursor>
#include <QTextTable>

te::layout::TitleController::TitleController(AbstractItemModel* model, AbstractItemView* view)
  : TextController(model, view)
  , m_table(0)
{
}

te::layout::TitleController::~TitleController()
{
}

QSizeF te::layout::TitleController::updateView()
{
  QRectF boundingRect;
  return boundingRect.size();
}

void te::layout::TitleController::textChanged()
{
  
}
