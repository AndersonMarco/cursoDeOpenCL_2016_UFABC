#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QScreen>
#include <QThread>
#include <QTcpSocket>
#include "window.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*    
    QScreen *screen = QGuiApplication::primaryScreen();
    QImage img=screen->grabWindow(0).toImage();
    int width=img.width();
    int height=img.height();
    int pixelsLength=width*height*4;
    QTcpSocket *socket = new QTcpSocket();
    while(true){
        QImage img=screen->grabWindow(0).toImage();
        
        
        const char *pixels=(const char*)img.bits();
        socket->connectToHost("192.168.0.2", 8088);
        if(socket->waitForConnected(3000)){
            socket->write(pixels,pixelsLength);
            socket->waitForBytesWritten(1000);            
            socket->close();
        }
        QThread::msleep(90);
    }
    */
    Window window;
    window.show();
    return a.exec();
}
