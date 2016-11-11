#include "fileoperationsfromresource.h"

FileOperationsFromResource::FileOperationsFromResource()
{

}
QString FileOperationsFromResource::readFile(QString filePath){
    QFile mFile(filePath);

       if(!mFile.open(QFile::ReadOnly | QFile::Text)){
           qDebug() << "could not open file for read";
           return NULL;
       }

           QTextStream in(&mFile);
           QString mText = in.readAll();


            qDebug() <<    mText;
           mFile.close();

    return mText;

}
