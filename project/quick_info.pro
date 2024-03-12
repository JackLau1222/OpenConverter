QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    core.h \
    widget.h

FORMS += \
    widget.ui

# LIBS += -L/usr/local/lib -lavformat

LIBS += -L/usr/local/lib -L/opt/homebrew/Cellar/libx11/1.8.7/lib -lavformat -lm -lbz2 -lz -Wl,-framework,CoreFoundation -Wl,-framework,Security -lavcodec -liconv -lm -framework AudioToolbox -pthread -lz -framework VideoToolbox -framework CoreFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices -lswresample -lm -lavutil -pthread -lm -framework VideoToolbox -lX11 -framework CoreFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices

INCLUDEPATH += /usr/local/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
