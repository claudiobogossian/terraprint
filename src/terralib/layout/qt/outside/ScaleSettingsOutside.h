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
  \file terralib/layout/qt/outside/ScaleSettingsOutside.h

  \brief
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SCALE_SETTINGS_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_SCALE_SETTINGS_OUTSIDE_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../core/property/Variant.h"
#include "../../core/Font.h"

// STL
#include <vector>

// Qt
#include <QDialog>
#include <QString>
#include <QColor>
#include <QFont>

class QComboBox;

namespace Ui { class ScaleSettings; }

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;

    class TELAYOUTEXPORT ScaleSettingsOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT

      public:

        ScaleSettingsOutside(AbstractOutsideController* controller);

        virtual ~ScaleSettingsOutside();

        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();

        virtual void load();
        
      signals:

        void updateProperty(Property prop);

        void updateProperties(std::vector<Property> props);
                        
      protected slots:
      
        void on_pBtnCancel_clicked();

        void on_pbtnFont_clicked();

        void on_cmbConnectedTo_currentIndexChanged(const QString & text);

        void on_cmbType_currentIndexChanged(const QString & text);

        void on_cmbUnit_currentIndexChanged(const QString & text);

        void on_chkOnlyFirstAndLastValue_clicked();

        void on_txtScaleGapXInUnit_editingFinished();

        void on_txtScaleGapX_editingFinished();

        void on_txtScaleGapY_editingFinished();

        void on_txtNumberOfBreaks_editingFinished();

        void on_chkByBreaks_clicked();

      protected:

        /*!
        \brief Load layers in double widget.
        */
        virtual void init();

        virtual void initString(QWidget* widget, std::string nameComponent);

        virtual void initInt(QWidget* widget, std::string nameComponent);

        virtual void initDouble(QWidget* widget, std::string nameComponent);

        virtual void initBool(QWidget* widget, std::string nameComponent);
        
        virtual void initCombo(QWidget* widget, std::string nameComponent);

        virtual void initTextEdit(QWidget* widget, std::string nameComponent);

        virtual void addComboOptions(QComboBox* combo, std::vector<Variant> options);

        virtual QString fontHTML(const Property& prop);

        virtual Font configFont(QWidget* widget);

        Font qFont2Font(QFont qFont);

        QFont font2QFont(Font font);

        virtual void createIntValidator(QWidget* widget);
        
      private:

        std::auto_ptr<Ui::ScaleSettings> m_ui;
        
    };
  }    
}     

#endif

