!OCTREE_PRI {

CONFIG += OCTREE_PRI

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/octreenode.h \
    $$PWD/octree.h \
    $$PWD/helperfunctions.h \
    $$PWD/triangle3d.h \
    $$PWD/Ray.h
SOURCES += \
    $$PWD/octreenode.cpp \
    $$PWD/octree.cpp \
    $$PWD/helperfunctions.cpp \
    $$PWD/triangle3d.cpp
}
