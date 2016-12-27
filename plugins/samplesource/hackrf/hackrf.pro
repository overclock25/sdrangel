#--------------------------------------------------------
#
# Pro file for Android and Windows builds with Qt Creator
#
#--------------------------------------------------------

TEMPLATE = lib
CONFIG += plugin

QT += core gui widgets multimedia opengl

TARGET = inputhackrf

DEFINES += USE_SSE2=1
QMAKE_CXXFLAGS += -msse2
DEFINES += USE_SSE4_1=1
QMAKE_CXXFLAGS += -msse4.1

CONFIG(MINGW32):LIBHACKRFSRC = "D:\softs\hackrf\host"
CONFIG(MINGW64):LIBHACKRFSRC = "D:\softs\hackrf\host"
INCLUDEPATH += $$PWD
INCLUDEPATH += ../../../sdrbase
INCLUDEPATH += $$LIBHACKRFSRC

CONFIG(Release):build_subdir = release
CONFIG(Debug):build_subdir = debug

SOURCES += hackrfinputgui.cpp\
  hackrfinput.cpp\
  hackrfinputplugin.cpp\
  hackrfinputsettings.cpp\
  hackrfinputthread.cpp

HEADERS += hackrfinputgui.h\
  hackrfinput.h\
  hackrfinputplugin.h\
  hackrfinputsettings.h\
  hackrfinputthread.h

FORMS += hackrfinputgui.ui

LIBS += -L../../../sdrbase/$${build_subdir} -lsdrbase
LIBS += -L../../../libhackrf/$${build_subdir} -llibhackrf

RESOURCES = ../../../sdrbase/resources/res.qrc
