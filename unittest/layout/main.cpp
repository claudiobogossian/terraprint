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

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_MODULE TempDataStorageEditorTest

// STL
#include <iostream> // std::cout
#include <fstream> // std::ofstream

// Qt
#include <QApplication>

// Boost 
#include <boost/test/unit_test.hpp>
#include <boost/test/debug.hpp>

struct UnitTestConfig
{
  UnitTestConfig() : test_log("layout_unittest_boost_test.log")  
  { 
    /* The only platform which supports memory leak detection is 
    Microsoft Visual Studio family of compilers in debug builds.  */
    boost::debug::detect_memory_leaks(false);

    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_messages); // output the contents of the macro BOOST_TEST_MESSAGE
    //boost::unit_test::unit_test_log.set_stream(test_log); 
  }
  ~UnitTestConfig()
  { 
    boost::unit_test::unit_test_log.set_stream(std::cout); 
  }
  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE(UnitTestConfig);

int main(int argv, char **args)
{
  QApplication app(argv, args); // required to run Boost Unit Test with Qt

  int result = boost::unit_test::unit_test_main(&init_unit_test, argv, args);
 
  return result;
}

