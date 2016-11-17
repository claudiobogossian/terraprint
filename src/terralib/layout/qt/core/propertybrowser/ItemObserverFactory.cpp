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
    \file ItemObserverFactory.cpp

    \brief

    \ingroup layout
*/

// TerraLib
#include "ItemObserverFactory.h"
#include "ItemObserverProperty.h"

te::layout::ItemObserverFactory::ItemObserverFactory(QObject *parent )
: QtAbstractEditorFactory<ItemObserverManager>(parent)
{

}

te::layout::ItemObserverFactory::~ItemObserverFactory()
{
  QList<ItemObserverProperty *> editors = m_editorToProperty.keys();
  QListIterator<ItemObserverProperty *> it(editors);
  while (it.hasNext())
    delete it.next();
}

void te::layout::ItemObserverFactory::connectPropertyManager(ItemObserverManager *manager)
{
  connect(manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
    this, SLOT(slotPropertyChanged(QtProperty *, const QVariant &)));
  connect(manager, SIGNAL(listNamesChanged(QtProperty *, const QStringList &)),
    this, SLOT(slotListNamesChanged(QtProperty *, const QStringList &)));
}

QWidget *te::layout::ItemObserverFactory::createEditor(ItemObserverManager *manager,
        QtProperty *property, QWidget *parent)
{
  ItemObserverProperty *editor = new ItemObserverProperty(parent);
  editor->setNameList(manager->names(property));
  editor->setCurrentName(manager->value(property));
  m_createdEditors[property].append(editor);
  m_editorToProperty[editor] = property;

  connect(editor, SIGNAL(currentNameChanged(const QString &)),
    this, SLOT(slotSetValue(const QString &)));
  connect(editor, SIGNAL(destroyed(QObject *)),
    this, SLOT(slotEditorDestroyed(QObject *)));
  return editor;
}

void te::layout::ItemObserverFactory::disconnectPropertyManager(ItemObserverManager *manager)
{
  disconnect(manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
    this, SLOT(slotPropertyChanged(QtProperty *, const QVariant &)));
  disconnect(manager, SIGNAL(listNamesChanged(QtProperty *, const QStringList &)),
    this, SLOT(slotListNamesChanged(QtProperty *, const QStringList &)));
}

void te::layout::ItemObserverFactory::slotPropertyChanged(QtProperty *property,
  const QVariant &value)
{
  if (!m_createdEditors.contains(property))
    return;

  QString name = value.toString();

  QList<ItemObserverProperty *> editors = m_createdEditors[property];
  QListIterator<ItemObserverProperty *> itEditor(editors);
  while (itEditor.hasNext())
    itEditor.next()->setCurrentName(name);
}

void te::layout::ItemObserverFactory::slotListNamesChanged(QtProperty *property,
  const QStringList &names)
{
  if (!m_createdEditors.contains(property))
    return;

  QList<ItemObserverProperty *> editors = m_createdEditors[property];
  QListIterator<ItemObserverProperty *> itEditor(editors);
  while (itEditor.hasNext())
    itEditor.next()->setNameList(names);
}

void te::layout::ItemObserverFactory::slotSetValue(const QString &value)
{
  QObject *object = sender();
  QMap<ItemObserverProperty *, QtProperty *>::ConstIterator itEditor = m_editorToProperty.constBegin();

  while (itEditor != m_editorToProperty.constEnd())
  {
    if (itEditor.key() == object) 
    {
      QtProperty *property = itEditor.value();
      ItemObserverManager *manager = propertyManager(property);
      if (!manager)
        return;
      manager->setValue(property, value);
      return;
    }
    itEditor++;
  }
}

void te::layout::ItemObserverFactory::slotEditorDestroyed(QObject *object)
{
  QMap<ItemObserverProperty *, QtProperty *>::ConstIterator itEditor =
    m_editorToProperty.constBegin();

  while (itEditor != m_editorToProperty.constEnd()) 
  {
    if (itEditor.key() == object) 
    {
      ItemObserverProperty *editor = itEditor.key();
      QtProperty *property = itEditor.value();
      m_editorToProperty.remove(editor);
      m_createdEditors[property].removeAll(editor);
      if (m_createdEditors[property].isEmpty())
        m_createdEditors.remove(property);
      return;
    }
    itEditor++;
  }
}

