#
#  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of the TerraLib - a Framework for building GIS enabled applications.
#
#  TerraLib is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  TerraLib is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with TerraLib. See COPYING. If not, write to
#  TerraLib Team at <terralib-team@terralib.org>.
#
#
#  Description: Build the tests with QTest for the Layout Module.
#
#  Author: Juliana Hohara de Souza Coelho <juliana.coelho@funcate.org.br>

include_directories(${Boost_INCLUDE_DIR})
include_directories(${QTPROPERTYBROWSER_INCLUDE_DIR})
include_directories(${TERRALIB_ABSOLUTE_ROOT_DIR}/src)

if(LOG4CXX_FOUND)
  include_directories(${LOG4CXX_INCLUDE_DIR})
endif()

if(WIN32)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -DTELAYOUTUNITTESTDLL -DQT_QTPROPERTYBROWSER_IMPORT)
  set(QT_USE_QTMAIN TRUE)
endif()

file(GLOB TERRALIB_MOD_LAYOUT_QTEST_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/qtest/*.cpp)
file(GLOB TERRALIB_MOD_LAYOUT_QTEST_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/qtest/*.h)

set(TERRALIB_MOD_LAYOUT_UNITTEST_FILES 	${TERRALIB_MOD_LAYOUT_QTEST_SRC_FILES}
										${TERRALIB_MOD_LAYOUT_QTEST_HDR_FILES})

if(Qt5_FOUND)

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  
  add_executable(terralib_mod_layout_qtest ${TERRALIB_MOD_LAYOUT_UNITTEST_FILES})
  
  qt5_use_modules(terralib_mod_layout_qtest Widgets Test)
  
else() # QT4_FOUND

  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  include(${QT_USE_FILE})
  include_directories(${QT_INCLUDE_DIR})
  add_definitions(${QT_DEFINITIONS})
  
  # moc'ing
  set(TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/qtest/ViewTest.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/qtest/PropertyEditorIntegrationTest.h
								)

  QT4_WRAP_CPP(TERRALIB_GEN_SRC_FILES ${TERRALIB_HDRS_TO_MOC_FILES} OPTIONS -DBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
  
  list(APPEND TERRALIB_MOD_LAYOUT_UNITTEST_FILES ${TERRALIB_GEN_SRC_FILES})
  
  add_executable(terralib_mod_layout_qtest ${TERRALIB_MOD_LAYOUT_UNITTEST_FILES})
  
  target_link_libraries(terralib_mod_layout_qtest ${QT_LIBRARIES})

endif()

target_link_libraries(terralib_mod_layout_qtest terralib_mod_layout
											    terralib_mod_color
											    terralib_mod_common
											    terralib_mod_dataaccess
											    terralib_mod_datatype
											    terralib_mod_geometry
											    terralib_mod_maptools
											    terralib_mod_memory
											    terralib_mod_qt_widgets
											    terralib_mod_srs
											    terralib_mod_symbology
											    ${QTPROPERTYBROWSER_LIBRARY}
											    ${Boost_LIBRARIES}
											    )

# Add the tests.  These invoke "cmake --build ..." which is a
# cross-platform way of building the given target.
add_test(NAME terralib_mod_layout_qtest
         COMMAND ${CMAKE_COMMAND} --build . --target terralib_mod_layout_qtest --config $<CONFIGURATION>
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR})	
												
install(FILES ${TERRALIB_MOD_LAYOUT_QTEST_SRC_FILES} ${TERRALIB_MOD_LAYOUT_QTEST_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_UNITTEST}/qtest COMPONENT devel)
