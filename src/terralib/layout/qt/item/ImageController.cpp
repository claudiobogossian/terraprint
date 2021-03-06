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

// TerraLib
#include "ImageController.h"
#include "ImageItem.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"


te::layout::ImageController::ImageController( te::layout::AbstractItemModel* model, AbstractItemView* view)
  : AbstractItemController(model, view)
{

}

te::layout::ImageController::~ImageController()
{

}

void te::layout::ImageController::setProperties(const Properties& properties)
{
  ImageItem* imageItem = dynamic_cast<ImageItem*>(this->getView());
  if (imageItem == 0)
  {
    AbstractItemController::setProperties(properties);
    return;
  }

  if (properties.contains("file_name") == false)
  {
    AbstractItemController::setProperties(properties);
    return;
  }

  const Property& pNewFileName = properties.getProperty("file_name");
  const std::string& newFileName = te::layout::Property::GetValueAs<std::string>(pNewFileName);
  const std::string& currentFileName = imageItem->getFileName();

  if (newFileName != currentFileName)
  {
    imageItem->setFileName(newFileName);
  }

  if (properties.getProperties().size() == 1)
  {
    imageItem->adjustSize();
  }

  AbstractItemController::setProperties(properties);
}

