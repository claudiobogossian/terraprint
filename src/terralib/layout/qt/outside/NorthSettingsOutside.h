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
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../core/property/NorthSettingsConfigProperties.h"

// Qt
#include <QDialog>
#include <QString>

namespace Ui { class NorthSettings; }

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;

    class TELAYOUTEXPORT NorthSettingsOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT

      public:

        NorthSettingsOutside(AbstractOutsideController* controller);

        virtual ~NorthSettingsOutside();

        virtual void load();

        /*!
          \brief Load layers in double widget.
        */

        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();
                        
      protected slots:

        void onOkPushButtonPressed();

        void onCancelPushButtonPressed();

        void onSaveAsClicked();

        virtual void on_pbNorthLineColor_clicked();
  
      signals:

        void updateProperty(Property prop);

      protected:

        virtual te::color::RGBAColor configColor(QWidget* widget);

        virtual void init();

        NorthSettingsConfigProperties* m_northSettings;
        EnumType* m_northType;
        EnumType* m_geodesicType;

        //virtual void initString(QWidget* widget, std::string nameComponent, EnumType* northType);

        //virtual void initInt(QWidget* widget, std::string nameComponent, EnumType* northType);

        //virtual void initDouble(QWidget* widget, std::string nameComponent, EnumType* northType);

        //virtual void initBool(QWidget* widget, std::string nameComponent, EnumType* northType);

        //virtual void initColor(QWidget* widget, std::string nameComponent, EnumType* northType);

        //virtual void initCombo(QWidget* widget, std::string nameComponent, EnumType* northType);*/

        
      private:

        std::auto_ptr<Ui::NorthSettings> m_ui;
        
    };
  }    
}     

#endif

