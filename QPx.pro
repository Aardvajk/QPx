QT += core gui widgets xml

TARGET = QPx
TEMPLATE = lib
CONFIG += staticlib

DEFINES -= UNICODE

QMAKE_CXXFLAGS += -fexceptions -frtti -fno-strict-aliasing \
                  -DQT_NO_CAST_TO_ASCII \
                  -fcheck-new -DNOMINMAX -std=gnu++0x -m64

QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas -Wno-comment -Wno-maybe-uninitialized -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-attributes

QMAKE_LFLAGS += -fexceptions -frtti -fno-builtin -fno-strict-aliasing \
                -static-libgcc -static -Wl,-enable-auto-import \
                -Wl,-enable-runtime-pseudo-reloc -Wl,--as-needed \
                -Wl,-enable-stdcall-fixup -Wl,-allow-multiple-definition -Wl,-enable-auto-import -m64

INCLUDEPATH += "C:/mingw64/x86_64-w64-mingw32.shared/include" \
               "C:/mingw64/x86_64-w64-mingw32.shared/qt5/include" \
               "C:/Projects/pcx"

LIBS += "C:/Projects/pcx/build-pcx/release/libpcx.a" \
        "C:/mingw64/x86_64-w64-mingw32.shared/lib/libuser32.a" \
        "C:/mingw64/x86_64-w64-mingw32.shared/lib/libgdi32.a" \
        "C:/mingw64/x86_64-w64-mingw32.shared/lib/libpsapi.a"

PRE_TARGETDEPS += "C:/Projects/pcx/build-pcx/release/libpcx.a"

SOURCES += \
    QPxWidgets/QPxAbstractTileWidget.cpp

HEADERS += \
    QPxWidgets/QPxAbstractTileWidget.h
