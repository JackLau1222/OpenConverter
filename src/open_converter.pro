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
    open_converter.cpp \
    process_parameter.cpp \
    stream_context.cpp \
    transcoder.cpp \
    transcoder_bmf.cpp

HEADERS += \
    converter.h \
    encode_parameter.h \
    encode_setting.h \
    info.h \
    open_converter.h \
    process_parameter.h \
    stream_context.h \
    transcoder.h \
    transcoder_bmf.h

FORMS += \
    encode_setting.ui \
    open_converter.ui


BMF_ROOT_PATH=/Users/jacklau/Documents/Programs/Git/Github/bmf

LIBS += -L/usr/local/lib -L/opt/homebrew/Cellar/x264/r3108/lib -L/opt/homebrew/Cellar/x265/4.0_1/lib -L/usr/local/lib -L/opt/homebrew/Cellar/libx11/1.8.9/lib -lavformat -lavcodec -lavutil -lm -lbz2 -lz -Wl,-framework,CoreFoundation -Wl,-framework,Security -lavcodec -liconv -lm -framework AudioToolbox -lx264 -lx265 -pthread -lz -framework VideoToolbox -framework CoreFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices -lswresample -lm -lavutil -pthread -lm -framework VideoToolbox -framework CoreFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices

LIBS += -L/Users/jacklau/Documents/Programs/Git/Github/bmf/output/bmf/lib -lengine -lbmf_module_sdk -lhmp -lbuiltin_modules

INCLUDEPATH += /usr/local/include

INCLUDEPATH += /opt/homebrew/Cellar/x264/r3108/include

INCLUDEPATH += /opt/homebrew/Cellar/x265/4.0_1/include

INCLUDEPATH += /Users/jacklau/Documents/Programs/Git/Github/bmf/output/bmf/include

TRANSLATIONS += lang_chinese.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    lang.qrc
