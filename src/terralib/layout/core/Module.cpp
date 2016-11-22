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
  \file Module.cpp
   
  \brief This singleton defines the TerraLib Layout module entry.
*/


#include "Module.h"


#include "../qt/core/InitFactories.h"
#include "property/DataTypesUtils.h"
#include "../core/property/Property.h"

// TerraLib
#include <terralib/common/Logger.h>
#include <terralib/common/TerraLib.h>
#include <terralib/common/Translator.h>

#include <QMetaType>

const te::layout::Module& sm_module = te::layout::Module::getInstance();

te::layout::Module::Module()
{
  TerraLib::Module m = { TE_LAYOUT_MODULE_NAME,
                         te::layout::Module::initialize,
                         te::layout::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib Intersection Operation support
  TE_ADD_TEXT_DOMAIN(TE_LAYOUT_TEXT_DOMAIN, TE_LAYOUT_TEXT_DOMAIN_DIR, "ISO-8859-1");

  te::layout::initToolFactories(); // init all tool factories
  te::layout::initItemFactories(); // init all item factories
  te::layout::initTemplateFactories(); // init all template factories
  te::layout::initEditorFactories(); // init all editors factories
  te::layout::initTreeItemFactories(); // init all tree items factories
  te::layout::initRenderFactories(); // init all renders factories

  te::layout::RegisterLayoutTypes();

  // Register the type Property to use in QVariant. 
  qRegisterMetaType<te::layout::Property>("te::layout::Property");
}

te::layout::Module::~Module()
{
  TerraLib::getInstance().remove(TE_LAYOUT_MODULE_NAME);
}

void te::layout::Module::initialize()
{
  TE_LOG_TRACE(TE_TR("TerraLib layout module initialized!"));
}

void te::layout::Module::finalize()
{
  TE_LOG_TRACE(TE_TR("TerraLib layout module finalized!"));
}

