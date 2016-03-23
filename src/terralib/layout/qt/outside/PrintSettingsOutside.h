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
  \file terralib/layout/qt/outside/PrintSettingsOutside.h

  \brief
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PRINT_SETTINGS_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_PRINT_SETTINGS_OUTSIDE_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractOutsideView.h"

// STL
#include <memory>

// Qt
#include <QDialog>
#include <QString>

class QPrinter;
class QListWidgetItem;

namespace Ui { class PrintSettings; }

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;
    class Scene;

    class TELAYOUTEXPORT PrintSettingsOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT

      public:

        PrintSettingsOutside(Scene* scene, AbstractOutsideController* controller, QWidget* parent);

        virtual ~PrintSettingsOutside();

        /*!
          \brief Load layers in double widget.
        */
        virtual void init();

        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();

        virtual QString getPrinterName();

        virtual int getDPI();

        virtual void setPrinter(QPrinter* printer);
                                
      protected slots:

        virtual void on_pBtnOK_clicked();

        virtual void on_pBtnCancel_clicked();

        virtual void on_helpPushButton_clicked();

        virtual void on_lstPrinter_currentItemChanged(QListWidgetItem * current, QListWidgetItem * previous);

        virtual void on_rdoSuperDraft_clicked();

        virtual void on_rdoDraft_clicked();

        virtual void on_rdoNormal_clicked();

        virtual void on_rdoFineDetail_clicked();
        
      protected:

        QStringList m_dpiList;
        Scene*      m_scene;
        QPrinter*   m_printer;

      private:

        std::auto_ptr<Ui::PrintSettings> m_ui;
    };
  }    
}     

#endif

