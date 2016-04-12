QT += widgets serialport sql gui

TARGET = terminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    ShowData.cpp \
    SettingsDialog.cpp \
    Console.cpp \
    MainWindow.cpp \
    FindData.cpp

HEADERS += \
    ShowData.h \
    SettingsDialog.h \
    Console.h \
    MainWindow.h \
    FindData.h

FORMS += \
    ShowData.ui \
    SettingsDialog.ui \
    MainWindow.ui \
    FindData.ui

RESOURCES += \
    terminal.qrc
