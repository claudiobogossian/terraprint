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

#include "../../core/ItemInputProxy.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/enum/EnumAlignmentType.h"
#include "../../qt/core/Scene.h"
#include "../../qt/item/AbstractItem.h"
#include "../../qt/item/TextItem.h"


#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QTextCursor>
#include <QColor>

te::layout::TextController::TextController(AbstractItemModel* model, AbstractItemView* view)
  : AbstractItemController(model, view)
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
  
  bool hasName = properties.contains("name");
  bool hasText = properties.contains("text");
  if (hasName && !hasText)
  {
    const Property& propName = properties.getProperty("name");
    const Property& propText = getProperty("text");
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    std::string name = te::layout::Property::GetValueAs<std::string>(propName);
    std::string text = te::layout::Property::GetValueAs<std::string>(propText);

    if (name.compare("") != 0 && text.compare("") == 0)
    {
      Property propText(0);
      propText.setName("text");
      propText.setValue(name, dataType->getDataTypeString());
      propertiesCopy.addProperty(propText);
    }
  }

  if (needUpdateBox(propertiesCopy))
  {
    TextItem* textItem = dynamic_cast<TextItem*>(m_view);
    QSizeF sizeMM;

    double dx = 0.;
    double dy = 0.;

    calculateSize(propertiesCopy, sizeMM, dx, dy);

    Property propertyWidth(0);
    propertyWidth.setName("width");
    propertyWidth.setValue<double>(sizeMM.width(), dataType->getDataTypeDouble());

    Property propertyHeight(0);
    propertyHeight.setName("height");
    propertyHeight.setValue<double>(sizeMM.height(), dataType->getDataTypeDouble());

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
  }
  else if (properties.contains("height"))
  {
    //if only the height have been changed, we must recalculate the font size based on it
    const Property& pCurrentHeight = this->getProperty("height");
    const Property& pNewHeight = propertiesCopy.getProperty("height");

    double currentHeight = te::layout::Property::GetValueAs<double>(pCurrentHeight);
    double newHeight = te::layout::Property::GetValueAs<double>(pNewHeight);
    double resizeFactor = newHeight / currentHeight;

    double ptSizeMM = 0.353;//size of one point (font measure), in millimeters
    double newFontSizeDouble = std::floor(newHeight / ptSizeMM);
    int newFontSize = (int)newFontSizeDouble;

    te::layout::Property pNewFont = this->getProperty("font"); //copies the current property
    Font newFont = te::layout::Property::GetValueAs<Font>(pNewFont);
    newFont.setPointSize(qRound(newFont.getPointSize() * resizeFactor));

    pNewFont.setValue(newFont, dataType->getDataTypeFont());
    propertiesCopy.addProperty(pNewFont);

    //as we chenged the font size, we must recalcute the other properties. So we call this function recursively
    setProperties(propertiesCopy);
    return;
  }

  AbstractItemController::setProperties(propertiesCopy);
}

double te::layout::TextController::getDpiForCalculation() const
{
  return m_dpiForCalculation;
}

QTextDocument* te::layout::TextController::createTextDocument(const te::layout::Properties& properties)
{
  const te::layout::Property& pText = getProperty("text", properties);
  const te::layout::Property& pFont = getProperty("font", properties);
  const te::layout::Property& pAligment = getProperty("alignment", properties);

  QString qText = ItemUtils::convert2QString(te::layout::Property::GetValueAs<std::string>(pText));
  QFont qFont = ItemUtils::convertToQfont(te::layout::Property::GetValueAs<Font>(pFont));
  EnumAlignmentType enumAligmentType;
  const std::string& label = pAligment.getOptionByCurrentChoice().toString();
  EnumType* currentAligmentType = enumAligmentType.getEnum(label);

  QTextOption textOption;

  if (currentAligmentType)
  {
    if (currentAligmentType == enumAligmentType.getAlignmentLeftType())
    {
      textOption.setAlignment(Qt::AlignLeft);
    }
    else if (currentAligmentType == enumAligmentType.getAlignmentRightType())
    {
      textOption.setAlignment(Qt::AlignRight);
    }
    else if (currentAligmentType == enumAligmentType.getAlignmentCenterType())
    {
      textOption.setAlignment(Qt::AlignCenter);
    }
    else if (currentAligmentType == enumAligmentType.getAlignmentJustifyType())
    {
      textOption.setAlignment(Qt::AlignJustify);
    }
  }

  QTextDocument* textDocument = new QTextDocument();
  textDocument->setTextWidth(-1);
  textDocument->setDefaultFont(qFont);
  textDocument->setPlainText(qText);
  textDocument->setDocumentMargin(0);
  textDocument->setDefaultTextOption(textOption);
  textDocument->setTextWidth(textDocument->size().width());

  return textDocument;
}


void te::layout::TextController::calculateSize(const te::layout::Properties& properties, QSizeF& sizeMM, double& dx, double& dy)
{
  QScopedPointer<QTextDocument> textDocument(createTextDocument(properties));
  QSizeF sizePx = textDocument->size();

  if (m_view == 0)
  {
    sizeMM.setWidth(Utils::pixel2mm(sizePx.width(), m_dpiForCalculation));
    sizeMM.setHeight(Utils::pixel2mm(sizePx.height(), m_dpiForCalculation));
    return;
  }

  TextItem* textItem = dynamic_cast<TextItem*>(m_view);
  ItemInputProxy* itemInputProxy = m_view->getItemInputProxy();
  if (itemInputProxy != 0)
  {
    m_dpiForCalculation = itemInputProxy->getContext().getDpiX();

    Utils utils = itemInputProxy->getUtils();
    sizeMM.setWidth(utils.pixel2mm(sizePx.width()));
    sizeMM.setHeight(utils.pixel2mm(sizePx.height()));
  }
  else
  {
    sizeMM.setWidth(Utils::pixel2mm(sizePx.width(), m_dpiForCalculation));
    sizeMM.setHeight(Utils::pixel2mm(sizePx.height(), m_dpiForCalculation));
  }
}


bool te::layout::TextController::needUpdateBox(const te::layout::Properties& properties)
{
  if (properties.contains("text") == false && properties.contains("font") == false)
  {
    return false;
  }
  return true;
}

void te::layout::TextController::refresh()
{
  QTextDocument* textDocument = createTextDocument(this->getProperties());

  TextItem* textItem = dynamic_cast<TextItem*>(m_view);
  textItem->setDocument(textDocument);

  AbstractItemController::refresh();
}

QColor te::layout::TextController::getCurrentTextColor()
{
  const te::layout::Property& pTextColor = getProperty("color");
  const te::color::RGBAColor& textColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pTextColor);
  QColor qTextColor(textColor.getRed(), textColor.getGreen(), textColor.getBlue(), textColor.getAlpha());
  return qTextColor;
}

