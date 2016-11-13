import socket
import time
import sys
import PyQt4
from PyQt4.QtCore import  QBuffer, QByteArray 
from PyQt4.QtGui import QPixmap, QApplication, QPainter, QCursor, QColor,QPen, QBrush


def getImage():
    imageDesktop=QPixmap.grabWindow(QApplication.desktop().winId())
    painter =QPainter()
    painter.begin(imageDesktop);
    darkGray=QPen((QColor(69,86,96,100)),1);
    cursor=QCursor()
    print QCursor.pos()
    painter.setBrush(QBrush(QColor(69,86,96,100)));
    painter.drawEllipse(QCursor.pos(),4,4);
    painter.end();
    img= imageDesktop.toImage()
    width=img.width()
    height=img.height()    
    return img;

def imgToBase64():
    ba= QByteArray()
    bufferImage= QBuffer(ba)
    bufferImage.open(PyQt4.QtCore.QIODevice.WriteOnly)
    
    img= getImage()
    img.save(bufferImage,"PNG")
    base64=ba.toBase64().data()
    return  bufferImage.size(),base64

def getHeaderForHttpServer(data):
        header="HTTP/1.1 200 OK\r\n"
        #header=header+"Date: Mon, 23 May 2005 22:38:34 GMT\r\n"
        ##header=header+"Content-Type: text/html; charset=UTF-8\r\n"
        header=header+"Content-Type: image/png;base64\r\n"
        header=header+"Content-Encoding: UTF-8\r\n"
        header=header+"Content-Length: "+str(len(data))+"\r\n"
        #header=header+"Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT\r\n"
        header=header+"Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\r\n"
        #header=header+"ETag: \"3f80f-1b6-3e1cb03b\"\r\n"
        header=header+"Access-Control-Allow-Origin: *\r\n"
        header=header+"Connection: close\r\n\r\n"

        return header
def server():    
    #setup server=======================================================    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.bind(('', 8888))
    except socket.error as msg:
        print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
        sys.exit()
     
    print 'Socket bind complete' 
    #Start listening on socket    
    s.listen(5)
    print 'Socket now listening' 
    #now keep talking with the client
    #end===============================================================
    while 1:        
        #send image========================================================
        conn, addr = s.accept()
        sizeImage,imageData=imgToBase64()
        dataRecv=conn.recv(1024).decode("ascii")
        conn.send((getHeaderForHttpServer(imageData)+imageData+"=").encode())
        conn.close()
        #end===============================================================
        
    s.close()

timeStep=1.0/30.0
app = QApplication(sys.argv)

server()
