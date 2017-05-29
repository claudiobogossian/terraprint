#include "ColorEditorFactory.h"

#include "ColorEditWidget.h"

#include <QtPropertyBrowser/qteditorfactory.h>


te::layout::ColorEditorFactory::ColorEditorFactory(QObject *parent) :
  QtAbstractEditorFactory<QtColorPropertyManager>(parent)
{
}

te::layout::ColorEditorFactory::~ColorEditorFactory()
{
  qDeleteAll(m_editorToProperty.keys());
}

void te::layout::ColorEditorFactory::connectPropertyManager(QtColorPropertyManager *manager)
{
  connect(manager, SIGNAL(valueChanged(QtProperty*, QColor)), this, SLOT(slotPropertyChanged(QtProperty*, QColor)));
}

QWidget* te::layout::ColorEditorFactory::createEditor(QtColorPropertyManager *manager, QtProperty *property, QWidget *parent)
{
  ColorEditWidget *editor = new ColorEditWidget(parent);
  editor->setValue(manager->value(property));
  editor->setTitle(property->propertyName());

  m_createdEditors[property].append(editor);
  m_editorToProperty[editor] = property;

  connect(editor, SIGNAL(valueChanged(const QColor&)), this, SLOT(slotSetValue(const QColor&)));
  connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
  return editor;
}

void te::layout::ColorEditorFactory::disconnectPropertyManager(QtColorPropertyManager *manager)
{
  disconnect(manager, SIGNAL(valueChanged(QtProperty*, QColor)), this, SLOT(slotPropertyChanged(QtProperty*, QColor)));
}

void te::layout::ColorEditorFactory::slotPropertyChanged(QtProperty *property, const QColor &value)
{
  const QMap<QtProperty *, QList<ColorEditWidget*> >::iterator it = m_createdEditors.find(property);
  if (it == m_createdEditors.end())
    return;

  QListIterator<ColorEditWidget *> itEditor(it.value());
  while (itEditor.hasNext())
    itEditor.next()->setValue(value);
}

void te::layout::ColorEditorFactory::slotSetValue(const QColor &value)
{
  QObject *object = sender();
  QMap<ColorEditWidget*, QtProperty*>::ConstIterator itEditor = m_editorToProperty.constBegin();

  while (itEditor != m_editorToProperty.constEnd())
  {
    if (itEditor.key() == object)
    {
      QtProperty *property = itEditor.value();
      QtColorPropertyManager *manager = propertyManager(property);
      if (!manager)
        return;
      manager->setValue(property, value);
      return;
    }
    ++itEditor;
  }
}

void te::layout::ColorEditorFactory::slotEditorDestroyed(QObject *object)
{
  QMap<ColorEditWidget*, QtProperty*>::ConstIterator itEditor = m_editorToProperty.constBegin();

  while (itEditor != m_editorToProperty.constEnd())
  {
    if (itEditor.key() == object)
    {
      ColorEditWidget* editor = itEditor.key();
      QtProperty *property = itEditor.value();
      m_editorToProperty.remove(editor);
      m_createdEditors[property].removeAll(editor);
      if (m_createdEditors[property].isEmpty())
        m_createdEditors.remove(property);
      return;
    }
    ++itEditor;
  }
}
