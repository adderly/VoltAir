#ifndef EDITOR_H
#define EDITOR_H

#include <QQuickItem>
#include <vector>

/**
*   @breif
*
*   Graphical editor for creating awesome stuff. ;D
*
*
*
*/
class Level;
class UiInternal;

class Editor : public QQuickItem
{
    Q_OBJECT
public:
    ~Editor();

    /**
    * Init the resources to the engine.
    * -- Create the EditorUI component
    * -- Add *this, as a property into the qml context for easing access from qml.
    * -- Call @initUi();
    *
    * Try to load saved levels, and maybe a config.
    */
    void init();
    /**
    *   The Application will have top menu, defined in qml.
    */
    void initUi();

    /**
    *   Initialize a new level properties.
    */
    void initLevel(Level* level);

    /**
    *  Save the currently editing level.
    */
    void saveLevel();

    /**
    *   Delete level by name.
    */
    void deleteLevel(QString name);

    /**
    *   Create a new level, is the optional name.
    */
    Q_INVOKABLE void newLevel(QString levelName = QString());

    /**
    *   Export levels to another directory.
    *   http://stackoverflow.com/questions/18711421/partial-serialization-of-qml-object-properties
    */
    void exportLevel(QString path);

    static Editor* getInstance();
signals:


public slots:

    /**
    *   When loading UI component, sometimes is not ready.
    *   This is the callback used to set things up.
    */
    void onLoadingUiChanged(QQmlComponent::Status);

    /**
    *   Exit from the menu.
    */
    void onMenuExit();

private:
    Editor(QQuickItem* parent = 0);

    QString m_default_save_path;
    bool m_initialized;
    bool m_ui_initialized;
    QQmlComponent *editorUiComponent;
    UiInternal* ui = nullptr;

    //All levels Available.
    std::vector<Level*> m_levels;
    Level* m_current;
    static Editor* editorInstance;
};
Q_DECLARE_METATYPE(Editor*)

#endif // EDITOR_H
