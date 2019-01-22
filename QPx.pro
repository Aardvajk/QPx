QT += core gui widgets xml

TARGET = QPx
TEMPLATE = lib
CONFIG += staticlib

DEFINES -= UNICODE

QMAKE_CXXFLAGS += -fexceptions -frtti -fno-strict-aliasing \
                  -DQT_NO_CAST_TO_ASCII \
                  -fcheck-new -DNOMINMAX -std=gnu++0x -m64 \
                  -include "QtCore/QDebug"

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
    QPxEditor/QPxAbstractEditorCommand.cpp \
    QPxEditor/QPxAbstractEditorModel.cpp \
    internal/qpx_undolist.cpp \
    QPxWidgets/QPxKeySequenceEdit.cpp \
    QPxActions/QPxAction.cpp \
    QPxActions/QPxActionList.cpp \
    QPxEditor/QPxEditorFileActions.cpp \
    QPxWidgets/QPxIconLabel.cpp \
    internal/qpx_settings_parser.cpp \
    QPxCore/QPxSettings.cpp \
    QPxWidgets/QPxKeyboardOptionsWidget.cpp \
    QPxWidgets/QPxMainWindow.cpp \
    QPxCore/QPxTreeModel.cpp \
    QPxPropertyBrowser/QPxPropertyBrowserDelegate.cpp \
    QPxPropertyBrowser/QPxPropertyBrowserDialog.cpp \
    QPxPropertyBrowser/QPxPropertyBrowserEditor.cpp \
    QPxPropertyBrowser/QPxPropertyBrowserItem.cpp \
    QPxPropertyBrowser/QPxPropertyBrowserModel.cpp \
    QPxPropertyBrowser/QPxPropertyBrowserType.cpp \
    internal/qpx_combo_box.cpp \
    QPxWidgets/QPxLayouts.cpp \
    QPxWidgets/QPxRichTextComboBox.cpp \
    QPxCore/QPxDeleter.cpp \
    QPxWidgets/QPxSplitterPanel.cpp \
    QPxWidgets/QPxLineSplitter.cpp \
    QPxWidgets/QPxPalette.cpp \   
    QPxCore/QPxUnitAnimation.cpp \
    QPxWidgets/QPxValueButton.cpp

HEADERS += \
    QPxEditor/QPxAbstractEditorCommand.h \
    QPxEditor/QPxAbstractEditorModel.h \
    internal/qpx_undolist.h \
    QPxWidgets/QPxKeySequenceEdit.h \
    QPxActions/QPxAction.h \
    QPxActions/QPxActionList.h \
    QPxEditor/QPxEditorFileActions.h \
    QPxWidgets/QPxIconLabel.h \
    QPxCore/QPxSettings.h \
    QPxWidgets/QPxKeyboardOptionsWidget.h \
    QPxWidgets/QPxMainWindow.h \
    internal/qpx_settings_parser.h \
    QPxCore/QPxTreeModel.h \
    QPxPropertyBrowser/QPxPropertyBrowserDelegate.h \
    QPxPropertyBrowser/QPxPropertyBrowserDialog.h \
    QPxPropertyBrowser/QPxPropertyBrowserEditor.h \
    QPxPropertyBrowser/QPxPropertyBrowserItem.h \
    QPxPropertyBrowser/QPxPropertyBrowserModel.h \
    QPxPropertyBrowser/QPxPropertyBrowserType.h \
    internal/qpx_combo_box.h \
    QPxCore/QPxStdHash.h \
    QPxWidgets/QPxLayouts.h \
    QPxWidgets/QPxRichTextComboBox.h \
    QPxCore/QPxDeleter.h \
    QPxWidgets/QPxSplitterPanel.h \
    QPxWidgets/QPxLineSplitter.h \
    QPxWidgets/QPxPalette.h \   
    QPxCore/QPxUnitAnimation.h \
    QPxWidgets/QPxValueButton.h \
    QPxCore/QPxStableList.h \
    QPxCore/QPxMetaType.h
