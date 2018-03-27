
#Qt version check
!contains(QT_VERSION, ^5\\.*\\..*) {
    message("Cannot build QtrainSim with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.0.")
}

# Compile without link to sound when loco have an accident
# CONFIG += NOSOUND

#Use maquettes instead of sim
#CONFIG += MAQUETTE

#Use cmain instead of cppmain
#CONFIG += CDEVELOP

#Use alternate painting style
#DEFINES += DRAW_BOUNDINGRECT

#Qt modules
QT += core gui widgets printsupport

!NOSOUND : QT += multimedia
!NOSOUND : DEFINES += WITHSOUND

#Target and template
TARGET = QtrainSim
TEMPLATE = app
DESTDIR = dist

#Configuration
CONFIG += debug


#Install the maquettes files
maquettes.path  =  $$OUT_PWD/dist/data/Maquettes
maquettes.files =  data/Maquettes/*
INSTALLS    += maquettes

#Install the file containing the infos about tracks
infos.path  =  $$OUT_PWD/dist/data
infos.files =  data/infosVoies.txt
INSTALLS    += infos

MAQUETTE: DEFINES += MAQUETTE
MAQUETTE: LIBS += -lmarklin

RESOURCES += qtrainsim.qrc

win32 {
    DEFINES += ON_WINDOWS
    UI_DIR = tmp/win/ui
    MOC_DIR = tmp/win/moc
    OBJECTS_DIR = tmp/win/obj
    RCC_DIR = tmp/win/rcc
    CDEVELOP: LIBS += -lpthread
}
unix {
    DEFINES += ON_LINUX
    UI_DIR = tmp/linux/ui
    MOC_DIR = tmp/linux/moc
    OBJECTS_DIR = tmp/linux/obj
    RCC_DIR = tmp/linux/rcc
}
macx {
    DEFINES += ON_MACOS
    UI_DIR = tmp/mac/ui
    MOC_DIR = tmp/mac/moc
    OBJECTS_DIR = tmp/mac/obj
    RCC_DIR = tmp/mac/rcc
    # Avoid to create a bundle that causes troubles with the maquette files
    CONFIG -= app_bundle
}

CDEVELOP: DEFINES += CDEVELOP
CDEVELOP: SOURCES += src/student_c/cmain.c
!CDEVELOP: SOURCES += src/student_cpp/cppmain.cpp

INCLUDEPATH += \
    src \
    src/student_c \
    src/student_cpp

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/voie.cpp \
    src/voiedroite.cpp \
    src/voiecourbe.cpp \
    src/voieaiguillage.cpp \
    src/voiecroisement.cpp \
    src/voievariable.cpp \
    src/voiebuttoir.cpp \
    src/voietraverseejonction.cpp \
    src/simview.cpp \
    src/commandetrain.cpp \
    src/loco.cpp \
    src/contact.cpp \
    src/segment.cpp \
    src/trainsimsettings.cpp \
    src/maquettemanager.cpp \
    src/voieaiguillageenroule.cpp \
    src/voieaiguillagetriple.cpp \
    src/ctrain_handler.cpp \
    src/student_cpp/locomotive.cpp

HEADERS += \
    src/mainwindow.h \
    src/voie.h \
    src/voiedroite.h \
    src/voiecourbe.h \
    src/voieaiguillage.h \
    src/voiecroisement.h \
    src/voievariable.h \
    src/voiebuttoir.h \
    src/voietraverseejonction.h \
    src/simview.h \
    src/connect.h \
    src/commandetrain.h \
    src/general.h \
    src/loco.h \
    src/contact.h \
    src/segment.h \
    src/trainsimsettings.h \
    src/maquettemanager.h \
    src/voieaiguillageenroule.h \
    src/voieaiguillagetriple.h \
    src/ctrain_handler.h \
    src/student_cpp/locomotive.h

OTHER_FILES += infosVoies.txt
