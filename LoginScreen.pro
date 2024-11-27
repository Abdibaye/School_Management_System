QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calender.cpp \
    dash_board.cpp \
    generate.cpp \
    main.cpp \
    login_screen.cpp \
    manu_screen.cpp \
    pay.cpp \
    payhistory.cpp \
    payment.cpp \
    registers_creen.cpp \
    roster.cpp \
    time_table.cpp

HEADERS += \
    calender.h \
    dash_board.h \
    generate.h \
    login_screen.h \
    manu_screen.h \
    pay.h \
    payhistory.h \
    payment.h \
    register_screen.h \
    roster.h \
    time_table.h

FORMS += \
    calender.ui \
    dashboard.ui \
    generate.ui \
    loginscreen.ui \
    manuscreen.ui \
    pay.ui \
    payhistory.ui \
    payment.ui \
    registerscreen.ui \
    roster.ui \
    timetable.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    build/background.qrc
