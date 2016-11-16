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
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/enum/EnumAlignmentType.h"
#include "../../core/pattern/singleton/Context.h"


// STL
#include <string>

// Qt
#include <QTextBlock>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QTextCursor>
#include <QPainter>
#include <QTextOption>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QTimer>

te::layout::TextItem::TextItem(AbstractItemController* controller)
  : QObject()
  , AbstractItem(controller)
  , m_document(0)
  , m_textCursor(0)
  , m_cursorTimer(0)
  , m_showCursor(true)
  , m_blockEditionRangeStart(0)
  , m_blockEditionRangeEnd(0)
{  
  //If enabled is true, this item will accept hover events
  setCursor(Qt::ArrowCursor); // default cursor

  m_document = new QTextDocument();
  m_document->setDefaultCursorMoveStyle(Qt::VisualMoveStyle);

}

te::layout::TextItem::~TextItem()
{
  delete m_textCursor;
  delete m_document;
}

void te::layout::TextItem::setDocument(QTextDocument* textDocument)
{
  if (m_document != 0)
  {
    delete m_document;
  }

  m_document = textDocument;

  updateBlockEditionRange();
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

  const Property& pDx = m_controller->getProperty("dx");
  double dx = te::layout::Property::GetValueAs<double>(pDx);

  const Property& pDy = m_controller->getProperty("dy");
  double dy = te::layout::Property::GetValueAs<double>(pDy);

  Utils utils = myScene->getUtils();

  QSizeF documentSizePx = m_document->size();

  double widthMM = utils.pixel2mm(documentSizePx.width());
  double heightMM = utils.pixel2mm(documentSizePx.height());

  //when we are editing the item, we let the item handle the changes in the bounding box
  QRectF rect(dx, dy, widthMM, heightMM);
  return rect;
}

void te::layout::TextItem::refresh()
{
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
    if (m_showCursor)
    {
      context.cursorPosition = m_textCursor->position();
    }

    //we defined the selection
    QAbstractTextDocumentLayout::Selection selection;
    selection.cursor = QTextCursor(*m_textCursor);
    selection.format.setBackground(QBrush(Qt::blue));
    selection.format.setForeground(QBrush(Qt::white));
    context.selections.append(selection);
  }

  painter->save();
  painter->setRenderHint(QPainter::Antialiasing, true);

  const Property& pDx = textController->getProperty("dx");
  double dx = te::layout::Property::GetValueAs<double>(pDx);

  const Property& pDy = textController->getProperty("dy");
  double dy = te::layout::Property::GetValueAs<double>(pDy);

  double dxPixels = dx / (dpiFactor * conversionfactor);
  double dyPixels = dy / (dpiFactor * conversionfactor);

  //we finally set the transformation into the qpainter
  QTransform transform;
  transform.scale(dpiFactor * conversionfactor, dpiFactor * conversionfactor * -1);
  transform.translate(dxPixels, -m_document->size().height() - dyPixels);
  painter->setTransform(transform, true);

  //and here we effectivally asks the textLayout to draw the document
  QAbstractTextDocumentLayout* documentLayout = m_document->documentLayout();

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

  //me must consider the optional displacement
  const Property& pDx = m_controller->getProperty("dx");
  double dx = te::layout::Property::GetValueAs<double>(pDx);

  const Property& pDy = m_controller->getProperty("dy");
  double dy = te::layout::Property::GetValueAs<double>(pDy);

  double xPosMM = event->pos().x() - dx;
  double yPosMM = boundingRect().height() - event->pos().y() + dy;

  Utils utils = myScene->getUtils();
  double xPos = utils.mm2pixel(xPosMM);
  double yPos = utils.mm2pixel(yPosMM);

  int newPosition = m_document->documentLayout()->hitTest(QPointF(xPos, yPos), Qt::FuzzyHit);
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

  int newPosition = m_document->documentLayout()->hitTest(QPointF(xPos, yPos), Qt::FuzzyHit);
  if (newPosition != -1 && newPosition != m_textCursor->position())
  {
    m_textCursor->setPosition(newPosition, QTextCursor::KeepAnchor);
    update();
  }

  event->accept();
}

void te::layout::TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
  if (isEditionMode() == false)
  {
    AbstractItem::mouseDoubleClickEvent(event);
    return;
  }

  if(event->button() == Qt::LeftButton)
  {
    m_textCursor->select(QTextCursor::WordUnderCursor);
    update();
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

  m_document->setTextWidth(-1);
  if (event->key() < Qt::Key_Escape && !(event->modifiers() & Qt::ControlModifier))
  {
    m_textCursor->insertText(event->text());
  }
  //here we handle the navigation
  else if (event == QKeySequence::MoveToNextChar || event == QKeySequence::SelectNextChar)
  {
    int currentPosition = m_textCursor->position();
    m_textCursor->movePosition(QTextCursor::Right, moveMode);
    if (m_textCursor->blockNumber() > m_blockEditionRangeEnd)
    {
      m_textCursor->setPosition(currentPosition, moveMode);
    }
  }
  else if (event == QKeySequence::MoveToPreviousChar || event == QKeySequence::SelectPreviousChar)
  {
    int currentPosition = m_textCursor->position();
    m_textCursor->movePosition(QTextCursor::Left, moveMode);
    if (m_textCursor->blockNumber() < m_blockEditionRangeStart)
    {
      m_textCursor->setPosition(currentPosition, moveMode);
    }
  }
  else if (event == QKeySequence::MoveToNextLine || event == QKeySequence::SelectNextLine)
  {
    int currentPosition = m_textCursor->position();
    m_textCursor->movePosition(QTextCursor::Down, moveMode);
    if (m_textCursor->blockNumber() > m_blockEditionRangeEnd)
    {
      m_textCursor->setPosition(currentPosition, moveMode);
    }
  }
  else if (event == QKeySequence::MoveToPreviousLine || event == QKeySequence::SelectPreviousLine)
  {
    int currentPosition = m_textCursor->position();
    m_textCursor->movePosition(QTextCursor::Up, moveMode);
    if (m_textCursor->blockNumber() < m_blockEditionRangeStart)
    {
      m_textCursor->setPosition(currentPosition, moveMode);
    }
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
    //keeps inside the defined block range, ignoring the document range
    int position = m_document->findBlockByNumber(m_blockEditionRangeStart).position();
    m_textCursor->setPosition(position, moveMode);
  }
  else if (event == QKeySequence::MoveToEndOfDocument || event == QKeySequence::SelectEndOfDocument)
  {
    //keeps inside the defined block range, ignoring the document range
    int position = m_document->findBlockByNumber(m_blockEditionRangeEnd).position();
    m_textCursor->setPosition(position, moveMode);
    m_textCursor->movePosition(QTextCursor::EndOfBlock, moveMode);
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
    m_document->undo();
  }
  else if (event == QKeySequence::Redo)
  {
    m_document->redo();
  }
  else
  {
    event->ignore();
    return;
  }

  event->accept();

  prepareGeometryChange();
  m_document->setTextWidth(m_document->size().width());
  updateBlockEditionRange();
  update();
}

void te::layout::TextItem::enterEditionMode()
{
  AbstractItem::enterEditionMode();

  setCursor(Qt::IBeamCursor);

  m_textCursor = new QTextCursor(m_document);
  m_textCursor->select(QTextCursor::Document);

  m_cursorTimer = new QTimer(this);
  connect(m_cursorTimer, SIGNAL(timeout()), this, SLOT(timerEvent()));

  m_cursorTimer->setInterval(500);
  m_cursorTimer->start();
}

void te::layout::TextItem::leaveEditionMode()
{
  AbstractItem::leaveEditionMode();

  //we clean the view objects
  delete m_cursorTimer;
  m_cursorTimer = 0;

  m_document->clearUndoRedoStacks();
  delete m_textCursor;
  m_textCursor = 0;

  setCursor(Qt::ArrowCursor);

  documentEditionFinished();
}

void te::layout::TextItem::documentEditionFinished()
{
  //and then we update the model with the new values
  QString qNewText = m_document->toPlainText();
  std::string newText = ItemUtils::convert2StdString(qNewText);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property propertyText(0);
  propertyText.setName("text");
  propertyText.setValue(newText, dataType->getDataTypeString());

  m_controller->setProperty(propertyText);
}


void te::layout::TextItem::timerEvent()
{
  m_showCursor = !m_showCursor;
  update();
}

void te::layout::TextItem::updateBlockEditionRange()
{
  m_blockEditionRangeStart = m_document->firstBlock().blockNumber();
  m_blockEditionRangeEnd = m_document->lastBlock().blockNumber();
}
