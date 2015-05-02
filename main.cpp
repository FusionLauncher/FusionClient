#include "mainwindow.h"
#include <QApplication>

#ifdef _WIN32
    #define OS Windows
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
        #define OS Mac
    #endif

#elif __linux
    #define OS Linux
#elif __unix // all unices not caught above
    #define OS Unix
#elif __posix
    #define OS POSIX
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
