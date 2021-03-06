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
#  Description: Build configuration for the Print Layout Module.
#
#  Author: Juliana Hohara de Souza Coelho <juliana.coelho@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})
include_directories(${QTPROPERTYBROWSER_INCLUDE_DIR})
include_directories(${TERRALIB_ABSOLUTE_ROOT_DIR}/src)

if(WIN32)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -DTELAYOUTDLL -DQT_QTPROPERTYBROWSER_IMPORT)
endif()

if(Qt5_FOUND)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

else() # QT4_FOUND

  include_directories(${CMAKE_CURRENT_BINARY_DIR})

  include(${QT_USE_FILE})

  include_directories(${QT_INCLUDE_DIR})

  add_definitions(${QT_DEFINITIONS})

endif()

find_package(GeographicLib 1.45)
if(GEOGRAPHICLIB_FOUND)
  include_directories(${GeographicLib_INCLUDE_DIRS})

  add_definitions(-DGEOGRAPHICLIB_ENABLED)

  file(GLOB GEOGRAPHICLIB_MODEL_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/magnetic_model/*.*)
endif()

file(GLOB TERRALIB_LAYOUT_CORE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_ENUM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/enum/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_ENUM_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/enum/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_FACTORY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/factory/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_FACTORY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/factory/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_MVC_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/mvc/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_MVC_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/mvc/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_OBSERVER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/observer/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_OBSERVER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/observer/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_PROXY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/proxy/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_PROXY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/proxy/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_SINGLETON_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/singleton/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_SINGLETON_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/singleton/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_PROPERTY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/property/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_PROPERTY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/property/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_SERIALIZATION_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/serialization/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_SERIALIZATION_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/serialization/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_SERVICE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/service/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_SERVICE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/service/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_TEMPLATE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/template/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_TEMPLATE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/template/*.h)
file(GLOB TERRALIB_LAYOUT_ITEM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/item/*.cpp)
file(GLOB TERRALIB_LAYOUT_ITEM_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/item/*.h)
file(GLOB TERRALIB_LAYOUT_OUTSIDE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/outside/*.cpp)
file(GLOB TERRALIB_LAYOUT_OUTSIDE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/outside/*.h)
file(GLOB TERRALIB_LAYOUT_QT_CORE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_CORE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/*.h)
file(GLOB TERRALIB_LAYOUT_QT_DEFAULT_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_DEFAULT_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/*.h)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_ITEM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/item/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_ITEM_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/item/*.h)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_OUTSIDE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/outside/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_OUTSIDE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/outside/*.h)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_TOOL_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/tool/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_TOOL_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/tool/*.h)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_EDITOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/propertyeditor/editor/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_EDITOR_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/propertyeditor/editor/*.h)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_TREE_ITEM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/propertyeditor/treeItem/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_TREE_ITEM_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/propertyeditor/treeItem/*.h)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_RENDER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/propertyeditor/render/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_RENDER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/propertyeditor/render/*.h)
file(GLOB TERRALIB_LAYOUT_QT_COMMAND_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/command/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_COMMAND_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/command/*.h)
file(GLOB TERRALIB_LAYOUT_QT_TOOLS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/tools/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_TOOLS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/tools/*.h)
file(GLOB TERRALIB_LAYOUT_QT_PROPERTYBROWSER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_PROPERTYBROWSER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/*.h)
file(GLOB TERRALIB_LAYOUT_QT_PROPERTYEDITOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_PROPERTYEDITOR_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/*.h)
file(GLOB TERRALIB_LAYOUT_QT_PROPERTYTREE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/tree/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_PROPERTYTREE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/tree/*.h)
file(GLOB TERRALIB_LAYOUT_QT_PROPERTYRENDER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/render/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_PROPERTYRENDER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/render/*.h)
file(GLOB TERRALIB_LAYOUT_QT_ITEM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_ITEM_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/*.h)
file(GLOB TERRALIB_LAYOUT_QT_OUTSIDE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_OUTSIDE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/*.h)
file(GLOB TERRALIB_LAYOUT_QT_INSIDE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/inside/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_INSIDE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/inside/*.h)
file(GLOB TERRALIB_LAYOUT_QT_TEMP_DATASTORAGE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/tempDataStorage/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_TEMP_DATASTORAGE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/tempDataStorage/*.h)
file(GLOB TERRALIB_LAYOUT_UI_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/ui/*.ui)

# datasource
source_group("Source Files\\core" FILES ${TERRALIB_LAYOUT_CORE_SRC_FILES})
source_group("Header Files\\core" FILES ${TERRALIB_LAYOUT_CORE_HDR_FILES})
source_group("Source Files\\core\\enum" FILES ${TERRALIB_LAYOUT_CORE_ENUM_SRC_FILES})
source_group("Header Files\\core\\enum" FILES ${TERRALIB_LAYOUT_CORE_ENUM_HDR_FILES})
source_group("Source Files\\core\\pattern\\factory" FILES ${TERRALIB_LAYOUT_CORE_FACTORY_SRC_FILES})
source_group("Header Files\\core\\pattern\\factory" FILES ${TERRALIB_LAYOUT_CORE_FACTORY_HDR_FILES})
source_group("Source Files\\core\\pattern\\mvc" FILES ${TERRALIB_LAYOUT_CORE_MVC_SRC_FILES})
source_group("Header Files\\core\\pattern\\mvc" FILES ${TERRALIB_LAYOUT_CORE_MVC_HDR_FILES})
source_group("Source Files\\core\\pattern\\observer" FILES ${TERRALIB_LAYOUT_CORE_OBSERVER_SRC_FILES})
source_group("Header Files\\core\\pattern\\observer" FILES ${TERRALIB_LAYOUT_CORE_OBSERVER_HDR_FILES})
source_group("Source Files\\core\\pattern\\proxy" FILES ${TERRALIB_LAYOUT_CORE_PROXY_SRC_FILES})
source_group("Header Files\\core\\pattern\\proxy" FILES ${TERRALIB_LAYOUT_CORE_PROXY_HDR_FILES})
source_group("Source Files\\core\\pattern\\singleton" FILES ${TERRALIB_LAYOUT_CORE_SINGLETON_SRC_FILES})
source_group("Header Files\\core\\pattern\\singleton" FILES ${TERRALIB_LAYOUT_CORE_SINGLETON_HDR_FILES})
source_group("Source Files\\core\\property" FILES ${TERRALIB_LAYOUT_CORE_PROPERTY_SRC_FILES})
source_group("Header Files\\core\\property" FILES ${TERRALIB_LAYOUT_CORE_PROPERTY_HDR_FILES})
source_group("Source Files\\core\\serialization" FILES ${TERRALIB_LAYOUT_CORE_SERIALIZATION_SRC_FILES})
source_group("Header Files\\core\\serialization" FILES ${TERRALIB_LAYOUT_CORE_SERIALIZATION_HDR_FILES})
source_group("Source Files\\core\\template" FILES ${TERRALIB_LAYOUT_CORE_TEMPLATE_SRC_FILES})
source_group("Header Files\\core\\template" FILES ${TERRALIB_LAYOUT_CORE_TEMPLATE_HDR_FILES})
source_group("Source Files\\core\\service" FILES ${TERRALIB_LAYOUT_CORE_SERVICE_SRC_FILES})
source_group("Header Files\\core\\service" FILES ${TERRALIB_LAYOUT_CORE_SERVICE_HDR_FILES})
source_group("Source Files\\item"   FILES ${TERRALIB_LAYOUT_ITEM_SRC_FILES})
source_group("Header Files\\item"   FILES ${TERRALIB_LAYOUT_ITEM_HDR_FILES})
source_group("Source Files\\outside"  FILES ${TERRALIB_LAYOUT_OUTSIDE_SRC_FILES})
source_group("Header Files\\outside"  FILES ${TERRALIB_LAYOUT_OUTSIDE_HDR_FILES})
source_group("Source Files\\qt\\core"  FILES ${TERRALIB_LAYOUT_QT_CORE_SRC_FILES})
source_group("Header Files\\qt\\core"  FILES ${TERRALIB_LAYOUT_QT_CORE_HDR_FILES})
source_group("Source Files\\qt\\default"  FILES ${TERRALIB_LAYOUT_QT_DEFAULT_SRC_FILES})
source_group("Header Files\\qt\\default"  FILES ${TERRALIB_LAYOUT_QT_DEFAULT_HDR_FILES})
source_group("Source Files\\qt\\core\\pattern\\factory\\item"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_ITEM_SRC_FILES})
source_group("Header Files\\qt\\core\\pattern\\factory\\item"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_ITEM_HDR_FILES})
source_group("Source Files\\qt\\core\\pattern\\factory\\outside"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_OUTSIDE_SRC_FILES})
source_group("Header Files\\qt\\core\\pattern\\factory\\outside"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_OUTSIDE_HDR_FILES})
source_group("Source Files\\qt\\core\\pattern\\factory\\tool"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_TOOL_SRC_FILES})
source_group("Header Files\\qt\\core\\pattern\\factory\\tool"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_TOOL_HDR_FILES})
source_group("Source Files\\qt\\core\\pattern\\factory\\propertyeditor\\editor"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_EDITOR_SRC_FILES})
source_group("Header Files\\qt\\core\\pattern\\factory\\propertyeditor\\editor"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_EDITOR_HDR_FILES})
source_group("Source Files\\qt\\core\\pattern\\factory\\propertyeditor\\treeItem"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_TREE_ITEM_SRC_FILES})
source_group("Header Files\\qt\\core\\pattern\\factory\\propertyeditor\\treeItem"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_TREE_ITEM_HDR_FILES})
source_group("Source Files\\qt\\core\\pattern\\factory\\propertyeditor\\render"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_RENDER_SRC_FILES})
source_group("Header Files\\qt\\core\\pattern\\factory\\propertyeditor\\render"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_RENDER_HDR_FILES})
source_group("Source Files\\qt\\core\\pattern\\command"  FILES ${TERRALIB_LAYOUT_QT_COMMAND_SRC_FILES})
source_group("Header Files\\qt\\core\\pattern\\command"  FILES ${TERRALIB_LAYOUT_QT_COMMAND_HDR_FILES})
source_group("Source Files\\qt\\core\\tools"  FILES ${TERRALIB_LAYOUT_QT_TOOLS_SRC_FILES})
source_group("Header Files\\qt\\core\\tools"  FILES ${TERRALIB_LAYOUT_QT_TOOLS_HDR_FILES})
source_group("Source Files\\qt\\core\\propertybrowser"  FILES ${TERRALIB_LAYOUT_QT_PROPERTYBROWSER_SRC_FILES})
source_group("Header Files\\qt\\core\\propertybrowser"  FILES ${TERRALIB_LAYOUT_QT_PROPERTYBROWSER_HDR_FILES})
source_group("Source Files\\qt\\core\\propertyeditor\\editor"  FILES ${TERRALIB_LAYOUT_QT_PROPERTYEDITOR_SRC_FILES})
source_group("Header Files\\qt\\core\\propertyeditor\\editor"  FILES ${TERRALIB_LAYOUT_QT_PROPERTYEDITOR_HDR_FILES})
source_group("Source Files\\qt\\core\\propertyeditor\\tree"  FILES ${TERRALIB_LAYOUT_QT_PROPERTYTREE_SRC_FILES})
source_group("Header Files\\qt\\core\\propertyeditor\\tree"  FILES ${TERRALIB_LAYOUT_QT_PROPERTYTREE_HDR_FILES})
source_group("Source Files\\qt\\core\\propertyeditor\\render"  FILES ${TERRALIB_LAYOUT_QT_PROPERTYRENDER_SRC_FILES})
source_group("Header Files\\qt\\core\\propertyeditor\\render"  FILES ${TERRALIB_LAYOUT_QT_PROPERTYRENDER_HDR_FILES})
source_group("Source Files\\qt\\core\\tempDataStorage" FILES ${TERRALIB_LAYOUT_QT_TEMP_DATASTORAGE_SRC_FILES})
source_group("Header Files\\qt\\core\\tempDataStorage" FILES ${TERRALIB_LAYOUT_QT_TEMP_DATASTORAGE_HDR_FILES})
source_group("Source Files\\qt\\item"  FILES ${TERRALIB_LAYOUT_QT_ITEM_SRC_FILES})
source_group("Header Files\\qt\\item"  FILES ${TERRALIB_LAYOUT_QT_ITEM_HDR_FILES})
source_group("Source Files\\qt\\outside"  FILES ${TERRALIB_LAYOUT_QT_OUTSIDE_SRC_FILES})
source_group("Header Files\\qt\\outside"  FILES ${TERRALIB_LAYOUT_QT_OUTSIDE_HDR_FILES})
source_group("Source Files\\qt\\inside"  FILES ${TERRALIB_LAYOUT_QT_INSIDE_SRC_FILES})
source_group("Header Files\\qt\\inside"  FILES ${TERRALIB_LAYOUT_QT_INSIDE_HDR_FILES})
source_group("UI Files\\"  FILES ${TERRALIB_LAYOUT_UI_HDR_FILES})

set(TERRALIB_FILES ${TERRALIB_LAYOUT_CORE_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_HDR_FILES}
           ${TERRALIB_LAYOUT_CORE_ENUM_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_ENUM_HDR_FILES}
           ${TERRALIB_LAYOUT_CORE_FACTORY_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_FACTORY_HDR_FILES}
           ${TERRALIB_LAYOUT_CORE_MVC_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_MVC_HDR_FILES}
           ${TERRALIB_LAYOUT_CORE_OBSERVER_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_OBSERVER_HDR_FILES}
           ${TERRALIB_LAYOUT_CORE_PROXY_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_PROXY_HDR_FILES}
           ${TERRALIB_LAYOUT_CORE_SINGLETON_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_SINGLETON_HDR_FILES}
           ${TERRALIB_LAYOUT_CORE_PROPERTY_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_PROPERTY_HDR_FILES}
           ${TERRALIB_LAYOUT_CORE_SERIALIZATION_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_SERIALIZATION_HDR_FILES}
           ${TERRALIB_LAYOUT_CORE_TEMPLATE_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_TEMPLATE_HDR_FILES}
           ${TERRALIB_LAYOUT_CORE_SERVICE_SRC_FILES}
           ${TERRALIB_LAYOUT_CORE_SERVICE_HDR_FILES}
           ${TERRALIB_LAYOUT_ITEM_SRC_FILES}
           ${TERRALIB_LAYOUT_ITEM_HDR_FILES}
           ${TERRALIB_LAYOUT_OUTSIDE_SRC_FILES}
           ${TERRALIB_LAYOUT_OUTSIDE_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_CORE_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_CORE_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_DEFAULT_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_DEFAULT_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_COMMAND_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_COMMAND_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_TOOLS_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_TOOLS_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_ITEM_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_ITEM_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_OUTSIDE_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_OUTSIDE_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_PROPERTYBROWSER_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_PROPERTYBROWSER_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_PROPERTYEDITOR_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_PROPERTYEDITOR_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_PROPERTYTREE_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_PROPERTYTREE_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_PROPERTYRENDER_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_PROPERTYRENDER_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_TEMP_DATASTORAGE_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_TEMP_DATASTORAGE_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_ITEM_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_ITEM_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_OUTSIDE_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_OUTSIDE_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_TOOL_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_TOOL_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_EDITOR_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_EDITOR_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_TREE_ITEM_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_TREE_ITEM_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_RENDER_HDR_FILES}
           ${TERRALIB_LAYOUT_QT_FACTORY_RENDER_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_INSIDE_SRC_FILES}
           ${TERRALIB_LAYOUT_QT_INSIDE_HDR_FILES})

if(Qt5_FOUND)

# uic'ing
  QT5_WRAP_UI(TERRALIB_LAYOUT_GEN_HDR_FILES ${TERRALIB_LAYOUT_UI_HDR_FILES})

  list(APPEND TERRALIB_FILES ${TERRALIB_LAYOUT_GEN_HDR_FILES})

  add_library(terralib_mod_layout SHARED ${TERRALIB_FILES})

  qt5_use_modules(terralib_mod_layout Widgets PrintSupport Svg)

else()

# uic'ing
  QT4_WRAP_UI(TERRALIB_LAYOUT_GEN_HDR_FILES ${TERRALIB_LAYOUT_UI_HDR_FILES})

# moc'ing
  set(TERRALIB_HDRS_TO_MOC_FILES
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/Scene.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/PropertiesOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/ObjectInspectorOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/ToolbarOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/ToolbarController.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/View.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/MapItem.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/PropertyBrowser.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/VisualizationArea.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/GridSettingsOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/PageSetupOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/SystematicScaleOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/EditTemplateOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/MenuBuilder.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/TextItem.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/TextGridSettingsOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/PrintScene.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/MapLayerChoiceOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/MapLocationSettingsOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/SVGDialogOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/DialogPropertiesBrowser.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/VariantPropertiesBrowser.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/ColorDialogOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/LegendChoiceOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/FontDialogOutside.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/ColorEditWidget.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/ColorEditorFactory.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/ItemObserverFactory.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/ItemObserverProperty.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertybrowser/ItemObserverManager.h
                                 #Base Layout
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/MainLayout.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/OutsideArea.h

                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/ColorDialogOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/LegendChoiceOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/FontDialogOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/ExportSettingsOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/DoubleTreeWidgetOutside.h
                                 #Base Layout
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/MainLayout.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/OutsideArea.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/DisplayDock.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/ObjectInspectorDock.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/PropertiesDock.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/EditTemplateDock.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/inside/MapToolbarInside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/inside/MapToolbarController.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/inside/DialogItemToolbar.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/MapSettingsOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/ScaleSettingsOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/NorthSettingsOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/PrintPreviewDialog.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/InputCoordDialog.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/PrintSettingsOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/MenuPrincipal.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/default/PropertiesCentralController.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/BuildGraphicsItem.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/tempDataStorage/TempDataStorageEditor.h
                                 #Property Editor
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/tree/PropertyTree.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/tree/PropertyDelegate.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/DoubleSpinBoxEditor.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/IntSpinBoxEditor.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/BoolCheckBoxEditor.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/StringLineEditor.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/StringListComboBoxEditor.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/FontEditor.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/ColorPickerEditor.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/MapSettingsEditor.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/propertyeditor/editor/GridSettingsEditor.h
                                 )

  QT4_WRAP_CPP(TERRALIB_GEN_SRC_FILES ${TERRALIB_HDRS_TO_MOC_FILES} OPTIONS -DBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

  list(APPEND TERRALIB_FILES ${TERRALIB_LAYOUT_GEN_HDR_FILES} ${TERRALIB_GEN_SRC_FILES})

  add_library(terralib_mod_layout SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_layout ${QT_LIBRARIES})

endif()

target_link_libraries(terralib_mod_layout  terralib_mod_color
                                           terralib_mod_common
										   terralib_mod_core
                                           terralib_mod_dataaccess
                                           terralib_mod_datatype
                                           terralib_mod_geometry
                                           terralib_mod_maptools
                                           terralib_mod_memory
                                           terralib_mod_qt_widgets
                                           terralib_mod_srs
                                           terralib_mod_symbology
                                           ${QTPROPERTYBROWSER_LIBRARY}
                                           ${Boost_LIBRARIES})

set_target_properties(terralib_mod_layout
                      PROPERTIES VERSION ${TERRALIB_LAYOUT_VERSION_MAJOR}.${TERRALIB_LAYOUT_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_LAYOUT_VERSION_MAJOR}.${TERRALIB_LAYOUT_VERSION_MINOR}
                                 INSTALL_NAME_DIR "@executable_path/../lib")

# translations

set(mo_input_dir "${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/messages/pt_BR/LC_MESSAGES")
set(mo_output_dir "${CMAKE_BINARY_DIR}/share/terralib/translations/messages/pt_BR/LC_MESSAGES")

file( MAKE_DIRECTORY ${mo_output_dir})
file(COPY ${mo_input_dir}/telayout.mo DESTINATION ${mo_output_dir})

file(GLOB _qm_files)
if (QT4_FOUND)
  GenerateTranslationQt4("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout" "terralib_mod_layout" _qm_files)
else()
  if (Qt5_FOUND)
    GenerateTranslationQt5("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout" "terralib_mod_layout" _qm_files)
  endif()
endif()
file(GLOB translated_files "${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/*.qm")
list(APPEND _qm_files ${translated_files})

#Creating build directory translation and copying translated files to build translation folder of terralib5_layout
  add_custom_command(
    TARGET terralib_mod_layout
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/share/terralib/translations
  )
  
foreach(qm_file ${_qm_files})
  add_custom_command(
    TARGET terralib_mod_layout
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${qm_file} ${CMAKE_BINARY_DIR}/share/terralib/translations
  )
endforeach()
  

#if (${TERRALIB_MOD_QT_PLUGINS_LAYOUT_ENABLED})
  #Copying translated files to build translation folder of terralib to be used on execution of application on IDE environment
#  add_custom_command(
#    TARGET terralib_mod_layout
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy ${_qm_files} ${terralib_DIR}/share/terralib/translations
#  )
#endif()

install(
  FILES ${mo_input_dir}/telayout.mo
  DESTINATION ${TERRALIB_DESTINATION_SHARE}/terralib/translations/messages/pt_BR/LC_MESSAGES
  COMPONENT runtime
)

install(
  FILES ${_qm_files}
  DESTINATION ${TERRALIB_DESTINATION_SHARE}/terralib/translations
  COMPONENT runtime
)

if(GEOGRAPHICLIB_FOUND)
  #for build version
  set(geographiclib_output_dir "${CMAKE_BINARY_DIR}/share/terralib/magnetic_model")

  file( MAKE_DIRECTORY ${geographiclib_output_dir})
  file(COPY ${GEOGRAPHICLIB_MODEL_FILES} DESTINATION ${geographiclib_output_dir})

  target_link_libraries(terralib_mod_layout ${GeographicLib_LIBRARY})

  #for installed version
  install(
    FILES ${GEOGRAPHICLIB_MODEL_FILES}
    DESTINATION ${TERRALIB_DESTINATION_SHARE}/terralib/magnetic_model
    COMPONENT runtime
  )
endif()

# -------------

install(TARGETS terralib_mod_layout
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_LAYOUT_CORE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_ENUM_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/enum COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_FACTORY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/pattern/factory COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_MVC_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/pattern/mvc COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_OBSERVER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/pattern/observer COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_PROXY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/pattern/proxy COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_SINGLETON_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/pattern/singleton COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_PROPERTY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/property COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_SERIALIZATION_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/serialization COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_TEMPLATE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/template COMPONENT devel)
        
install(FILES ${TERRALIB_LAYOUT_CORE_SERVICE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/service COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_ITEM_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/item COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_OUTSIDE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/outside COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_CORE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_DEFAULT_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/default COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_DEFAULT_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/inside COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_FACTORY_ITEM_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/pattern/factory/item COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_FACTORY_OUTSIDE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/pattern/factory/outside COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_FACTORY_TOOL_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/pattern/factory/tool COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_FACTORY_EDITOR_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/pattern/factory/propertyeditor/editor COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_FACTORY_TREE_ITEM_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/pattern/factory/propertyeditor/treeItem COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_FACTORY_RENDER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/pattern/factory/propertyeditor/render COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_COMMAND_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/pattern/command COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_TOOLS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/tools COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_PROPERTYBROWSER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/propertybrowser COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_PROPERTYEDITOR_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/propertyeditor/editor COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_PROPERTYTREE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/propertyeditor/tree COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_PROPERTYRENDER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/propertyeditor/render COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_ITEM_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/item COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_OUTSIDE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/outside COMPONENT devel)

if(UNIX)
  install(FILES ${CMAKE_BINARY_DIR}/bin/libterralib_mod_layout.so.${TERRALIB_LAYOUT_VERSION_MAJOR}.${TERRALIB_LAYOUT_VERSION_MINOR}
          DESTINATION ${terralib_DIR}/../../)
  install(FILES ${CMAKE_BINARY_DIR}/bin/libterralib_mod_layout.so
          DESTINATION ${terralib_DIR}/../../)
endif()

export(TARGETS terralib_mod_layout APPEND FILE ${CMAKE_BINARY_DIR}/terralib_layout-exports.cmake)

if(MSVC)
    add_custom_command(
        TARGET terralib_mod_layout
        POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/Debug ${terralib_DIR}/debug

        COMMENT "Copying plugin files..."
        VERBATIM
      )
  add_custom_command(
    TARGET terralib_mod_layout
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E  copy_directory ${CMAKE_BINARY_DIR}/Release ${terralib_DIR}/release

    COMMENT "Copying plugin files..."
    VERBATIM
    )
endif()
