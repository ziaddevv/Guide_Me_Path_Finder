QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += include

SOURCES += \
    src/filehandler.cpp \
    src/graph.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    include/filehandler.hpp \
    include/graph.hpp \
    include/mainwindow.h


FORMS += \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
