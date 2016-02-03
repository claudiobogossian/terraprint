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
  \file PrintScene.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PrintScene.h"
#include "../../core/PaperConfig.h"
#include "../../core/enum/Enums.h"
#include "BuildGraphicsOutside.h"
#include "../outside/PDFSettingsOutside.h"
#include "Scene.h"
#include "PrintPreviewDialog.h"

// STL
#include <sstream>

// Qt
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>

te::layout::PrintScene::PrintScene( QGraphicsScene* scene ):
  m_scene(scene),
  m_printState(te::layout::NoPrinter),
  m_currentPdfDpi(150)
{
}

te::layout::PrintScene::~PrintScene()
{
}

void te::layout::PrintScene::printPreview()
{
  if(!m_scene)
    return;

  Scene* sc = dynamic_cast<Scene*>(m_scene);
  if (!sc)
    return;

  ContextObject oldContext = sc->getContext();

  QPrinter* printer = createPrinter();
  printer->setOutputFormat(QPrinter::NativeFormat);
  
  PrintPreviewDialog *preview = new PrintPreviewDialog(printer);
  connect(preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPaper(QPrinter*)));

  if(preview->exec() == QDialog::Rejected || m_printState == te::layout::PrintingScene)
  {
    if(printer)
    {
      delete printer;
      printer = 0;
    }
  }

  if(preview)
  {
    delete preview;
    preview = 0;

    Scene* sc = dynamic_cast<Scene*>(m_scene);
    if(sc)
    {
      sc->redrawItems();
    }
  }

  sc->setContext(oldContext);
}

void te::layout::PrintScene::printPaper( QPrinter* printer )
{
  if(!printer)
    return;

  //Part print scene
  //It's not necessary to change the scale of View
  QPainter newPainter(printer);
  newPainter.setRenderHint(QPainter::Antialiasing);

  Scene* sc = dynamic_cast<Scene*>(m_scene);
  if(!sc)
    return;

  if (m_printState == NoPrinter)
  {
    m_printState = PreviewScene;
  }
  else if (m_printState == PreviewScene)
  {
    m_printState = PrintingScene;
  }

  ContextObject context = createNewContext(printer);
  sc->setContext(context);

  renderScene(&newPainter, printer);
}

QPrinter* te::layout::PrintScene::createPrinter()
{
  QPrinter* printer = 0;

  if(!m_scene)
    return printer;

  Scene* sc = dynamic_cast<Scene*>(m_scene);
  if(!sc)
    return printer;

  PaperConfig* config = sc->getPaperConfig();

  if(!config)
    return 0;

  double w = 0;
  double h = 0;
  config->getPaperSize(w, h);

  printer = new QPrinter(QPrinter::HighResolution);
  printer->setResolution(96);
  QSizeF sf(w, h);

  if (config->getPaperOrientantion() == Portrait)
  {
    printer->setOrientation(QPrinter::Portrait);
  }
  else
  {
    printer->setOrientation(QPrinter::Landscape);

    /*
    There is a bug in Qt 4 when the orientation is changed to "Landscape", so
    it is necessary to switch height and width manually.
    The error occurs because the default is "Portrait", but when it is changed to "Landscape" 
    QPrinter is rotating two times, once properly and the second makes return to "Portrait".
    This bug was corrected only in Qt 5.3.
    */
    sf.setHeight(w);
    sf.setWidth(h);
  }

  printer->pageRect(QPrinter::Millimeter);

  if (changePageSizeType(printer, config) == te::layout::Custom)
  {
    printer->setPaperSize(sf, QPrinter::Millimeter);
  }
    
  return printer;
}

void te::layout::PrintScene::renderScene( QPainter* newPainter, QPrinter* printer )
{
  if(!m_scene)
    return;

  Scene* sc = dynamic_cast<Scene*>(m_scene);
  if(!sc)
  {
    return;
  }

  if(!newPainter)
    return;

  if(!printer)
    return;
  
  double w = 0;
  double h = 0;

  PaperConfig* conf = sc->getPaperConfig();
  conf->getPaperSize(w, h);
        
  //Box Paper in the Scene (Source)
  QRectF mmSourceRect(0, 0, w, h);
  
  /* Gets the margins */
  qreal top = 0;
  qreal bottom = 0;
  qreal left = 0;
  qreal right = 0;
  QRect pageRect = printer->pageRect();

  //Paper size using the printer dpi (Target)
  //Convert Paper Size world to screen coordinate. Uses dpi printer.
  //Adjusts the destination box to use 100% of the paper, including the unprintable area.
  //In this case, items that are at the edge of the paper mu be cut off.
  QPointF origin(-pageRect.left(), pageRect.top());
  QSizeF paperPixelBox = printer->paperSize(QPrinter::DevicePixel);
  QRectF pxTargetRect(origin, paperPixelBox);

  //Mirroring Y-Axis
  newPainter->translate( paperPixelBox.width() / 2., paperPixelBox.height() / 2. );
  newPainter->scale( 1, -1 );
  newPainter->translate( -(paperPixelBox.width() / 2.), -(paperPixelBox.height() / 2.) );

  sc->deselectAllItems();

  QGraphicsItem* paperItem = sc->getPaperItem();
  paperItem->setVisible(false);

  QColor newBackgroundColor(255, 255, 255, 0);
  QBrush newBrush(newBackgroundColor);
  QBrush copyBackgroundColor = m_scene->backgroundBrush();

  m_scene->setBackgroundBrush(newBrush);//transparent
  m_scene->render(newPainter, pxTargetRect, mmSourceRect); 
  m_scene->setBackgroundBrush(copyBackgroundColor);

  paperItem->setVisible(true);
}

void te::layout::PrintScene::print()
{
  m_printState = PrintingScene;

  QPrinter* printer = createPrinter();
    
  printPaper(printer);

  if(printer)
  {
    delete printer;
    printer = 0;
  }

  Scene* sc = dynamic_cast<Scene*>(m_scene);
  if(sc)
  {
    sc->redrawItems();
  }

  m_printState = NoPrinter;
}

bool te::layout::PrintScene::exportToPDF()
{ 
  Scene* sc = dynamic_cast<Scene*>(m_scene);
  if (!sc)
    return false;

  BuildGraphicsOutside build;
  EnumObjectType* type = Enums::getInstance().getEnumObjectType();
  QWidget* outside = build.createOuside(type->getPDFSettingsDialog(), sc);
  PDFSettingsOutside* pdfSettings = dynamic_cast<PDFSettingsOutside*>(outside);
  pdfSettings->setCurrentDPI(m_currentPdfDpi);
    
  QString fileName;
  bool    willExport = true;
  
  int result = pdfSettings->exec();  
        
  if (result == QDialog::Accepted)
  {
    std::string txt = pdfSettings->getFilePath();
    fileName = QString::fromStdString(txt);
    m_currentPdfDpi = pdfSettings->getDPI();
  }
  else if (result == QDialog::Rejected)
  {
    willExport = false;
  }
      
  if (pdfSettings)
  {
    delete pdfSettings;
    pdfSettings = 0;
  }

  if (!willExport || fileName.isEmpty())
  {
    return false;
  }

  QPrinter* printer= createPrinter();

  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setOutputFileName(fileName);

  printer->setResolution(m_currentPdfDpi);

  m_printState = PrintingScene;

  printPaper(printer);

  QPrinter::PrinterState state = printer->printerState();

  if(printer)
  {
    delete printer;
    printer = 0;
  } 

  QMessageBox msgBox;
  if (state == QPrinter::Error)
  {
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("Could not export the PDF! Possible cause: the file is already opened by another application.");
  }
  else
  {
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("PDF exported successfully!");
  }
  msgBox.exec();

  sc->redrawItems();  

  m_printState = NoPrinter;

  return true;
}

te::layout::ContextObject te::layout::PrintScene::createNewContext( QPrinter* printer )
{
  ContextObject invalidContext(0,0,0,0);

  if(!m_scene)
    return invalidContext;

  Scene* sc = dynamic_cast<Scene*>(m_scene);
  if(!sc)
    return invalidContext;

  double dpiX = printer->logicalDpiX();
  double dpiY = printer->logicalDpiY();
  int zoom = 100;
  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();
  EnumType* mode = 0;
  if (m_printState == PreviewScene)
  {
    mode = enumMode->getModePrinterPreview();
  }
  else if (m_printState == PrintingScene)
  {
    mode = enumMode->getModePrinter();
  }

  ContextObject context(zoom, dpiX, dpiY, mode);
  return context;
}

te::layout::LayoutAbstractPaperType te::layout::PrintScene::changePageSizeType(QPrinter* printer, PaperConfig* paperConfig)
{
  te::layout::LayoutAbstractPaperType type = te::layout::Custom;

  if (paperConfig->getPaperType() == te::layout::A0)
  {
    printer->setPaperSize(QPrinter::A0);
    type = te::layout::A0;
  }
  else if (paperConfig->getPaperType() == te::layout::A1)
  {
    printer->setPaperSize(QPrinter::A1);
    type = te::layout::A1;
  }
  else if (paperConfig->getPaperType() == te::layout::A2)
  {
    printer->setPaperSize(QPrinter::A2);
    type = te::layout::A2;
  }
  else if (paperConfig->getPaperType() == te::layout::A3)
  {
    printer->setPaperSize(QPrinter::A3);
    type = te::layout::A3;
  }
  else if (paperConfig->getPaperType() == te::layout::A4)
  {
    printer->setPaperSize(QPrinter::A4);
    type = te::layout::A4;
  }
  else if (paperConfig->getPaperType() == te::layout::A5)
  {
    printer->setPaperSize(QPrinter::A5);
    type = te::layout::A5;
  }
  else 
  {
    printer->setPaperSize(QPrinter::Custom);
  }

  return type;
}

