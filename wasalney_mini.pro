QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += include

SOURCES += \
     src/mainform.cpp \
    src/editgraph.cpp \
    src/program.cpp \
    src/city.cpp \
    src/filehandler.cpp \
    src/graph.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/exploremap.cpp

HEADERS += \
    include/program.hpp \
    include/city.hpp \
    include/filehandler.hpp \
    include/graph.hpp \
    include/graphviewitems.hpp \
    include/mainwindow.h \
    include/exploremap.h \
    include/mainform.h \
    include/editgraph.h

FORMS += \
    ui/mainform.ui \
    ui/editgraph.ui \
    ui/exploremap.ui \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
