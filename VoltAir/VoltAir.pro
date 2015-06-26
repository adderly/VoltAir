#
# Copyright (C) 2014 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Add more folders to ship with the application, here
# TODO: This manner of handling deployment folders needs some love. Sub-projects that want to deploy
# folders must make sure their var names don't collide with these.
TEMPLATE=app
assets_folder.source = assets
assets_folder.target = .
qml_folder.source = qml
qml_folder.target = .
DEPLOYMENTFOLDERS += assets_folder qml_folder

INCLUDEPATH += $$PWD \
    $$PWD/src   \
    $$PWD/../VoltEngine

DEFINES += BOX2D_LOADER
DEFINES += QML_BOX2D
DEFINES += LIQUIDFUN_EXTERNAL_LANGUAGE_API
DEFINES += EXT_COMPONENTS
DEFINES += EXT_MODELS

HEADERS += \
    Doxygen.h \

SOURCES += \
    main.cpp \

INCLUDEPATH += ../third_party/liquidfun/liquidfun/Box2D/
INCLUDEPATH += ../third_party/liquidfun/liquidfun/Box2D/Box2D

include(VoltAir.pri)

OTHER_FILES += \
    ../VoltEngine/Engine/android/deploy/src/com/google/fpl/utils/* \
    ../VoltEngine/Engine/android/deploy/src/com/google/fpl/voltair/* \
    ../VoltEngine/Engine/android/deploy/res/values/*


DISTFILES += \
    ../VoltEngine/Engine/android/deploy/gradle/wrapper/gradle-wrapper.jar \
    ../VoltEngine/Engine/android/deploy/AndroidManifest.xml \
    ../VoltEngine/Engine/android/deploy/gradlew.bat \
    ../VoltEngine/Engine/android/deploy/res/values/libs.xml \
    ../VoltEngine/Engine/android/deploy/build.gradle \
    ../VoltEngine/Engine/android/deploy/gradle/wrapper/gradle-wrapper.properties \
    ../VoltEngine/Engine/android/deploy/gradlew


