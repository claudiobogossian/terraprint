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
  \file PrintScene.h
   
  \brief Class responsible for printing the entire content or part of the scene. As the scene is upside down, it is necessary to invert the y of the painter before printing.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PRINT_SCENE_H
#define __TERRALIB_LAYOUT_INTERNAL_PRINT_SCENE_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/enum/AbstractType.h"
#include "../../core/ContextObject.h"

// Qt
#include <QObject>
#include <QRectF>

class QGraphicsScene;
class QPrinter;
class QPainter;
class QPaintDevice;
class QString;

namespace te
{
  namespace layout
  {
    class PaperConfig;
    class ExportSettingsOutside;
    /*!
    \brief Class responsible for printing the entire content or part of the scene. As the scene is upside down, it is necessary to invert the y of the painter before printing.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT PrintScene : public QObject
    {
      Q_OBJECT //for slots/signals
        
      public:

        /*!
          \brief Constructor
      
          \param scene
        */
        PrintScene( QGraphicsScene* scene );

        /*!
          \brief Destructor
        */
        virtual ~PrintScene();

        /*!
          \brief Shows a preview window before printing(only the area corresponding to the paper).
        */
        virtual void showPrintPreviewDialog();

        /*!
          \brief Shows a window to print the scene. Allows to choose and configure the printer.
        */
        virtual void showPrintDialog();

        /*!
        \brief Shows the QPrinterDialog to print the scene. Allows to choose and configure the printer.
        */
        virtual void showQPrinterDialog();

        /*!
          \brief Print the scene(only the area corresponding to the paper).
        */
        virtual bool print();

        virtual bool exportAs(const std::string& fileFormat = "");

      protected slots:

        virtual bool printPaper(QPrinter* printer);

      protected:

        virtual void initPrinter(QPrinter* printer);

        virtual bool renderSceneOnPrinter(QPrinter* printer);

        virtual ContextObject createNewContext(QPrinter* printer);

        virtual te::layout::LayoutAbstractPaperType changePageSizeType(QPrinter* printer, PaperConfig* paperConfig);

        virtual bool render(QPaintDevice* device, const QRectF& pixelTargetRect, const ContextObject& context);

        QRectF paperRectMM();

        /*!
        \brief Export scene to pdf(only the area corresponding to the paper).
        */
        virtual bool exportToPDF(const QString& filePath);

        /*!
        \brief Export scene to SVG (only the area corresponding to the paper).
        */
        virtual bool exportToSVG(const QString& filePath);

        /*!
        \brief Export scene to SVG (only the area corresponding to the paper).
        */
        virtual bool exportToImage(const QString& filePath, const QString& fileFormat);

        ExportSettingsOutside* createExportSettingsOutside(const std::string& fileFormat = "");

        virtual bool exportTo(const QString& filePath, const QString& fileFormat);

        virtual void raiseMessage(const QString& fileFormat, bool hasError);

        QRectF imagePixelTargetRect(const ContextObject& context);
        
      protected:

        QGraphicsScene*           m_scene;
        te::layout::PrinterScene  m_printState;
        int                       m_currentDPI;
    };
  }
}

#endif 
