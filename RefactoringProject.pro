#-------------------------------------------------
#
# Project created by QtCreator 2019-07-17T10:29:33
#
#-------------------------------------------------

QT       += core gui testlib
LIBS += -lopengl32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RefactoringProject
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

include(Octree/Octree.pri)

SOURCES += \
        arcball.cpp \
        camera3d.cpp \
        loadobj.cpp \
        main.cpp \
        model3d.cpp \
        navlog.cpp \
        scene.cpp \
        tests.cpp \
        widget.cpp

HEADERS += \
        arcball.h \
        camera3d.h \
        loadobj.h \
        model3d.h \
        navlog.h \
        scene.h \
        tests.h \
        timer.h \
        vertexdata.h \
        widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    models.qrc \
    shaders.qrc \
    textures.qrc

DISTFILES +=
