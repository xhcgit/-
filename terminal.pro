QT += widgets serialport sql gui

TARGET = terminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    ShowData.cpp \
    SettingsDialog.cpp \
    Console.cpp \
    MainWindow.cpp

HEADERS += \
    ShowData.h \
    SettingsDialog.h \
    Console.h \
    MainWindow.h

FORMS += \
    ShowData.ui \
    MainWindow.ui \
    SettingsDialog.ui

RESOURCES += \
    terminal.qrc
