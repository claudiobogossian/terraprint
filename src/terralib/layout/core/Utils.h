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
  \file Utils.h
   
  \brief Utility class with functions to manipulate the canvas and conversion between projections.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_UTILS_H 
#define __TERRALIB_LAYOUT_INTERNAL_UTILS_H

// TerraLib
#include "terralib/common/UnitOfMeasure.h"
#include "terralib/maptools/Enums.h"
#include "Config.h"

// STL
#include <string>
#include <iostream>
#include <fstream>

class QPointF;

namespace te
{
  namespace gm
  {
    class Envelope;
    class LinearRing;
    class Point;
    class Polygon;
  }

  namespace map
  {
    class WorldDeviceTransformer;
  }

  namespace layout
  {
    class PaperConfig;
    class Properties;
    class WorldTransformer;
    class Scene;

    /*!
      \brief Utility class with functions to manipulate the canvas and conversion between projections.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT Utils
    {
      public:

        /*!
          \brief Constructor
        */ 
        Utils(Scene* scene);

        /*!
          \brief Destructor
        */
        virtual ~Utils();

        /*!
          \brief Millimeter to pixel
      
          \param millimeter value
          \return pixel value
        */ 
        virtual int mm2pixel(double mm);

        /*!
        \brief Millimeter to pixel

        \param millimeter value
        \return pixel value
        */
        static int mm2pixel(double mm, double dpi);

        /*!
          \brief Pixel to millimeter
      
          \param pixel value
          \return millimeter value
        */ 
        virtual double pixel2mm(int pixel);

        /*!
        \brief Pixel to millimeter

        \param pixel value
        \return millimeter value
        */
        static double pixel2mm(int pixel, double dpi);

        /*!
          \brief Creates a line with n points in x axis. Method used to create the grid lines on a map. Horizontal line.
            
      
          \param box in mm
          \param axisCoord axis y value
          \param gap distance between the points on the x axis
          \return
        */ 
        virtual te::gm::LinearRing* addCoordsInX(te::gm::Envelope box, double axisCoord, double gap);

        /*!
          \brief Creates a line with n points in y axis. Method used to create the grid lines on a map. Vertical line.
      
          \param box in mm
          \param axisCoord axis x value
          \param gap distance between the points on the y axis
          \return
        */ 
        virtual te::gm::LinearRing* addCoordsInY(te::gm::Envelope box, double axisCoord, double gap);

        /*!
          \brief Converts the box world (mm) to screen coordinates (pixel).
      
          \param box in mm    
          \return box in pixel
        */ 
        virtual te::gm::Envelope viewportBox(te::gm::Envelope box);

        /*!
          \brief Returns a WorldTransformer object to transformations between geo coordinates and millimeter coordinates.
                
          \param box in geo coordinates     
          \return box in mm
        */ 
        te::layout::WorldTransformer getTransformGeo(te::gm::Envelope boxgeo, te::gm::Envelope boxmm);

        /*!
          \brief Converts decimal geo coordinates to degrees.
      
          \param value in decimal
          \param bDegrees true if should appear in the return string, false otherwise
          \param bMinutes true if should appear in the return string, false otherwise
          \param bSeconds true if should appear in the return string, false otherwise
          \return string value in degree
        */ 
        virtual std::string convertDecimalToDegree(const double& value, bool bDegrees, bool bMinutes, bool bSeconds,  int precision = 0);

        /*!
        \brief Converts decimal geo coordinates to degrees, using the ANP pattern.

        \param value in decimal
        \param bDegrees true if should appear in the return string, false otherwise
        \param bMinutes true if should appear in the return string, false otherwise
        \param bSeconds true if should appear in the return string, false otherwise
        \return string value in degree
        */
        virtual std::string convertDecimalToDegreeANP(const double& value, bool bDegrees, bool bMinutes, bool bSeconds, int precision = 0);

        /*!
        \brief Concatenates the given 'lpadValue' to the left of the given 'text' until it reaches the given 'length'
        */
        virtual std::string lpadString(std::string& text, size_t length, char lpadValue);

        /*!
          \brief Converts degree geo coordinates to decimal.
          
          \return double value in decimal
        */ 
        virtual double convertDegreeToDecimal();

        /*!
          \brief Rounds double to int.
      
          \param value   
          \return number rounded
        */ 
        static int roundNumber(const double& value);

        /*!
          \brief Returns string wkt with non-planar projection.
     
          \return wkt
        */
        std::string proj4DescToGeodesic();

        /*!
          \brief Returns a UnitOfMeasurePtr pointer.
      
          \param srid projection number  
          \return A UnitOfMeasurePtr pointer
        */
        te::common::UnitOfMeasurePtr unitMeasure(int srid);

        /*!
          \brief Map latlong to UTM zone.
      
          \param box in latlong
          \param zone returns UTM zone
        */
        virtual void remapToPlanar(te::gm::Envelope* latLongBox, int zone);

        /*!
          \brief Map latlong LinearRing (line) to UTM zone.
      
          \param line line in latlong
          \param zone returns UTM zone
        */
        virtual void remapToPlanar(te::gm::LinearRing* line, int zone);

        /*!
          \brief Map latlong Point (point) to UTM zone.
      
          \param zone returns UTM zone    
        */
        virtual void remapToPlanar(te::gm::Point* point, int zone);

        /*!
          \brief Convert LinearRing from one coordinate system to mm
      
          \param Object with logic for transforming
          \param line LinearRing pointer in one coordinate system
        */
        virtual void convertToMillimeter(WorldTransformer transf, te::gm::LinearRing* line); 

        /*!
          \brief Convert Polygon from one coordinate system to mm
      
          \param Object with logic for transforming
          \param line LinearRing pointer in one coordinate system
        */
        virtual void convertToMillimeter(WorldTransformer transf, te::gm::Polygon* poly); 

        /*!
          \brief Converts from PaperConfig to Properties
        */
        static Properties convertToProperties(const PaperConfig& paperConfig);

        /*!
          \brief Converts from Properties to Paper Config
        */
        static PaperConfig convertToPaperConfig(const Properties& properties);

        virtual te::gm::Envelope getWorldBoxInGeographic(const te::gm::Envelope& worldBox, int srid);

        static double getLineWidthMinimumValue();

        static void setLineWidthMinimumValue(double minimumValue);

        static double round(double n, double precision);

        static double calculateAngle(QPointF p1, QPointF p2);

        static std::vector<std::string> Tokenize(const std::string& value, const std::string& separator);

      protected:
        
        /*!
          \brief Transforms the box (mm) to screen coordinates (pixel).
      
          \param transf logic for transforming from device coordinate to world coordinate and vice-versa.     
          \return box in screen coordinates (pixel)
        */
        virtual te::gm::Envelope transformToViewport(te::map::WorldDeviceTransformer transf, te::gm::Envelope box);
        
        /*!
          \brief Converts the box world (mm) to screen coordinates (pixel).
      
          \param box in mm   
        */ 
        virtual te::gm::Envelope viewportBoxFromMM(te::gm::Envelope box);

        
      protected:

        bool                      m_applyZoom; //!<
        Scene*                    m_scene;
        static double             m_lineWidthMinimumValue;
    };
  }
}

#endif 
