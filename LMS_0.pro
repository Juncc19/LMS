QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    accounttablemodel.cpp \
    addnewdialog.cpp \
    admininterface.cpp \
    bookadminwidget.cpp \
    booktablemodel.cpp \
    borrowreturntab.cpp \
    filterdialog.cpp \
    loginwidget.cpp \
    main.cpp \
    newreadertab.cpp \
    overtimeinspection.cpp \
    overtimerecordtab.cpp \
    passworddialog.cpp \
    readerinfotab.cpp \
    readermanagementmodel.cpp \
    readermanagementwidget.cpp \
    recordtablemodel.cpp \
    registerwidget.cpp \
    studentinterface.cpp \
    unitfilter.cpp \
    userinterface.cpp

HEADERS += \
    accounttablemodel.h \
    addnewdialog.h \
    admininterface.h \
    bookadminwidget.h \
    booktablemodel.h \
    borrowreturntab.h \
    constants.h \
    filterdialog.h \
    initdb.h \
    loginwidget.h \
    newreadertab.h \
    overtimeinspection.h \
    overtimerecordtab.h \
    passworddialog.h \
    readerinfotab.h \
    readermanagementmodel.h \
    readermanagementwidget.h \
    recordtablemodel.h \
    registerwidget.h \
    studentinterface.h \
    unitfilter.h \
    userinterface.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

FORMS += \
    studentinterface.ui
