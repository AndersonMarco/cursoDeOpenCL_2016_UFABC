#ifndef FILEOPERATIONSFROMRESOURCE_H
#define FILEOPERATIONSFROMRESOURCE_H
#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QResource>
#include <QIODevice>

class FileOperationsFromResource
{
public:
    FileOperationsFromResource();
    static QString readFile(QString filePath);
};

#endif // FILEOPERATIONSFROMRESOURCE_H
