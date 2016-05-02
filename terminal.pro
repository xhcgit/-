QT += widgets serialport sql gui

TARGET = terminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    ShowData.cpp \
    SettingsDialog.cpp \
    Console.cpp \
    MainWindow.cpp \
    FindData.cpp \
    PaintData.cpp

HEADERS += \
    ShowData.h \
    SettingsDialog.h \
    Console.h \
    MainWindow.h \
    FindData.h \
    PaintData.h

FORMS += \
    ShowData.ui \
    SettingsDialog.ui \
    MainWindow.ui \
    FindData.ui \
    PaintData.ui

RESOURCES += \
    terminal.qrc

DEFINES += QT_DLL QWT_DLL
LIBS += -L"C:\Qt\Qt5.6.0\5.6\mingw49_32\lib" -lqwtd
LIBS += -L"C:\Qt\Qt5.6.0\5.6\mingw49_32\lib" -lqwt
INCLUDEPATH += C:\Qt\Qt5.6.0\5.6\mingw49_32\include\Qwt

RC_FILE = Alcohol.rc
