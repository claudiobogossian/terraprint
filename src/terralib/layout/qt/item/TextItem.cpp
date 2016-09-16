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
  \file TextItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextItem.h"
#include "TextController.h"
#include "../../qt/core/Scene.h"
#include "../core/ItemUtils.h"
#include "../../core/enum/EnumAlignmentType.h"
#include "../../core/pattern/singleton/Context.h"


// STL
#include <string>

// Qt
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QTextCursor>
#include <QPainter>
#include <QTextOption>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

te::layout::TextItem::TextItem(AbstractItemController* controller)
  : AbstractItem(controller)
  , m_textCursor(0)
{  
  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(false);
  setCursor(Qt::ArrowCursor); // default cursor

  QTextDocument* document = new QTextDocument();

  m_textCursor = new QTextCursor(document);
  m_textCursor->document()->setDefaultCursorMoveStyle(Qt::VisualMoveStyle);
}

te::layout::TextItem::~TextItem()
{
  delete m_textCursor->document();
  delete m_textCursor;
}

QRectF te::layout::TextItem::boundingRect() const
{
  if (m_isEditionMode == false)
  {
    return AbstractItem::boundingRect();
  }

  AbstractScene* myScene = this->getScene();
  if (myScene == 0)
  {
    return AbstractItem::boundingRect();
  }

  Utils utils = myScene->getUtils();

  QSizeF documentSizePx = m_textCursor->document()->size();

  double widthMM = utils.pixel2mm(documentSizePx.width());
  double heightMM = utils.pixel2mm(documentSizePx.height());

  //when we are editing the item, we let the item handle the changes in the bounding box
  QRectF rect(0, 0, widthMM, heightMM);
  return rect;
}

void te::layout::TextItem::refresh()
{
  const te::layout::Property& pText = m_controller->getProperty("text");  
  const te::layout::Property& pFont = m_controller->getProperty("font");
  const Property& pAligment = m_controller->getProperty("alignment");

  QString qText = ItemUtils::convert2QString(pText.getValue().toString());
  QFont qFont = ItemUtils::convertToQfont(pFont.getValue().toFont());
  EnumAlignmentType enumAligmentType;
  const std::string& label = pAligment.getOptionByCurrentChoice().toString();
  EnumType* currentAligmentType = enumAligmentType.searchLabel(label);

  QTextOption textOption;
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

  m_textCursor->document()->setTextWidth(-1);
  m_textCursor->document()->setDefaultFont(qFont);
  m_textCursor->document()->setPlainText(qText);
  m_textCursor->document()->setDocumentMargin(0);
  m_textCursor->document()->setDefaultTextOption(textOption);
  m_textCursor->document()->setTextWidth(m_textCursor->document()->size().width());

  AbstractItem::refresh();
}

void te::layout::TextItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  AbstractScene* myScene = this->getScene();
  if (myScene == 0)
  {
    return;
  }

  TextController* textController = (TextController*)m_controller;

  //we must set some transformation in order to prepare the item to be rendered on the screen or in other output device
  //we must first aquire some information about the current dpi and the zoom
  int zoom = myScene->getContext().getZoom();
  double zoomFactor = zoom / 100.;
  double dpiFactor = myScene->getContext().getDpiX() / textController->getDpiForCalculation();

  //the we aquire information about the bounding rect references in pixels and in millimeters
  QRectF boxMM = this->boundingRect();
  QRect boxPixels = painter->transform().mapRect(boxMM).toRect();

  QAbstractTextDocumentLayout::PaintContext context;

  //if we are in edition mode, we must draw the cursor position and the selection
  if (isEditionMode())
  {
    //we define the position
    context.cursorPosition = m_textCursor->position();

    //we defined the selection
    QAbstractTextDocumentLayout::Selection selection;
    selection.cursor = QTextCursor(*m_textCursor);
    selection.format.setBackground(QBrush(Qt::blue));
    selection.format.setForeground(QBrush(Qt::white));
    context.selections.append(selection);
  }

  painter->save();

  //we finally set the transformation into the qpainter
  QTransform transform;
  transform.scale(zoomFactor * dpiFactor, zoomFactor * dpiFactor);

  painter->setViewport(boxPixels);
  painter->setWindow(0, 0, boxPixels.width(), boxPixels.height());
  painter->setTransform(transform);

  //and here we effectivally asks the textLayout to draw the document
  QAbstractTextDocumentLayout* documentLayout = m_textCursor->document()->documentLayout();

  documentLayout->draw(painter, context);

  painter->restore();
}

void te::layout::TextItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  if (isEditionMode() == false)
  {
    QGraphicsItem::mousePressEvent(event);
    return;
  }
  

  if (event->button() != Qt::LeftButton)
  {
    return;
  }

  AbstractScene* myScene = this->getScene();
  if (myScene == 0)
  {
    return;
  }

  Utils utils = myScene->getUtils();

  double xPos = utils.mm2pixel(event->pos().x());
  double yPos = utils.mm2pixel(boundingRect().height()) - utils.mm2pixel(event->pos().y());

  int newPosition = m_textCursor->document()->documentLayout()->hitTest(QPointF(xPos, yPos), Qt::FuzzyHit);
  if (newPosition != -1)
  {
    m_textCursor->setPosition(newPosition);
  }

  event->accept();
}

void te::layout::TextItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  if (isEditionMode() == false)
  {
    QGraphicsItem::mouseMoveEvent(event);
    return;
  }

  if (!(event->buttons() & Qt::LeftButton))
  {
    return;
  }

  AbstractScene* myScene = this->getScene();
  if (myScene == 0)
  {
    return;
  }
  
  Utils utils = myScene->getUtils();

  double xPos = utils.mm2pixel(event->pos().x());
  double yPos = utils.mm2pixel(boundingRect().height()) - utils.mm2pixel(event->pos().y());

  int newPosition = m_textCursor->document()->documentLayout()->hitTest(QPointF(xPos, yPos), Qt::FuzzyHit);
  if (newPosition != -1 && newPosition != m_textCursor->position())
  {
    m_textCursor->setPosition(newPosition, QTextCursor::KeepAnchor);
    update();
  }

  event->accept();
}

void te::layout::TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
  if(event->button() == Qt::LeftButton)
  {
    if(m_isEditionMode == true)
    {
      enterEditionMode();
    }
  }
}

void te::layout::TextItem::keyPressEvent(QKeyEvent * event)
{
  if (isEditionMode() == false)
  {
    AbstractItem::keyPressEvent(event);
    return;
  }

  event->accept();

  QTextCursor::MoveMode moveMode = QTextCursor::MoveAnchor;
  if (event->modifiers() & Qt::ShiftModifier)
  {
    moveMode = QTextCursor::KeepAnchor;
  }

  m_textCursor->document()->setTextWidth(-1);
  switch (event->key())
  {
  case Qt::Key_Backspace:
    m_textCursor->deletePreviousChar();
    break;
  case Qt::Key_Delete:
    m_textCursor->deleteChar();
    break;
  case Qt::Key_Left:
    m_textCursor->movePosition(QTextCursor::Left, moveMode);
    break;
  case Qt::Key_Right:
    m_textCursor->movePosition(QTextCursor::Right, moveMode);
    break;
  case Qt::Key_Up:
    m_textCursor->movePosition(QTextCursor::Up, moveMode);
    break;
  case Qt::Key_Down:
    m_textCursor->movePosition(QTextCursor::Down, moveMode);
    break;
  case Qt::Key_Home:
    if(event->modifiers() & Qt::ControlModifier)
      m_textCursor->movePosition(QTextCursor::Start, moveMode);
    else
      m_textCursor->movePosition(QTextCursor::StartOfLine, moveMode);
    break;
  case Qt::Key_End:
    if (event->modifiers() & Qt::ControlModifier)
      m_textCursor->movePosition(QTextCursor::End, moveMode);
    else
      m_textCursor->movePosition(QTextCursor::EndOfLine, moveMode);
    break;
  default:
    m_textCursor->insertText(event->text());
    break;
  }

  m_textCursor->document()->setTextWidth(m_textCursor->document()->size().width());
  update();
}

void te::layout::TextItem::enterEditionMode()
{
  AbstractItem::enterEditionMode();

  setCursor(Qt::IBeamCursor);

  m_textCursor->movePosition(QTextCursor::Start);
  m_textCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
}

void te::layout::TextItem::leaveEditionMode()
{
  AbstractItem::leaveEditionMode();

  setCursor(Qt::ArrowCursor);

  QString qNewText = m_textCursor->document()->toPlainText();
  std::string newText = ItemUtils::convert2StdString(qNewText);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property propertyText(0);
  propertyText.setName("text");
  propertyText.setValue(newText, dataType->getDataTypeString());

  m_controller->setProperty(propertyText);
}
