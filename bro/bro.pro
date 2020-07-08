TEMPLATE = app
TARGET = bro
DESTDIR = bin
QT += webenginewidgets
QT -= network

greaterThan(QT_MAJOR_VERSION, 4) {
    DEFINES += HAVE_QT5
}

BUILDNUM = 0

#message($$BUILDNUM)
VERSION = 1.0.$$BUILDNUM
DEFINES += VERSION_STRING=\\\"$${VERSION}\\\"

#CONFIG += c++11 resources_big

HEADERS += \
    browser.h \
    browserwindow.h \
    searchwidget.h \
    tabwidget.h \
    webpage.h \
    webpopupwindow.h \
    webview.h \
    treeop.h

SOURCES += \
    browser.cpp \
    browserwindow.cpp \
    main.cpp \
    searchwidget.cpp \
    tabwidget.cpp \
    webpage.cpp \
    webpopupwindow.cpp \
    webview.cpp \
    treeop.cpp

FORMS += \
    passworddialog.ui \
    about.ui \
    ../catdir/clist.ui

RESOURCES += \
    bro.qrc

HEADERS += \
        ../libengines/libengines.h
DEPENDPATH += . ../libengines
INCLUDEPATH += ../libengines
LIBS += -L../libengines
CONFIG(debug, debug|release):LIBS += -L../libengines/debug
CONFIG(release, debug|release):LIBS += -L../libengines/release
LIBS += -llibengines

win32 {
    RC_FILE = version.rc
    QT += texttospeech
}

CODECFORSRC = UTF-8

CONFIG(debug, debug|release):CONFIG += console #no print
