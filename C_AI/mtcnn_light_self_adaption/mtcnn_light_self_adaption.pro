TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ./OpenBLAS/include
INCLUDEPATH += D:/develop/tools/opencv343/build/include
               D:/develop/tools/opencv343/build/include/opencv
               D:/develop/tools/opencv343/build/include/opencv2
               D:/develop/tools/opencv343/build/x64/vc14/bin
#CONFIG(debug, debug|release): {
#LIBS += -LD:/develop/workstations/GitHub/ \
#-llibopenblas \
#LIBS += -LD:/develop/tools/opencv343/build/x64/vc14/lib \
#-lopencv_world343d \

#} else:CONFIG(release, debug|release): {
#LIBS += -LD:/develop/workstations/GitHub/OpenBLAS-v0.2.8-x86_64-Win/lib \
#-llibopenblas \
#LIBS += -LD:/develop/tools/opencv343/build/x64/vc14/lib \
#-lopencv_world343 \
#}
CONFIG(debug, debug|release): {
LIBS += ./OpenBLAS/lib/libopenblas.lib \
        D:/develop/tools/opencv343/build/x64/vc14/lib/opencv_world343d.lib \

} else:CONFIG(release, debug|release): {
LIBS += ./OpenBLAS/lib/libopenblas.lib \
        D:/develop/tools/opencv343/build/x64/vc14/lib/opencv_world343.lib \
}
#CONFIG(debug, debug|release): {
#LIBS += D:/develop/workstations/GitHub/OpenBLAS/build/lib/DEBUG/openblas.lib \
#        D:/develop/tools/opencv343/build/x64/vc14/lib/opencv_world343d.lib \

#} else:CONFIG(release, debug|release): {
#LIBS += D:/develop/workstations/GitHub/OpenBLAS/build/lib/RELEASE/openblas.lib \
#        D:/develop/tools/opencv343/build/x64/vc14/lib/opencv_world343.lib \
#}

SOURCES += \
    src/main.cpp \
    src/mtcnn.cpp \
    src/network.cpp \
    src/pBox.cpp

DISTFILES += \
    images_adaption/1.jpg \
    images_adaption/2.jpg \
    images_adaption/3.jpg \
    models/Onet.txt \
    models/Pnet.txt \
    models/Rnet.txt

HEADERS += \
    src/mtcnn.h \
    src/network.h \
    src/pBox.h
