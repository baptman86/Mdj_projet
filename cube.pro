QT      += \
        core gui widgets \
        3drender

TARGET = OpenGl_intro_LEMOINE
TEMPLATE = app

SOURCES += main.cpp \
    object.cpp \
    obstacle.cpp \
    character.cpp \
    mapgrid.cpp \
    weapon.cpp \
    rifle.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp \
    mesh.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    mesh.h \
    object.h \
    obstacle.h \
    character.h \
    mapgrid.h \
    weapon.h \
    astar.h \
    rifle.h

RESOURCES += \
    shaders.qrc \
    textures.qrc \
    meshs.qrc

LIBS += \
    -lopengl32

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
