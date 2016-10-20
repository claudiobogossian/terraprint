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
    \file Observer.h

    \brief Abstract class to represent an observer. "View" part of MVC component.

    \ingroup layout
*/

// TerraLib
#include "Observer.h"

#include "Subject.h"

te::layout::Observer::Observer()
{

}

te::layout::Observer::~Observer()
{
  std::vector<Subject*>::iterator it = m_vecSubjects.begin();
  while (it != m_vecSubjects.end())
  {
    (*it)->detach(this);
    ++it;
  }

  m_vecSubjects.clear();
}

void te::layout::Observer::addSubject(Subject* subject)
{
  m_vecSubjects.push_back(subject);
}

void te::layout::Observer::removeSubject(Subject* subject)
{
  std::vector<Subject*>::iterator it = m_vecSubjects.begin();
  while (it != m_vecSubjects.end())
  {
    if ((*it) == subject)
    {
      m_vecSubjects.erase(it);
      break;
    }
    
    ++it;
  }
}

