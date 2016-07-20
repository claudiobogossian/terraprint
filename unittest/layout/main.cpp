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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TempDataStorageEditorTest
#include <boost/test/unit_test.hpp>

int add(int i, int j) 
{ 
  return i + j; 
}

BOOST_AUTO_TEST_CASE(tempDataStorageTest)
{
  // seven ways to detect and report the same error:
  BOOST_CHECK(add(2, 2) == 4);        // #1 continues on error

  BOOST_REQUIRE(add(2, 2) == 4);      // #2 throws on error

  if (add(2, 2) != 4)
    BOOST_ERROR("Ouch...");            // #3 continues on error

  if (add(2, 2) != 4)
    BOOST_FAIL("Ouch...");             // #4 throws on error

  if (add(2, 2) != 4) throw "Ouch..."; // #5 throws on error

  BOOST_CHECK_MESSAGE(add(2, 2) == 4,  // #6 continues on error
    "add(..) result: " << add(2, 2));

  BOOST_CHECK_EQUAL(add(2, 2), 4);   // #7 continues on error
}

