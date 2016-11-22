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
  \file DoubleSpinBoxEditor.h
   
   \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_DOUBLE_SPINBOX_EDITOR_H 
#define __TERRALIB_LAYOUT_INTERNAL_DOUBLE_SPINBOX_EDITOR_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../../core/property/Property.h"
#include "../../../../core/Config.h"
#include "AbstractEditor.h"
#endif

// STL
#include <vector>

// Qt
#include <QDoubleSpinBox>

namespace te
{
  namespace layout
  {
    class View;

    /*!
    \brief 
    
    \ingroup layout
    */
    class TELAYOUTEXPORT DoubleSpinBoxEditor : public QDoubleSpinBox, public AbstractEditor
    {
      Q_OBJECT //for slots/signals

      public:

        DoubleSpinBoxEditor(const QModelIndex& index, QWidget* parent = 0);

        virtual ~DoubleSpinBoxEditor();
        
        virtual QVariant getValue();
        
      signals:

        void dataValueChanged(QWidget* widget, const Property& prop);

      protected slots:

        virtual void onValueChanged(double d);

      protected:

        virtual void changeEditorData(const QModelIndex& index);
    };
  }
}

#endif


