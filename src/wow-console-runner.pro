QT += \
    core gui widgets

CONFIG += \
    c++17

# This should run windeployqt after build, but it doesn't work
CONFIG(release) {
    CONFIG += windeployqt
}

# Warns if you use any Qt feature that has been marked deprecated
# (the exact warnings are compiler-dependent).
DEFINES += QT_DEPRECATED_WARNINGS

RC_ICONS = rc/icon.ico

SOURCES += \
    tabconsole.cpp \
    utils.cpp \
    main.cpp \
    console.cpp \
    mainwindow.cpp

HEADERS += \
    tabconsole.h \
    utils.h \
    console.h \
    mainwindow.h

FORMS += \
    mainwindow.ui \
    tabconsole.ui

RESOURCES += \
    fonts.qrc \
    graphics.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
