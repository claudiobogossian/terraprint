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

#include "LayoutDoubleValidator.h"

te::layout::LayoutDoubleValidator::LayoutDoubleValidator(double bottom, double top, int decimals, QObject * parent) : 
QDoubleValidator(bottom, top, decimals, parent)
{
}

te::layout::LayoutDoubleValidator::~LayoutDoubleValidator()
{

}

QValidator::State te::layout::LayoutDoubleValidator::validate(QString &s, int &i) const
{
  if (this->bottom() < 0)
  {
    if (s.isEmpty() || s == "-") 
    {
      return QValidator::Intermediate;
    }
  }
  else
  {
    if (s.isEmpty()) 
    {
      return QValidator::Intermediate;
    }

  }  

  QLocale locale;

  QChar decimalPoint = locale.decimalPoint();
  int charsAfterPoint = s.length() - s.indexOf(decimalPoint) - 1;

  if (charsAfterPoint > decimals() && s.indexOf(decimalPoint) != -1) 
  {
    return QValidator::Invalid;
  }

  bool ok;
  double d = locale.toDouble(s, &ok);

  if (ok && d >= bottom() && d <= top()) 
  {
    return QValidator::Acceptable;
  }
  else 
  {
    return QValidator::Invalid;
  }

}