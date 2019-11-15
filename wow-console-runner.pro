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
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    graphics.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
