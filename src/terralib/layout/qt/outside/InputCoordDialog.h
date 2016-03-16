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


#ifndef __TERRALIB_LAYOUT_INTERNAL_INPUT_COORD_DIALOG_H
#define __TERRALIB_LAYOUT_INTERNAL_INPUT_COORD_DIALOG_H

#include "../../core/Config.h"

#include <memory>
#include <string>

//Qt
#include <QDialog>

namespace Ui { class InputCoordWidgetForm; }

namespace te
{
  namespace layout
  {

    /*!
    \file InputCoordDialog.h

    \brief Input coords in Decimal Degrees/Degree, Minutes and Seconds

    \ingroup layout
    */
    class TELAYOUTEXPORT InputCoordDialog : public QDialog
    {
      Q_OBJECT

      public:
        
        /*!
        \brief Constructor
        */
        InputCoordDialog(std::string& coord, int bottom, int top, QWidget* parent);
        
        /*!
        \brief destructor
        */
        ~InputCoordDialog();

        /*!
        \brief returns input value in DMS
        */
        virtual std::string  getCoordvalueDMS();

        /*!
        \brief returns input value in DD
        */
        virtual std::string  getCoordvalueDD();

      private slots:

        virtual void on_lneDegree_editingFinished();

        virtual void on_lneMinute_editingFinished();

        virtual void on_lneSecond_editingFinished();

        virtual void on_lneDecimalDegree_editingFinished();

        virtual void on_pbOK_clicked();

        virtual void on_rdoDD_clicked();

        virtual void on_rdoDMS_clicked();

        virtual void on_pbCancel_clicked();

    protected:     

      virtual void init();

      std::auto_ptr<Ui::InputCoordWidgetForm> m_ui;
      
      int         m_degree;
      int         m_min;
      double      m_sec;
      double      m_decimalD;
      std::string m_outPutValueDMS;
      std::string m_outPutValueDD;
      std::string m_inputValue;
      int         m_degreeBottom;
      int         m_degreeTop;
    };
  }
}

#endif