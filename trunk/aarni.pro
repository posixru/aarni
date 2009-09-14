# -------------------------------------------------
# Project created by QtCreator 2009-09-03T09:20:06
# -------------------------------------------------
TARGET = aarni
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    message-digest.cpp \
    ripemd160.cpp \
    cipher.cpp \
    arc4.cpp \
    main-dialog-view.cpp \
    encryptor.cpp \
    aes.cpp
HEADERS += message-digest.h \
    const.h \
    ripemd160.h \
    cipher.h \
    arc4.h \
    main-dialog-view.h \
    encryptor.h \
    aes.h
RESOURCES += resources.qrc
TRANSLATIONS += trans/lang_en.ts \
    trans/lang_zh.ts
