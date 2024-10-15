QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    converter.cpp \
    encode_parameter.cpp \
    encode_setting.cpp \
    info.cpp \
    main.cpp \
    process_parameter.cpp \
    stream_context.cpp \
    transcoder.cpp \
    widget.cpp

HEADERS += \
    converter.h \
    encode_parameter.h \
    encode_setting.h \
    info.h \
    process_parameter.h \
    stream_context.h \
    transcoder.h \
    widget.h

FORMS += \
    encode_setting.ui \
    widget.ui


LIBS += -L/usr/local/lib -L/opt/homebrew/Cellar/x264/r3108/lib -L/opt/homebrew/Cellar/x265/3.6/lib -L/usr/local/lib -L/opt/homebrew/Cellar/libx11/1.8.9/lib -lavformat -lavcodec -lavutil -lm -lbz2 -lz

INCLUDEPATH += /usr/local/include

INCLUDEPATH +=/opt/homebrew/Cellar/x264/r3108/include

INCLUDEPATH +=/opt/homebrew/Cellar/x265/3.5/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
