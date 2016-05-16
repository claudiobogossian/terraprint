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
  \file terralib/layout/qt/outside/SVGDialogOutside.h

  \brief
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_NORTH_SETTINGS_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_NORTH_SETTINGS_OUTSIDE_H

// TerraLib
#include "../../core/Config.h"
#ifndef Q_MOC_RUN
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../core/property/NorthSettingsConfigProperties.h"
#include "../../core/Config.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/color/RGBAColor.h"
#include "../../core/property/Property.h"
#endif

// Qt
#include <QDialog>
#include <QString>
#include <QWidget>
#include <QVariant>
#include <QComboBox>

namespace Ui { class NorthSettings; }

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;
    class NorthSettingsConfigProperties;

    class TELAYOUTEXPORT NorthSettingsOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT

      public:

        NorthSettingsOutside(AbstractOutsideController* controller, QWidget* parent);

        virtual ~NorthSettingsOutside();

        virtual void load();

        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();
                        
      protected slots:

        void on_nBtnOK_clicked();

        void on_nBtnCancel_clicked();

        virtual void on_btnColor_clicked();

        virtual void on_rdbMillimeters_clicked();

        virtual void on_rdbCentimeters_clicked();

        virtual void on_lineEditNorthWidth_editingFinished();

        virtual void on_lineEditNorthHeight_editingFinished();

        virtual void on_cbNorth_currentIndexChanged(const QString & text);
  
      signals:

        void updateProperty(Property prop);

      protected:

        virtual te::color::RGBAColor configColor(QWidget* widget);

        virtual void init();

        virtual void initDouble(QWidget* widget, std::string nameComponent);

        virtual void initBool(QWidget* widget, std::string nameComponent);

        virtual void initColor(QWidget* widget, std::string nameComponent);

        virtual void initCombo(QWidget* widget, std::string nameComponent);

        virtual void addComboOptions(QComboBox* combo, std::vector<Variant> options);

      protected:

        NorthSettingsConfigProperties* m_northSettings;

      private:

        std::auto_ptr<Ui::NorthSettings> m_ui;
        
    };
  }    
}     

#endif

