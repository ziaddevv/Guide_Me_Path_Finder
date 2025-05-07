QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += include

SOURCES += \
    src/program.cpp \
    src/city.cpp \
    src/cityexplorer.cpp \
    src/filehandler.cpp \
    src/graph.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    include/program.hpp \
    include/city.hpp \
    include/cityexplorer.h \
    include/filehandler.hpp \
    include/graph.hpp \
    include/graphviewitems.hpp \
    include/mainwindow.h

FORMS += \
    ui/cityexplorer.ui \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
