#include "Editor.h"
#include "LevelInfo.h"
#include "Level.h"
#include "UiInternal.h"
#include <Engine.h>
#include <Engine/utils/Util.h>
#include <QAction>
#include <QQmlContext>
#include <QQmlEngine>
#include <QGuiApplication>

Editor* Editor::editorInstance = nullptr;

Editor* Editor::getInstance(){
    if(!editorInstance){
        editorInstance = new Editor();
    }
    return editorInstance;
}


Editor::Editor(QQuickItem* parent):
    QQuickItem(parent),m_initialized(false),m_ui_initialized(false)
{
}

Editor::~Editor(){}

void Editor::init()
{
    if(m_initialized){
        return;
    }
    Engine* engine = Engine::getInstance();
    QQmlEngine* qmlEngine = engine->getQmlEngine();

    QQmlContext* context = qmlEngine->rootContext();
    context->setContextProperty("editor",this);

    editorUiComponent = new QQmlComponent(qmlEngine,Util::getUrlPathToAsset("qml/editor/EditorUi.qml"));

    for(QQmlError error :editorUiComponent->errors()){
        qDebug() << "Error on Line" << error.line() << ":" << error.description();
    }

    if(editorUiComponent->isReady()){
        initUi();
    }else {
        connect(editorUiComponent,SIGNAL(statusChanged(QQmlComponent::Status)),
                this,SLOT(onLoadingUiChanged(QQmlComponent::Status)));
    }

    m_initialized = true;
}


void Editor::initUi()
{
    Engine* engine = Engine::getInstance();
    //QObject* editorUi = editorUiComponent.create();
    ui = qobject_cast<UiInternal*>(editorUiComponent->create());

    if(ui){
        ui->setFocus(true);
        engine->addChildItem(ui);
        engine->setFullscreen(false);

        m_ui_initialized = true;
    }
    //we cannot have this happen here
    Q_ASSERT(ui!=nullptr);
}



void Editor::initLevel(Level *level){
    Q_UNUSED(level)
}

void Editor::saveLevel(){}

void Editor::deleteLevel(QString name){
    Q_UNUSED(name)
}

void Editor::newLevel(QString levelName){
    Q_UNUSED(levelName)
    LevelInfo* levelInfo = new LevelInfo;
    levelInfo->setFileName("qml/editor/LevelTest.qml");
    levelInfo->setName("TEST LEVEL");
    levelInfo->setFileInLevelPath(false);
    Engine::getInstance()->loadLevel(levelInfo);
}



void Editor::exportLevel(QString path){
    Q_UNUSED(path)
}

void Editor::onMenuExit(){
    Engine::getInstance()->onQuitRequested();
    QGuiApplication::quit();
}

void Editor::onLoadingUiChanged(QQmlComponent::Status status){
    Q_UNUSED(status)
   if(editorUiComponent->isReady()){
       initUi();
   }else if(editorUiComponent->isError()){
     //Problem loading editorComponent
     Q_ASSERT(true);
   }
}

