QT += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    CONFIG += c++11                 #C++11 for Qt5
} else {
    QMAKE_CXXFLAGS += -std=c++11    #C++11 for Qt4.8 and earlier
    #QMAKE_CXXFLAGS += -std=c++0x   #if you have gcc4.6 (comment previous line)
}

#if you have mac os x 10.6 or earlier then either use gcc4.7 or don't use c++11 features
macx {
    QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -stdlib=libc++
    QMAKE_LFLAGS += -mmacosx-version-min=10.7 -stdlib=libc++
}

