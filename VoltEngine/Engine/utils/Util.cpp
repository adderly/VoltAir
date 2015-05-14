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

#include <QCoreApplication>
#include <QDir>
#include <QObject>
#include <QMetaObject>
#include <QMetaMethod>
#include <QTextStream>
#include <QTouchEvent>
#include <QUrl>
#include <QWindow>
#include <algorithm>
#include "Util.h"

#ifdef Q_OS_ANDROID
#include "android/AndroidActivity.h"
#endif

QString Util::getPathToAsset(const QString& assetPath) {
    // Note: Adapted from QtQuick2ApplicationViewerPrivate::adjustPath.
#if defined(Q_OS_MAC)
    if (!QDir::isAbsolutePath(assetPath))
        return QString::fromLatin1("%1/../Resources/%2")
                .arg(QCoreApplication::applicationDirPath(), assetPath);
#elif defined(Q_OS_ANDROID)
    // This part differs from QtQuick2ApplicationViewerPrivate::adjustPath.
    return QString::fromLatin1("assets:/%1").arg(assetPath);
#elif defined(Q_OS_LINUX)
    if (!QDir::isAbsolutePath(assetPath))
        return QString::fromLatin1("%1/%2")
                .arg(QCoreApplication::applicationDirPath(), assetPath);
#endif
    return assetPath;
}

QUrl Util::getUrlPathToAsset(const QString& assetPath) {
    // Note: Adapted from QtQuick2ApplicationViewerPrivate::adjustPath.
#if defined(Q_OS_MAC)
    if (!QDir::isAbsolutePath(assetPath))
        return QString::fromLatin1("file://%1/../Resources/%2")
                .arg(QCoreApplication::applicationDirPath(), assetPath);
#elif defined(Q_OS_ANDROID)
    // This part differs from QtQuick2ApplicationViewerPrivate::adjustPath.
    return QString::fromLatin1("assets:/%1").arg(assetPath);
#elif defined(Q_OS_LINUX)
    if (!QDir::isAbsolutePath(assetPath))
        return QString::fromLatin1("file://%1/%2")
                .arg(QCoreApplication::applicationDirPath(), assetPath);
#endif
    return assetPath;
}

QString Util::getPathToFont(const QString& fontPath) {
    return Util::getPathToAsset(QString("assets/fonts/%1").arg(fontPath));
}

QString Util::getPathToImage(const QString& imagePath) {
    return Util::getPathToAsset(QString("assets/images/%1").arg(imagePath));
}

QString Util::getPathToMovie(const QString& moviePath) {
    return Util::getPathToAsset(QString("assets/movies/%1").arg(moviePath));
}

QString Util::getPathToLevel(const QString& levelPath) {
    return Util::getPathToAsset(QString("assets/levels/%1").arg(levelPath));
}

QString Util::getPathToSound(const QString& soundPath) {
    return QString("assets/sounds/%1").arg(soundPath);
}

QString Util::getPathToData(const QString& dataPath) {
    return Util::getPathToAsset(QString("assets/data/%1").arg(dataPath));
}

QByteArray Util::readFileAsBytes(QString &filePath){
    QFile codeFile(filePath);
    codeFile.open(QFile::ReadOnly | QFile::Text);
    return codeFile.readAll();
}

QString Util::readFileAsQString(const QString& filePath) {
    QFile codeFile(filePath);
    codeFile.open(QFile::ReadOnly | QFile::Text);
    return QTextStream(&codeFile).readAll();
}

std::string Util::readFileAsStdString(const QString& filePath) {
    QFile codeFile(filePath);
    codeFile.open(QFile::ReadOnly | QFile::Text);
    return QTextStream(&codeFile).readAll().toStdString();
}

QRectF Util::united(const QRectF& rect, const QPointF& point) {
    QRectF united;
    united.setLeft(std::min(rect.left(), point.x()));
    united.setRight(std::max(rect.right(), point.x()));
    united.setBottom(std::min(rect.bottom(), point.y()));
    united.setTop(std::max(rect.top(), point.y()));
    return united;
}

// This is a work-around for a bug in QMetaObject in which a javascript
// function defined in an Item directly is not returned via QMetaObject::indexOfMethod();
bool Util::javaScriptFuncExists(QObject* object, const char* method) {
    const QMetaObject* m = object->metaObject();
    for (int i = 0 ; i < m->methodCount() ; i++ ) {
        if (m->method(i).name() == method) {
            return true;
        }
    }
    return false;
}

void Util::javaScriptExecuteFuncIfExists(QObject* object, const char* method) {
    const QMetaObject* m = object->metaObject();
    for (int i = 0 ; i < m->methodCount() ; i++ ) {
        if (m->method(i).name() == method) {
            QMetaObject::invokeMethod(object,m->method(i).name());
        }
    }
}

bool Util::nullOrEmpty(const QString& st){
    return (st.isNull() || st.isEmpty());
}

/**
* Find the name of the file based in the url.
* And if exists, it will add a number to differentiate it.
*/
QString Util::fileName(QString dirpath,QUrl url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(dirpath+basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(dirpath+basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }
    if(dirpath.isEmpty())
        dirpath = "./";

    return dirpath+basename;
}

/**
*   Checks if a file exists.
*/
bool Util::fileExists(QString& fname){
    QFile file(fname);
    return file.exists();
}

bool Util::deviceHasTouchScreen() {
    return getDeviceTouchScreenDeviceId() >= 0;
}

int Util::getDeviceTouchScreenDeviceId() {
#ifdef Q_OS_ANDROID
    return AndroidActivity::getTouchScreenDeviceId();
#else
    // TODO: Determine touch screen device id on other devices.
    return -1;
#endif
}

b2AABB Util::getBodyBoundingBox(b2Body* body)
{
    b2AABB aabb;
    b2Transform t;
    t.SetIdentity();
    aabb.lowerBound = b2Vec2(FLT_MAX,FLT_MAX);
    aabb.upperBound = b2Vec2(-FLT_MAX,-FLT_MAX);
    b2Fixture* fixture = body->GetFixtureList();
    while (fixture != nullptr) {
         const b2Shape *shape = fixture->GetShape();
         const int childCount = shape->GetChildCount();
         for (int child = 0; child < childCount; ++child) {
                const b2Vec2 r(shape->m_radius, shape->m_radius);
                b2AABB shapeAABB;
                shape->ComputeAABB(&shapeAABB, t, child);
                shapeAABB.lowerBound = shapeAABB.lowerBound + r;
                shapeAABB.upperBound = shapeAABB.upperBound - r;
                aabb.Combine(shapeAABB);
        }
        fixture = fixture->GetNext();
    }

    return aabb;
}
