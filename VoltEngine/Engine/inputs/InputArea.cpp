/*
 * Copyright (C) 2014 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <GameInput/ControllerEvent.h>
#include <GameInput/routers/KeyboardRouter.h>
#include <cassert>
#include "Engine.h"
#include "InputArea.h"
#include "Camera.h"
#include "UiInternal.h"
#include "utils/Util.h"

QMap<int, KeyCode> InputArea::sQtKeyConversionMap({
    // TODO: Find back quote
    { Qt::Key_1, KEYCODE_1 },
    { Qt::Key_2, KEYCODE_2 },
    { Qt::Key_3, KEYCODE_3 },
    { Qt::Key_4, KEYCODE_4 },
    { Qt::Key_5, KEYCODE_5 },
    { Qt::Key_6, KEYCODE_6 },
    { Qt::Key_7, KEYCODE_7 },
    { Qt::Key_8, KEYCODE_8 },
    { Qt::Key_9, KEYCODE_9 },
    { Qt::Key_0, KEYCODE_0 },
    { Qt::Key_Minus, KEYCODE_MINUS },
    { Qt::Key_Equal, KEYCODE_EQUALS },
    // TODO: Backspace / delete?

    { Qt::Key_Tab, KEYCODE_TAB },
    { Qt::Key_Q, KEYCODE_Q },
    { Qt::Key_W, KEYCODE_W },
    { Qt::Key_E, KEYCODE_E },
    { Qt::Key_R, KEYCODE_R },
    { Qt::Key_T, KEYCODE_T },
    { Qt::Key_Y, KEYCODE_Y },
    { Qt::Key_U, KEYCODE_U },
    { Qt::Key_I, KEYCODE_I },
    { Qt::Key_O, KEYCODE_O },
    { Qt::Key_P, KEYCODE_P },
    { Qt::Key_BracketLeft, KEYCODE_LEFT_BRACKET },
    { Qt::Key_BracketRight, KEYCODE_RIGHT_BRACKET },
    { Qt::Key_Backslash, KEYCODE_BACKSLASH },

    { Qt::Key_CapsLock, KEYCODE_CAPS_LOCK },
    { Qt::Key_A, KEYCODE_A },
    { Qt::Key_S, KEYCODE_S },
    { Qt::Key_D, KEYCODE_D },
    { Qt::Key_F, KEYCODE_F },
    { Qt::Key_G, KEYCODE_G },
    { Qt::Key_H, KEYCODE_H },
    { Qt::Key_J, KEYCODE_J },
    { Qt::Key_K, KEYCODE_K },
    { Qt::Key_L, KEYCODE_L },
    { Qt::Key_Semicolon, KEYCODE_SEMICOLON },
    { Qt::Key_Apostrophe, KEYCODE_APOSTROPHE },
    { Qt::Key_Enter, KEYCODE_ENTER },

    { Qt::Key_Shift, KEYCODE_SHIFT_LEFT },
    { Qt::Key_Z, KEYCODE_Z },
    { Qt::Key_X, KEYCODE_X },
    { Qt::Key_C, KEYCODE_C },
    { Qt::Key_V, KEYCODE_V },
    { Qt::Key_B, KEYCODE_B },
    { Qt::Key_N, KEYCODE_N },
    { Qt::Key_M, KEYCODE_M },
    { Qt::Key_Comma, KEYCODE_COMMA },
    { Qt::Key_Period, KEYCODE_PERIOD },
    { Qt::Key_Slash, KEYCODE_SLASH },
    // Unable to distinguish shift keys...

    { Qt::Key_Space, KEYCODE_SPACE },
    { Qt::Key_Up, KEYCODE_DPAD_UP },
    { Qt::Key_Left, KEYCODE_DPAD_LEFT },
    { Qt::Key_Down, KEYCODE_DPAD_DOWN },
    { Qt::Key_Right, KEYCODE_DPAD_RIGHT },
});

InputArea::InputArea(QQuickItem* parent) : QQuickItem(parent) {
    setActiveFocusOnTab(true);
}

void InputArea::keyPressEvent(QKeyEvent* event) {
    // See InputArea file comment.
#ifdef Q_OS_ANDROID
    QQuickItem::keyPressEvent(event);
#else
    // Since the UI receives events before the InputArea, we do not need to check for
    // any special UI-affecting keys here as we have already done so.

    // Convert QKeyEvent to ControllerEvent
    ControllerEvent controllerEvent;
    controllerEvent.setKeyState(convertQtKeyCode(event->key()), true);

    // Give the event to the KeyboardRouter to handle, if it doesn't handle
    // the event, pass the original event to the default QQuickItem handler.
    if (KeyboardRouter::getInstance()->routeControllerEvent(&controllerEvent)) {
        event->accept();
    } else {
        QQuickItem::keyPressEvent(event);
    }
#endif
}

void InputArea::keyReleaseEvent(QKeyEvent* event) {
    // See InputArea file comment.
#ifdef Q_OS_ANDROID
    QQuickItem::keyPressEvent(event);
#else
    // Since the UI receives events before the InputArea, we do not need to check for
    // any special UI-affecting keys here as we have already done so.

    ControllerEvent controllerEvent;
    controllerEvent.setKeyState(convertQtKeyCode(event->key()), false);

    // Give the event to the KeyboardRouter to handle, if it doesn't handle
    // the event, pass the original event to the default QQuickItem handler.
    if (KeyboardRouter::getInstance()->routeControllerEvent(&controllerEvent)) {
        event->accept();
    } else {
        QQuickItem::keyReleaseEvent(event);
    }
#endif
}

void InputArea::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Clicked inputArea";
    QQuickItem::mousePressEvent(event);
}

void InputArea::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << event->modifiers();
    QPoint pos = event->pos();
    emit mouseMovedChanged(pos,mDeltaPos);
    mDeltaPos = pos;
    qDebug() << "Pos = " << pos << " DeltaPos = " << mDeltaPos;
    QQuickItem::mouseMoveEvent(event);
}

void InputArea::mouseReleaseEvent(QMouseEvent *event)
{
    QQuickItem::mouseReleaseEvent(event);
}

void InputArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    QQuickItem::mouseDoubleClickEvent(event);
}

void InputArea::wheelEvent(QWheelEvent *event)
{
    emit mouseWheelChanged(event->pixelDelta(),event->angleDelta());
//    TODO: add logging OPTIONS
#ifdef PRINT_MOUSE_INTERACTION
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

      if (!numPixels.isNull()) {
          qDebug() << " wheelPixel : " << numPixels;
      } else if (!numDegrees.isNull()) {
          QPoint numSteps = numDegrees / 15;
          qDebug() << "wheelDegrees : " << numDegrees;
      }
#endif
    QQuickItem::wheelEvent(event);
}

KeyCode InputArea::convertQtKeyCode(int qtKeyCode) {
    return sQtKeyConversionMap.value(qtKeyCode, KEYCODE_UNKNOWN);
}

void InputArea::mouseEventFromUi(const QEvent *e)
{
    const QMouseEvent* event = static_cast<const QMouseEvent*>(e);
    if(event->modifiers().testFlag(Qt::ControlModifier)){
        switch(event->type()){
            case QEvent::MouseButtonPress:{
                mDeltaPos = event->pos();
//                qDebug() << "button pressed";
            }break;
            case QEvent::MouseMove:{
                QPoint pos = event->pos();
                QPoint delta = mDeltaPos - event->pos();
//                qDebug() << "1 Pos = " << pos << " DeltaPos = " << mDeltaPos;
                emit mouseMovedChanged(pos,delta);
//                qDebug() << "2 Pos = " << pos << " DeltaPos = " << mDeltaPos;
            }
                break;
            default:break;
        }
    }

    Camera* camera = Engine::getInstance()->getCamera();
    QPoint epos = event->pos();
    QPointF worldCoords = camera->mapFromItem(this,QPointF(event->pos()));
    QQuickItem* item = childAt(worldCoords.x(),worldCoords.y());


    qDebug() <<"MouseX "<< event->x() << " MouseY " << event->y();
    qDebug() <<"MouseWorldX "<< worldCoords.x() << " MouseWorldY " << worldCoords.y();
//    for(ResizeControl* actor: findChildren<ResizeControl*>()){
//        if(actor->position().toPoint() == worldCoords.toPoint()){

//            qDebug() << "Item Found At  posX = " << epos.x() << " posY = "<< epos.y();
//            qDebug() << "Item \n  ItemX = " << actor->x() << "  ItemY = "<< actor->y();
//            qDebug() << "  ItemWidth = " << actor->width() << "  ItemHeight = "<< actor->width();
//            qDebug() << "Class Info = " <<actor->metaObject()->className();

//            const QMetaObject* metaObject = actor->metaObject();
//            QStringList properties;
//            for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
//                properties << QString::fromLatin1(metaObject->property(i).name());

//            qDebug() << properties;
//            mTransforming = true;
//            transformedItem = qobject_cast<QQuickItem*>(actor);
//            event->accept();
//        }
//    }
//    if(item){
//        qDebug() << "Item Found "  ;
//        qDebug() << "Class Info = " <<item->metaObject()->className();
//    }
}
