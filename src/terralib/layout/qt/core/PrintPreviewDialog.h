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
  \file PrintPreviewDialog.h
   
  \brief Class responsible for show a preview of the entire content or part of the scene. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PRINT_PREVIEW_DIALOG_H
#define __TERRALIB_LAYOUT_INTERNAL_PRINT_PREVIEW_DIALOG_H

// TerraLib
#include "../../core/Config.h"

// Qt
#include <QPrintPreviewDialog>

class QPrinter;

namespace te
{
  namespace layout
  {
    class PaperConfig;
    /*!
    \brief Class responsible for show a preview of the entire content or part of the scene.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT PrintPreviewDialog : public QPrintPreviewDialog
    {
      Q_OBJECT //for slots/signals
        
      public:

        /*!
          \brief Constructor
      
          \param scene
        */
        PrintPreviewDialog(QPrinter * printer, QWidget * parent = 0, Qt::WindowFlags flags = 0);

        /*!
          \brief Destructor
        */
        virtual ~PrintPreviewDialog();
        
      //protected slots:


      protected:

        virtual void configureDialog();

        void disableAllButtons();

        void invisibleAllButtons();

        virtual void enableSomeButtons();

        virtual void visibleSomeButtons();

    };
  }
}

#endif 
