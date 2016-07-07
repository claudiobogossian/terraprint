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
  \file terralib/layout/qt/outside/MapLocationSettingsOutside.h

  \brief
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LOCATION_MAP_SETTINGS_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_LOCATION_MAP_SETTINGS_OUTSIDE_H

// TerraLib
#include "../../core/Config.h"
#ifndef Q_MOC_RUN
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#endif

// Qt
#include <QDialog>
#include <QString>
#include <QComboBox>

namespace Ui { class LocationMapSettings;
               class MapLayerChoice;
}


namespace te
{
  namespace layout
  {
    class AbstractOutsideController;
    class MapLayerChoiceOutside;

    class TELAYOUTEXPORT MapLocationSettingsOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT

      public:

        MapLocationSettingsOutside(AbstractOutsideController* controller, QWidget* parent);

        virtual ~MapLocationSettingsOutside();

        /*!
          \brief Load layers in double widget.
        */
        virtual void init();

        virtual void load();
        
        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();


      signals:

        void updateProperty(Property prop);

        void updateProperties(std::vector<Property> props);
                        
      protected slots:

        void onCancelPushButtonPressed();

        void updateWidgetProperty(Property prop);

        void on_cmbConnected_currentIndexChanged(const QString & text);
        
        void on_colorButton_pressed();

        void on_cColorButton_pressed();

      protected :

       // virtual void initCombo(QWidget* widget, std::string nameComponent);

        virtual void addComboOptions(QComboBox* combo, std::vector<Variant> options);
        
        virtual te::color::RGBAColor configColor(QWidget* widget);
        

        
      private:

        std::auto_ptr<Ui::LocationMapSettings> m_ui;
        std::auto_ptr<Ui::MapLayerChoice> ui_MapLayerChoice;
        std::auto_ptr<MapLayerChoiceOutside> m_widget;
        void initDouble(QWidget* widget, std::string nameComponent);
        void initCombo(QWidget* widget, std::string nameComponent);
        void initBool(QWidget* widget, std::string nameComponent);
        double mm2cm(double mmSize);
        double cm2mm(double cmSize);
        void loadColor(QWidget* widget, std::string nameComponent);
        std::string formatScaleValue(std::string inputValue);
        MapLayerChoiceOutside *m_mapChoice;
        
    };
  }    
}     

#endif

