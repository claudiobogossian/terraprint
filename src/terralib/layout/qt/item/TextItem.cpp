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
#include <QApplication>
#include <QClipboard>

te::layout::TextItem::TextItem(AbstractItemController* controller)
  : AbstractItem(controller)
  , m_textCursor(0)
{  
  //If enabled is true, this item will accept hover events
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

  Utils utils = myScene->getUtils();
  double widthPx = utils.mm2pixel(boxMM.width());
  double heightPx = utils.mm2pixel(boxMM.height());

  double conversionfactor = boxMM.width() / widthPx;
  
  //if we are in edition mode, we must draw the cursor position and the selection
  QAbstractTextDocumentLayout::PaintContext context;
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
  painter->setRenderHint(QPainter::Antialiasing, true);

  //we finally set the transformation into the qpainter
  QTransform transform;
  transform.translate(0, boxMM.height());
  transform.scale(dpiFactor * conversionfactor, dpiFactor * conversionfactor * -1);
  painter->setTransform(transform, true);

  //and here we effectivally asks the textLayout to draw the document
  QAbstractTextDocumentLayout* documentLayout = m_textCursor->document()->documentLayout();

  documentLayout->draw(painter, context);

  painter->restore();
}

void te::layout::TextItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  if (isEditionMode() == false)
  {
    AbstractItem::mousePressEvent(event);
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
    update();
  }

  event->accept();
}

void te::layout::TextItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  if (isEditionMode() == false)
  {
    AbstractItem::mouseMoveEvent(event);
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
      m_textCursor->select(QTextCursor::WordUnderCursor);
      update();
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

  QTextCursor::MoveMode moveMode = QTextCursor::MoveAnchor;
  if (event->modifiers() & Qt::ShiftModifier)
  {
    moveMode = QTextCursor::KeepAnchor;
  }

  m_textCursor->document()->setTextWidth(-1);
  if (event->key() < Qt::Key_Escape && !(event->modifiers() & Qt::ControlModifier))
  {
    m_textCursor->insertText(event->text());
  }
  //here we handle the navigation
  else if (event == QKeySequence::MoveToNextChar || event == QKeySequence::SelectNextChar)
  {
    m_textCursor->movePosition(QTextCursor::Right, moveMode);
  }
  else if (event == QKeySequence::MoveToPreviousChar || event == QKeySequence::SelectPreviousChar)
  {
    m_textCursor->movePosition(QTextCursor::Left, moveMode);
  }
  else if (event == QKeySequence::MoveToNextLine || event == QKeySequence::SelectNextLine)
  {
    m_textCursor->movePosition(QTextCursor::Down, moveMode);
  }
  else if (event == QKeySequence::MoveToPreviousLine || event == QKeySequence::SelectPreviousLine)
  {
    m_textCursor->movePosition(QTextCursor::Up, moveMode);
  }
  else if (event == QKeySequence::MoveToStartOfLine || event == QKeySequence::SelectStartOfLine)
  {
    m_textCursor->movePosition(QTextCursor::StartOfLine, moveMode);
  }
  else if (event == QKeySequence::MoveToEndOfLine || event == QKeySequence::SelectEndOfLine)
  {
    m_textCursor->movePosition(QTextCursor::EndOfLine, moveMode);
  }
  else if (event == QKeySequence::MoveToStartOfDocument || event == QKeySequence::SelectStartOfDocument)
  {
    m_textCursor->movePosition(QTextCursor::Start, moveMode);
  }
  else if (event == QKeySequence::MoveToEndOfDocument || event == QKeySequence::SelectEndOfDocument)
  {
    m_textCursor->movePosition(QTextCursor::End, moveMode);
  }
  else if (event == QKeySequence::SelectAll)
  {
    m_textCursor->select(QTextCursor::Document);
  }
  //here we handle the edition keys
  else if (event == QKeySequence::InsertParagraphSeparator)
  {
    m_textCursor->insertText(event->text());
  }
  else if (event->key() == Qt::Key_Backspace) //we need a special handle here
  {
    m_textCursor->deletePreviousChar();
  }
  else if (event == QKeySequence::Delete)
  {
    m_textCursor->deleteChar();
  }
  else if (event == QKeySequence::Copy)
  {
    QApplication::clipboard()->setText(m_textCursor->selectedText(), QClipboard::Clipboard);
  }
  else if (event == QKeySequence::Cut)
  {
    QApplication::clipboard()->setText(m_textCursor->selectedText(), QClipboard::Clipboard);
    m_textCursor->removeSelectedText();
  }
  else if (event == QKeySequence::Paste)
  {
    m_textCursor->insertText(QApplication::clipboard()->text(QClipboard::Clipboard));
  }
  else if (event == QKeySequence::Undo)
  {
    m_textCursor->document()->undo();
  }
  else if (event == QKeySequence::Redo)
  {
    m_textCursor->document()->redo();
  }
  else
  {
    event->ignore();
    return;
  }

  event->accept();

  prepareGeometryChange();
  m_textCursor->document()->setTextWidth(m_textCursor->document()->size().width());
  update();
}

void te::layout::TextItem::enterEditionMode()
{
  AbstractItem::enterEditionMode();

  setCursor(Qt::IBeamCursor);

  m_textCursor->select(QTextCursor::Document);
}

void te::layout::TextItem::leaveEditionMode()
{
  AbstractItem::leaveEditionMode();

  m_textCursor->document()->clearUndoRedoStacks();

  setCursor(Qt::ArrowCursor);

  QString qNewText = m_textCursor->document()->toPlainText();
  std::string newText = ItemUtils::convert2StdString(qNewText);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property propertyText(0);
  propertyText.setName("text");
  propertyText.setValue(newText, dataType->getDataTypeString());

  m_controller->setProperty(propertyText);
}
