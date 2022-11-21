QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

CONFIG += c++17
unix {

ANDROID_OPENCV = D:/opencv/opencv-android/OpenCV-android-sdk/sdk/native


INCLUDEPATH += $$ANDROID_OPENCV/jni/include/opencv2 \
               $$ANDROID_OPENCV/jni/include
LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_java4.so
LIBS += $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_calib3d.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_core.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_dnn.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_features2d.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_flann.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_gapi.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_highgui.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_imgcodecs.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_imgproc.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_ml.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_objdetect.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_photo.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_stitching.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_video.a \
        $$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_videoio.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libade.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libcpufeatures.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libIlmImf.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libittnotify.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibjpeg-turbo.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibopenjp2.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibpng.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibprotobuf.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibtiff.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibwebp.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libquirc.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libtbb.a \
        $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libtegra_hal.a \





}
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abcddialog.cpp \
    datatabel.cpp \
    imagelabel.cpp \
    main.cpp \
    mainwindow.cpp \
    plotwidget.cpp \
    rotateimagedialog.cpp \
    sampledialog.cpp \
    scrollareawidget.cpp \
    callout.cpp

HEADERS += \
    abcddialog.h \
    datatabel.h \
    imagelabel.h \
    mainwindow.h \
    plotwidget.h \
    rotateimagedialog.h \
    sampledialog.h \
    scrollareawidget.h \
    callout.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../../../../opencv/opencv-android/OpenCV-android-sdk/sdk/native/libs/armeabi-v7a/libopencv_java4.so

    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
RESOURCES += \
    res.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml
