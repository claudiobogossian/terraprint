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
  \file DialogPropertiesBrowser.h
   
   \brief Manage properties dialogs values. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_DIALOG_PROPERTIES_BROWSER_H 
#define __TERRALIB_LAYOUT_INTERNAL_DIALOG_PROPERTIES_BROWSER_H

// TerraLib
#ifndef Q_MOC_RUN
#include "AbstractPropertiesBrowser.h"
#include "../../../core/Config.h"
#include "../../../core/property/Property.h"
#endif

// STL
#include <map>
#include <string>
#include <vector>

// Qt
#include <QObject>

// QtPropertyBrowser
class QtStringPropertyManager;
class QtColorPropertyManager;
class QtDlgEditorFactory;
class QtProperty;

class QGraphicsItem;
class QWidget;

namespace te
{
  namespace layout
  {
    class Properties;
    class EnumDataType;
    class EnumType;
    class Scene;
    class AbstractProxyProject;

    /*!
    \brief Manage properties dialogs values. 
    
    \ingroup layout
    */
    class TELAYOUTEXPORT DialogPropertiesBrowser : public AbstractPropertiesBrowser
    {
      Q_OBJECT //for slots/signals

      public:

        DialogPropertiesBrowser(Scene* scene, AbstractProxyProject* proxyProject, QObject *parent = 0);

        virtual ~DialogPropertiesBrowser();

        QtStringPropertyManager* getStringPropertyManager();

        QtColorPropertyManager* getColorPropertyManager();

        QtDlgEditorFactory* getDlgEditorFactory();

        virtual void closeAllWindows();

        virtual bool hasOpenWindows();
        
        virtual QtProperty* addProperty(const Property& property);

        virtual bool updateProperty(const Property& property);
      
        virtual Property getProperty(QtProperty* qtProperty);

        virtual void clearAll();

        /*!
          \brief Opens a corresponding dialog window to an existing property in the Property Browser. 
            Example usage: Click on the context menu for an item, the user choose to open a window.
          \param prop corresponding property choice
        */
        virtual void directlyShowWindow(const Property& prop);
            
      protected slots:

         /*
          \brief Call when one of the Windows was close.
        */
        virtual void onDestroyed(QObject* obj = 0);

        /*
          \brief By default it is connected with the internalDlg method of the class QtDlgEditorFactory. 
            The internalDlg method is called when an item of property browser tree is clicked.
        */

        virtual void onSetDlg(QWidget *parent, QtProperty * prop);

        virtual void updateOutside(const Property& prop);

        virtual void onShowGridSettingsDlg();

        virtual void onShowTextGridSettingsDlg();

        virtual void onShowFontDlg();

        virtual void onShowColorDlg();

        virtual void onShowMapLayerChoiceDlg();

        virtual void onShowLegendChoiceDlg();

        virtual void onShowViewDlg();

        virtual void onShowMapSettingsDlg();

        virtual void onShowMapLocationSettingsDlg();

        virtual void onShowScaleSettingsDlg();

        virtual void onShowNorthSettingsDlg();

      public slots:

      virtual void onShowImageDlg();
      
      virtual void onShowFolderDlg();

      signals:

        void changeDlgProperty(Property property);

      protected:

        virtual void createManager();

        virtual void changeValueQtPropertyDlg(const QString& label, const QVariant& variant);

        virtual QWidget* createOutside(EnumType* enumType);

        virtual void appendDialog(QWidget* widget);
      
      protected:

        QtStringPropertyManager*        m_strDlgManager;
        QtColorPropertyManager*         m_colorDlgManager;
        QtDlgEditorFactory*             m_dlgEditorFactory;
        Property                        m_currentPropertyClicked;
        QList<QWidget*>                 m_dialogs;
        AbstractProxyProject*           m_proxyProject;
    };
  }
}

#endif



