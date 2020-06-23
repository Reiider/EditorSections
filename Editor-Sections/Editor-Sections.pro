#-------------------------------------------------
#
# Project created by QtCreator 2017-06-25T11:11:23
#
#-------------------------------------------------

QT       += core gui opengl sql axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0

TARGET = Editor-Sections
TEMPLATE = app

LIBS += -lopengl32 -lglu32

SOURCES += main.cpp\
        mainwindow.cpp \
    decisiontask.cpp \
    ogl.cpp \
    createtask.cpp \
    helpfunction.cpp \
    point.cpp \
    linear.cpp \
    flat.cpp \
    pyramid.cpp \
    cube.cpp \
    prism.cpp \
    arraystep.cpp \
    possiblestep.cpp \
    digestogl.cpp \
    paintlogo.cpp \
    createtest.cpp \
    adminpanel.cpp \
    statistic.cpp \
    viewtasks.cpp \
    createdemo.cpp \
    authorization.cpp \
    buttonfortest.cpp \
    filemanager.cpp \
    listtaskslog.cpp \
    database.cpp \
    layouttasktype.cpp \
    test.cpp \
    testvariant.cpp \
    testvarianttype.cpp \
    testvarianttypetask.cpp \
    demotask.cpp \
    demovariant.cpp \
    buttonblockaction.cpp \
    writelog.cpp \
    windowdialogs.cpp \
    constructing.cpp \
    colorflat.cpp \
    graphicanswer.cpp \
    replicate.cpp \
    replicatetasktocopy.cpp

HEADERS  += mainwindow.h \
    decisiontask.h \
    ogl.h \
    createtask.h \
    helpfunction.h \
    point.h \
    linear.h \
    flat.h \
    pyramid.h \
    cube.h \
    prism.h \
    arraystep.h \
    possiblestep.h \
    digestogl.h \
    paintlogo.h \
    createtest.h \
    adminpanel.h \
    statistic.h \
    viewtasks.h \
    createdemo.h \
    authorization.h \
    buttonfortest.h \
    filemanager.h \
    listtaskslog.h \
    database.h \
    layouttasktype.h \
    test.h \
    testvariant.h \
    testvarianttype.h \
    testvarianttypetask.h \
    demotask.h \
    demovariant.h \
    buttonblockaction.h \
    writelog.h \
    windowdialogs.h \
    constructing.h \
    glfw3.h \
    colorflat.h \
    graphicanswer.h \
    replicate.h \
    replicatetasktocopy.h

FORMS    += mainwindow.ui \
    decisiontask.ui \
    createtask.ui \
    createtest.ui \
    adminpanel.ui \
    statistic.ui \
    viewtasks.ui \
    createdemo.ui \
    authorization.ui \
    demotask.ui \
    demovariant.ui \
    windowdialogs.ui \
    constructing.ui \
    replicate.ui \
    replicatetasktocopy.ui

RC_ICONS = icon.ico

