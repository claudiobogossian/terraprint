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
#include "TextController.h"

#include "../../core/enum/EnumAlignmentType.h"
#include "../../qt/core/Scene.h"
#include "../../qt/item/AbstractItem.h"
#include "../../qt/item/TextItem.h"


#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QTextCursor>

te::layout::TextController::TextController(AbstractItemModel* model)
  : AbstractItemController(model)
  , m_dpiForCalculation(96.)
{
}

te::layout::TextController::~TextController()
{
}

void te::layout::TextController::setProperty(const te::layout::Property& property)
{
  te::layout::Properties properties;
  properties.addProperty(property);

  setProperties(properties);
}

void te::layout::TextController::setProperties(const te::layout::Properties& properties)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  Properties propertiesCopy(properties);


  if (!needUpdateBox(properties))
  {
    AbstractItemController::setProperties(propertiesCopy);
    return;
  }

  TextItem* textItem = dynamic_cast<TextItem*>(m_view);
  QSizeF sizeMM;

  double dx = 0.;
  double dy = 0.;

  calculateSize(propertiesCopy, sizeMM, dx, dy);

  Property propertyWidth(0);
  propertyWidth.setName("width");
  propertyWidth.setValue(sizeMM.width(), dataType->getDataTypeDouble());

  Property propertyHeight(0);
  propertyHeight.setName("height");
  propertyHeight.setValue(sizeMM.height(), dataType->getDataTypeDouble());


  propertiesCopy.addProperty(propertyWidth);
  propertiesCopy.addProperty(propertyHeight);


  Property pDx;
  pDx.setName("dx");
  pDx.setValue(dx, dataType->getDataTypeDouble());

  Property pDY;
  pDY.setName("dy");
  pDY.setValue(dy, dataType->getDataTypeDouble());

  propertiesCopy.addProperty(pDx);
  propertiesCopy.addProperty(pDY);


  textItem->prepareGeometryChange();

  AbstractItemController::setProperties(propertiesCopy);
}

double te::layout::TextController::getDpiForCalculation() const
{
  return m_dpiForCalculation;
}


void te::layout::TextController::calculateSize(const te::layout::Properties& properties, QSizeF& sizeMM, double& dx, double& dy)
{
  Properties propertiesCopy(properties);

  te::layout::Property pText = propertiesCopy.getProperty("text");
  if (pText.isNull())
  {
    pText = this->getProperty("text");
  }

  te::layout::Property pFont = propertiesCopy.getProperty("font");
  if (pFont.isNull())
  {
    pFont = this->getProperty("font");
  }


  QString qText = ItemUtils::convert2QString(pText.getValue().toString());
  QFont qFont = ItemUtils::convertToQfont(pFont.getValue().toFont());

  TextItem* textItem = dynamic_cast<TextItem*>(m_view);

  QTextDocument textDocument;
  textDocument.setTextWidth(-1);
  textDocument.setDefaultFont(qFont);
  textDocument.setPlainText(qText);
  textDocument.setDocumentMargin(0);
  textDocument.setTextWidth(textDocument.size().width());
  QSizeF sizePx = textDocument.size();

  QGraphicsScene* qScene = textItem->scene();
  if (qScene != 0)
  {
    Scene* myScene = dynamic_cast<Scene*>(qScene);
    m_dpiForCalculation = myScene->getContext().getDpiX();

    Utils utils = myScene->getUtils();
    sizeMM.setWidth(utils.pixel2mm(sizePx.width()));
    sizeMM.setHeight(utils.pixel2mm(sizePx.height()));
  }
  else
  {
    sizeMM.setWidth(Utils::pixel2mm(sizePx.width(), m_dpiForCalculation));
    sizeMM.setHeight(Utils::pixel2mm(sizePx.height(), m_dpiForCalculation));
  }
  
}


bool  te::layout::TextController::needUpdateBox(const te::layout::Properties& properties)
{
  if (properties.contains("text") == false && properties.contains("font") == false)
  {
    return false;
  }
  return true;
}