#include "editor.h"
#include "Level.h"
#include "UiInternal.h"
#include <Engine.h>
#include <Engine/utils/Util.h>

Editor* Editor::editorInstance = nullptr;

Editor* Editor::getInstance(){
    if(!editorInstance){
        editorInstance = new Editor();
    }
    return editorInstance;
}


Editor::Editor(QQuickItem* parent):
    QQuickItem(parent)
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
    editorUiComponent = new QQmlComponent(qmlEngine,Util::getUrlPathToAsset("qml/editor/EditorUi.qml"));

    if( editorUiComponent->isLoading()){
        connect(editorUiComponent,SIGNAL(statusChanged(QQmlComponent::Status)),
                this,SLOT(onLoadingUiChanged(QQmlComponent::Status)));
    }
    else{
        initUi();
    }

    m_initialized = true;
}


void Editor::initUi()
{
    if(!editorUiComponent->isReady()){
        qDebug() << "Init ui , component not ready...";
        return;
    }
    Engine* engine = Engine::getInstance();
    //QObject* editorUi = editorUiComponent.create();
    ui = qobject_cast<UiInternal*>(editorUiComponent->create());

    for(QQmlError error :editorUiComponent->errors()){
        qDebug() << "Error on Line" << error.line() << ":" << error.description();
    }

    ui->setFocus(true);
    engine->addChildItem(ui);

    engine->setFullscreen(false);
    m_ui_initialized = true;
}

void Editor::initLevel(Level *level){}

void Editor::saveLevel(){}

void Editor::deleteLevel(QString name){}

void Editor::newLevel(QString levelName){}

void Editor::exportLevel(QString path){}

void Editor::onLoadingUiChanged(QQmlComponent::Status status){
   if(editorUiComponent->isError()){

   }
   else if(editorUiComponent->isReady()){
       initUi();
   }
}

