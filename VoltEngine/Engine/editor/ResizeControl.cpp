#include "ResizeControl.h"
#include <QBrush>
#include <QColor>
#include <QPainter>
#include "Engine.h"
#include "Camera.h"

ResizeControl::ResizeControl(QQuickItem* parent):Graphic(parent)
{
    setFlag(ItemHasContents);

    if(parent){

    }
}

ResizeControl::~ResizeControl()
{

}

//void ResizeComponent::paint(QPainter *painter)
//{
//    QBrush brush(QColor("#007430"));
//    painter->setBrush(brush);
//    painter->setPen(Qt::NoPen);
//    //painter->setRenderHint(QPainter::Antialiasing);

//    painter->drawRoundedRect(x(), y(), width(), height(), 10, 10);
//}

void ResizeControl::initControls()
{
}

QSGNode* ResizeControl::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data){
   node = QQuickItem::updatePaintNode(node,data);

    data->transformNode->setMatrix(_m);


    if(node)        node->markDirty(QSGNode::DirtyOpacity);
    return node;
}

void ResizeControl::componentComplete(){
    Graphic::componentComplete();

    Camera* cam = Engine::getInstance()->getCamera();
   // _m = cam->getOpenGLViewMatrix();
}

void ResizeControl::mousePressEvent(QMouseEvent *event){
    this->setVisible(false);
}

void ResizeControl::mouseReleaseEvent(QMouseEvent *event){
    this->setVisible(true);
}


