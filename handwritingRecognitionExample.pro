QT += quick qml
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/main.cpp \
    src/writingrecognizer.cpp

HEADERS += \
    include/writingrecognizer.h

RESOURCES += \
    content/qml.qrc

INCLUDEPATH += $$PWD/include

win32: LIBS += -L$$PWD/lib/win32 -llibdhwr
else:unix: LIBS += -L$$PWD/lib/linux -ldhwr -ldhwr-base -ldhwr-core

win32:DEFINES += "RESOURCE_PATH=\\\"$$PWD/hdb\\\"" \
    "LICENSE_PATH=\\\"$$PWD/license/license.key\\\"" \
    "LIBRARY_PATH=\\\"$$PWD/lib/win32\\\""
else:unix:DEFINES += "RESOURCE_PATH=\\\"$$PWD/hdb\\\"" \
    "LICENSE_PATH=\\\"$$PWD/license/license.key\\\"" \
    "LIBRARY_PATH=\\\"$$PWD/lib/linux\\\""
