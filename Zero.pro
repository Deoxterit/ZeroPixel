QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SurfaceBox.cpp \
    SurfaceBoxRenderer.cpp \
    SurfaceBoxRendererList.cpp \
    main.cpp

HEADERS += \
    BPixelOp.h \
    Bgra.h \
    Bitmap.h \
    PixelBlendOps.h \
    PixelOp.h \
    ScaleFactor.h \
    Surface.h \
    SurfaceBox.h \
    SurfaceBoxBaseRenderer.h \
    SurfaceBoxRenderer.h \
    SurfaceBoxRendererList.h \
    UPixelOp.h \
    UPixelOps.h \
    bmpFileIO.h
LIBS += libopengl32 libglu32
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
