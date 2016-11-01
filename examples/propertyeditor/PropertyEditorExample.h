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

#ifndef __TERRALIB_LAYOUT_INTERNAL_EXAMPLES_PROPERTY_EDITOR_H
#define __TERRALIB_LAYOUT_INTERNAL_EXAMPLES_PROPERTY_EDITOR_H

// Qt
#include <QWidget>

namespace te
{
  namespace layout
  {
    class RectangleItem;
    class PropertyTree;

    /*!
      \brief 
    
      \ingroup layout
    */
    class PropertyEditorExample : public QWidget
    {
      Q_OBJECT

      public:

        /*!
        \brief Constructor.
        */
        PropertyEditorExample(QWidget *parent = 0);

        /*!
        \brief Destructor.
        */
        virtual ~PropertyEditorExample();

        void run();

      protected:

        void createRectangleItem();
        
        void createPropertyTree();

      private:

        te::layout::RectangleItem* m_rectItem;
        te::layout::PropertyTree*  m_tree;
    };
  }
}

#endif

