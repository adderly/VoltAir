INCLUDEPATH += $$PWD/../liquidfun/freeglut/include/GL/
INCLUDEPATH += $$PWD/../liquidfun/freeglut/include/
INCLUDEPATH += $$PWD


android{
    DEFINES += FREEGLUT_GLES2
#    DEFINES += RUBE_DRAW_IMAGE
}

HEADERS += \
    $$PWD/b2dJson.h \
    $$PWD/b2dJsonImage.h \
    $$PWD/b2dJsonImage_OpenGL.h \
    $$PWD/bitmap.h \
    $$PWD/json/json.h \
    $$PWD/json/json-forwards.h

SOURCES += \
    $$PWD/b2dJson.cpp \
    $$PWD/b2dJsonImage.cpp \
    $$PWD/b2dJsonImage_OpenGL.cpp \
    $$PWD/jsoncpp.cpp
