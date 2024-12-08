QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Coin.cpp \
    Enemy.cpp \
    GameScene.cpp \
    GameWindow.cpp \
    Heart.cpp \
    Level5.cpp \
    Player.cpp \
    WelcomeWindow.cpp \
    fallingRock.cpp \
    flyingenemy.cpp \
    level2.cpp \
    level3.cpp \
    level4.cpp \
    main.cpp \
    weapon.cpp

HEADERS += \
    Coin.h \
    Enemy.h \
    GameScene.h \
    GameWindow.h \
    Heart.h \
    Level5.h \
    Player.h \
    WelcomeWindow.h \
    fallingRock.h \
    flyingenemy.h \
    level2.h \
    level3.h \
    level4.h \
    weapon.h

FORMS += \
    GameWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES += \
    flyingenemy.png \
    heart.png \
    mario.png \
    rock.png \
    rocknew.png
