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

#ifndef __TERRALIB_LAYOUT_INTERNAL_QTEST_PROPERTY_EDITOR_INTEGRATION_TEST_H
#define __TERRALIB_LAYOUT_INTERNAL_QTEST_PROPERTY_EDITOR_INTEGRATION_TEST_H

// Layout Module
#include "terralib/layout/qt/core/View.h"
#include "terralib/layout/qt/default/OutsideArea.h"

// STL
#include <memory>

// Qt
#include <QObject>

namespace te
{
  namespace layout
  {
    /*!
      \brief 
    
      \ingroup layout
    */
    class PropertyEditorIntegrationTest : public QObject
    {
      Q_OBJECT

      private slots:

        void on_test_create_view();

        void on_test_create_scene();

        void on_test_create_outside_area();

        void on_test_create_rectangle_item();

        void on_test_click_rectangle_item();

        void on_test_create_property_tree();

      private:

        std::unique_ptr<te::layout::View>         m_view;
        std::unique_ptr<te::layout::OutsideArea>  m_outsideArea;
    };
  }
}

#endif

