QT      += \
        core gui widgets \
        3drender

TARGET = OpenGl_intro_LEMOINE
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp \
    mesh.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    mesh.h

RESOURCES += \
    shaders.qrc \
    textures.qrc \
    meshs.qrc

LIBS += \
    -lopengl32

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
