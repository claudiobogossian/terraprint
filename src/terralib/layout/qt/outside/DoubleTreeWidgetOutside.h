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
  \file DoubleTreeWidgetOutside.h

  \brief ...
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_DOUBLE_TREE_WIDGET_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_DOUBLE_TREE_WIDGET_OUTSIDE_H

#include <memory>

// TerraLib
#include "../../core/Config.h"

// Qt
#include <QWidget>

class QListWidgetItem;

namespace Ui { class DoubleTreeWidget; }

namespace te
{
  namespace layout
  {
    /*!
      \class DoubleTreeWidgetOutside

      \brief ...
    */
    class TELAYOUTEXPORT DoubleTreeWidgetOutside : public QWidget
    {
      Q_OBJECT

      public:

        DoubleTreeWidgetOutside(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~DoubleTreeWidgetOutside();

        Ui::DoubleTreeWidget* getForm() const;

        void setLeftLabel(std::string value);

        void setRightLabel(std::string value);

        void setInputValues(std::vector<std::string> values);

        void setInputDataValues(std::vector<std::string> values, std::vector<int> ids);

        void setOutputValues(std::vector<std::string> values);

        void setOutputDataValues(std::vector<std::string> values, std::vector<int> ids);

        void setFixedOutputValues(std::vector<std::string> values, std::string iconName);

        std::vector<std::string> getOutputValues();

        std::vector<int> getOutputDataValues();

        void clearInputValues();

        void clearOutputValues();

        void setVisibledOrderButtons(bool state);

      protected slots:

        void onAddToolButtonPressed();

        void onAddAllToolButtonPressed();

        void onRemoveToolButtonPressed();

        void onRemoveAllToolButtonPressed();

        void onLeftListItemSelectionChanged();

        void onRightListItemSelectionChanged();

        void onUpToolButtonPressed();

        void onDownToolButtonPressed();

      signals:

        void itemChanged();

      private:

        std::auto_ptr<Ui::DoubleTreeWidget> m_ui;
    }; 
  }   
}     

#endif  // __TERRALIB_LAYOUT_INTERNAL_DOUBLE_TREE_WIDGET_OUTSIDE_H

