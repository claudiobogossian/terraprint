#include "DataTypesUtils.h"

#include "DataConverters.h"
#include "DataTypes.h"

//DataTypes
#include <string>
#include "../Font.h"

//Terralib
#include <terralib/common/Exception.h>
#include <terralib/color/RGBAColor.h>
#include <terralib/datatype/Enums.h>
#include <terralib/geometry/Envelope.h>
#include <terralib/geometry/Geometry.h>
#include <terralib/maptools/AbstractLayer.h>

//STL
#include <list>


void te::layout::registerLayoutTypes()
{
  TypeManager::getInstance().registerType<bool>((int)te::layout::LayoutTypes::BOOLEAN_TYPE, "bool", ConvertStringToBool, ConvertBoolToString);
  TypeManager::getInstance().registerType<te::color::RGBAColor>((int)te::layout::LayoutTypes::COLOR_TYPE, "color", ConvertStringToColor, ConvertColorToString);
  TypeManager::getInstance().registerType<double>((int)te::layout::LayoutTypes::DOUBLE_TYPE, "double", ConvertStringToDouble, ConvertDoubleToString);
  TypeManager::getInstance().registerType<te::gm::Envelope>((int)te::layout::LayoutTypes::ENVELOPE_TYPE, "envelope", ConvertStringToEnvelope, ConvertEnvelopeToString);
  TypeManager::getInstance().registerType<te::layout::Font>((int)te::layout::LayoutTypes::FONT_TYPE, "font", ConvertStringToFont, ConvertFontToString);
  TypeManager::getInstance().registerType<te::gm::Geometry*>((int)te::layout::LayoutTypes::GEOMETRY_TYPE, "geometry", ConvertStringToGeometry, ConvertGeometryToString);
  TypeManager::getInstance().registerType<int>((int)te::layout::LayoutTypes::INT_TYPE, "int", ConvertStringToInt, ConvertIntToString);
  TypeManager::getInstance().registerType<std::string>((int)te::layout::LayoutTypes::STRING_TYPE, "string", 0, 0);
  TypeManager::getInstance().registerType< std::vector<std::string> >((int)te::layout::LayoutTypes::STRING_LIST_TYPE, "string_list", ConvertStringToStringList, ConvertStringListToString);
  TypeManager::getInstance().registerType< std::vector< std::vector< std::string> > >((int)te::layout::LayoutTypes::STRING_MATRIX_TYPE, "string_matrix", ConvertStringToStringMatrix, ConvertStringMatrixToString);
  TypeManager::getInstance().registerType< std::list<te::map::AbstractLayerPtr> >((int)te::layout::LayoutTypes::LAYER_LIST_TYPE, "layer_list", ConvertStringToAbstractLayerList, ConvertAbstractLayerListToString);
}
