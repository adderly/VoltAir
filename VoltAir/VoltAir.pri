## this is going to be the include file for posible games.
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


defined ( LIQUIDFUN ) {
    LIQUIDFUN_DIR = $$(LIQUIDFUN)
} else {
    # TODO: change to "liquidfun" for release.
    LIQUIDFUN_DIR = ../third_party/liquidfun/liquidfun
}

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH +=

CONFIG += c++11
QT += multimedia qml quick widgets gui
DEFINES += GL_GLEXT_PROTOTYPES


INCLUDEPATH += . \
        $$LIQUIDFUN_DIR/Box2D \
        $$PWD \
        $$PWD/GameNodes \
        $$PWD/Engine/utils \

include(../VoltEngine/Engine/Engine.pri)
include(GameInput/GameInput.pri)
include(renderer/Renderer.pri)
include(GameNodes/GameNodes.pri)

contains(DEFINES,EXT_COMPONENTS) {
    include(../third_party/components/components.pri)
}

contains(DEFINES,BOX2D_LOADER) {
    include(../third_party/rubestuff/rube.pri)
}

message("DEFINES = "$$DEFINES)
include(deployment.pri)

RESOURCES += \








