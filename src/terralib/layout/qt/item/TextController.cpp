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

#include "../core/pattern/command/ChangePropertyCommand.h"
#include "../../core/enum/EnumAlignmentType.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../qt/core/Scene.h"
#include "../../qt/item/AbstractItem.h"
#include "../../qt/item/TextItem.h"

#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QFontMetricsF>
#include <QTextCursor>

te::layout::TextController::TextController(AbstractItemModel* model)
  : AbstractItemController(model)
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

  if (propertiesCopy.contains("text") == false && propertiesCopy.contains("font") == false)
  {
    AbstractItemController::setProperties(propertiesCopy);
    return;
  }

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

  QPainterPath qPath = ItemUtils::textToVector(qText, qFont);
  QRectF rect = qPath.boundingRect();
  QSizeF size = rect.size();

  //We map to MM
  AbstractItem* absItem = (AbstractItem*)m_view;
  QSizeF sizeMM(size);

  Property propertyWidth(0);
  propertyWidth.setName("width");
  propertyWidth.setValue(sizeMM.width(), dataType->getDataTypeDouble());

  Property propertyHeight(0);
  propertyHeight.setName("height");
  propertyHeight.setValue(sizeMM.height(), dataType->getDataTypeDouble());

  propertiesCopy.addProperty(propertyWidth);
  propertiesCopy.addProperty(propertyHeight);

  absItem->prepareGeometryChange();

  AbstractItemController::setProperties(propertiesCopy);
}

/*
QSizeF te::layout::TextController::updateView()
{
  TextItem* view = dynamic_cast<TextItem*>(m_view);

  if(view != 0)
  {
    //reads the properties
    const Property& pText = getProperty("text");
    const Property& pFont = getProperty("font");
    const Property& pColor = getProperty("color");
    const Property& pAligment = getProperty("alignment");

    const std::string& text = pText.getValue().toString();
    const Font& ft = pFont.getValue().toFont();
    const te::color::RGBAColor& color = pColor.getValue().toColor();

    EnumAlignmentType enumAligmentType;
    const std::string& label = pAligment.getOptionByCurrentChoice().toString();
    EnumType* currentAligmentType = enumAligmentType.searchLabel(label);

    //converts information about the font
    QFont qft;

    std::string fontName = ft.getFamily();
    QString qFontName = ItemUtils::convert2QString(fontName);

    qft.setFamily(qFontName);
    qft.setBold(ft.isBold());
    qft.setItalic(ft.isItalic());
    qft.setKerning(ft.isKerning());
    qft.setStrikeOut(ft.isStrikeout());
    qft.setUnderline(ft.isUnderline());

    //we need to set the font size from PT to MM

    qft.setPointSizeF(ft.getPointSize());

    //converts the text
    QString qText = ItemUtils::convert2QString(text);

    //converts the color
    QColor qColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

    //converts information about the type
    QTextOption txtOpt = view->document()->defaultTextOption();
    if(currentAligmentType == enumAligmentType.getAlignmentCenterType())
    {

      txtOpt.setAlignment(Qt::AlignCenter);
      view->setTextWidth(-1);
      view->setTextWidth(view->boundingRect().width());

    }

    if(currentAligmentType == enumAligmentType.getAlignmentLeftType())
    {
      txtOpt.setAlignment(Qt::AlignLeft);
      view->setTextWidth(-1);
      view->setTextWidth(view->boundingRect().width());
    }

    if(currentAligmentType == enumAligmentType.getAlignmentRightType())
    {

      txtOpt.setAlignment(Qt::AlignRight);
      view->setTextWidth(-1);
      view->setTextWidth(view->boundingRect().width());

    }

    if (currentAligmentType == enumAligmentType.getAlignmentJustifyType())
    {

      txtOpt.setAlignment(Qt::AlignJustify);
      view->setTextWidth(-1);
      //setting width to broken text lines. Text width must be < than bounding rect width
      view->setTextWidth(view->boundingRect().width() -  0.01);
      
    }

    // We now synchronize the attributes of the view based on he attributes of the model
    bool changed = false;
    if(view->toPlainText() != qText)
    {
      view->setPlainText(qText);
      changed = true;
    }
    if(view->font() != qft)
    {
      view->setFont(qft);
      changed = true;
    }
    
    if(changed == true) 
    {
      view->setTextWidth(-1);
      view->setTextWidth(view->boundingRect().width());
    }
    if(view->document()->defaultTextOption().alignment() != txtOpt.alignment())
    {
      view->document()->setDefaultTextOption(txtOpt);
    }
    if(view->defaultTextColor() != qColor)
    {
      view->setDefaultTextColor(qColor);
    }

    //we get the size in pixels
    QRectF rect = view->boundingRect();
    QRectF boundingRect(QPointF(0, 0), rect.size());

    //then we convert to millimiters
    boundingRect = view->mapRectToScene(boundingRect);

    //we finally return the new size in millimiters
    return boundingRect.size();
  }
  return QSizeF();
}
*/

void te::layout::TextController::textChanged()
{
  /*
  TextItem* view = dynamic_cast<TextItem*>(m_view);
  if(view != 0)
  {
    const Property& pText = getProperty("text");
    std::string currentText = pText.getValue().toString();

    QString qCurrentText = ItemUtils::convert2QString(currentText);

    QString qNewText = view->toPlainText();

    if(qCurrentText != qNewText)
    {
      EnumDataType* dataType = Enums::getInstance().getEnumDataType();

      Property propertyText(0);
      propertyText.setName("text");

      std::string newText = ItemUtils::convert2StdString(qNewText);
      propertyText.setValue(newText, dataType->getDataTypeString());

      Properties oldCommand = m_model->getProperties();
      m_model->setProperty(propertyText);
      Properties newCommand = m_model->getProperties();

      QUndoCommand* command = new ChangePropertyCommand(view, oldCommand, newCommand);

      Scene* sc = dynamic_cast<Scene*>(view->scene());
      if(sc)
      {
        sc->addUndoStack(command);
      }
    }
  }
  */
}
