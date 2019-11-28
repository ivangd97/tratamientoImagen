FORMS += mainForm.ui 
 
HEADERS += pracaoc.h \
           imageprocess.h 
           
SOURCES += pracaoc.cpp \
           main.cpp \
           imageprocess.cpp 
           
TEMPLATE = app

CONFIG += release \
warn_on \
thread \
qt

QMAKE_CFLAGS  += -m32 -fno-omit-frame-pointer
QMAKE_CXXFLAGS  += -m32 -fno-omit-frame-pointer
QMAKE_LFLAGS  += -m32
QMAKE_LIBDIR  += /usr/lib/i386-linux-gnu  /usr/lib32


TARGET = pracAOC
