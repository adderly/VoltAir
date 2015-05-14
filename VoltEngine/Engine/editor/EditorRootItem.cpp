#include "EditorRootItem.h"
#include <QMimeData>
#include "Engine.h"
#include "Camera.h"
#include "Actor.h"
#include "ResizeControl.h"

EditorRootItem::EditorRootItem(QQuickItem* parent):
        QQuickItem(parent)
{
    setFlag(QQuickItem::ItemAcceptsDrops,true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
}

EditorRootItem::~EditorRootItem()
{

}

void EditorRootItem::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void EditorRootItem::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "Drag moved";
}

void EditorRootItem::dragLeaveEvent(QDragLeaveEvent *event)
{
    unsetCursor();
}

void EditorRootItem::dropEvent(QDropEvent *event)
{
    qDebug() <<"Mime of the drop: "<< event->mimeData()->text();
    unsetCursor();
}

void EditorRootItem::mousePressEvent(QMouseEvent *event)
{
    QQuickItem::mousePressEvent(event);
    Camera* camera = Engine::getInstance()->getCamera();
    QPoint epos = event->pos();
    QPointF worldCoords = camera->mapFromItem(this,QPointF(event->pos()));
    QQuickItem* item = childAt(worldCoords.x(),worldCoords.y());


    qDebug() <<"MouseX "<< event->x() << " MouseY " << event->y();
    qDebug() <<"MouseWorldX "<< worldCoords.x() << " MouseWorldY " << worldCoords.y();
    for(ResizeControl* actor: findChildren<ResizeControl*>()){
        if(actor->position().toPoint() == worldCoords.toPoint()){

            qDebug() << "Item Found At  posX = " << epos.x() << " posY = "<< epos.y();
            qDebug() << "Item \n  ItemX = " << actor->x() << "  ItemY = "<< actor->y();
            qDebug() << "  ItemWidth = " << actor->width() << "  ItemHeight = "<< actor->width();
            qDebug() << "Class Info = " <<actor->metaObject()->className();

            const QMetaObject* metaObject = actor->metaObject();
            QStringList properties;
            for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
                properties << QString::fromLatin1(metaObject->property(i).name());

            qDebug() << properties;
            mTransforming = true;
            transformedItem = qobject_cast<QQuickItem*>(actor);
            event->accept();
        }
    }
    if(item){
        qDebug() << "Item Found "  ;
        qDebug() << "Class Info = " <<item->metaObject()->className();
    }

}

void EditorRootItem::mouseMoveEvent(QMouseEvent *event)
{

    qDebug() <<"move";
    QQuickItem* tparent =nullptr;

    if(mTransforming && transformedItem && (tparent = transformedItem->parentItem())){
        qDebug() << "increase size";
        tparent->setWidth(tparent->width()+0.5);
        tparent->setHeight(tparent->height()+0.5);
    }
}

void EditorRootItem::mouseReleaseEvent(QMouseEvent *event)
{

    qDebug() <<"release";
    mTransforming = false;
}

void EditorRootItem::hoverEnterEvent(QHoverEvent *event)
{
    qDebug() <<"hover";

}

