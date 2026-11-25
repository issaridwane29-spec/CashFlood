QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    budget.cpp \
    budgetdao.cpp \
    categorie.cpp \
    categoriedao.cpp \
    compte.cpp \
    comptedao.cpp \
    compteservice.cpp \
    databasemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    operation.cpp \
    operationdao.cpp \
    comptedialog.cpp \
    comptewidget.cpp \
    transactiondialog.cpp \
    transactionwidget.cpp
    validateur.cpp

HEADERS += \
    budget.h \
    budgetdao.h \
    categorie.h \
    categoriedao.h \
    compte.h \
    comptedao.h \
    compteservice.h \
    databasemanager.h \
    mainwindow.h \
    operation.h \
    operationdao.h \
    comptedialog.h \
    comptewidget.h \
    transactiondialog.h \
    transactionwidget.h
    validateur.h

FORMS += \
    mainwindow.ui \
    comptewidget.ui \
    comptedialog.ui \
    transactiondialog.ui \
    transactionwidget.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    schema.sql

RESOURCES += \
    Ressources.qrc
