#include "databaseadapter.h"

#include <QMessageBox>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDir>
#include <QTextStream>
#include <QtDebug>
#include <QStandardPaths>

DatabaseAdapter::DatabaseAdapter()
{
    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        QMessageBox::critical (nullptr,QObject::tr("Fatal error"), QObject::tr("The driver for the database is not available. This can happen if the file sqldrivers/qsqlite.dll cannot be found."));
        return;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QString writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if( writableLocation.isEmpty() )
    {
        QMessageBox::information (nullptr,QObject::tr("Error Message"),QObject::tr("There was a problem in opening the database. No writable location could be found. It is unlikely that you will solve this on your own. Rather you had better contact the developer.").arg(db.lastError().databaseText()) );
        return;
    }

    QDir dir(writableLocation);
    dir.mkpath(writableLocation);
    db.setDatabaseName( dir.absoluteFilePath("names.db"));

    if(!db.open())
    {
        QMessageBox::information (nullptr,QObject::tr("Error Message"),QObject::tr("There was a problem in opening the database. The program said: %1. It is unlikely that you will solve this on your own. Rather you had better contact the developer.").arg(db.lastError().databaseText()) );
        return;
    }
    if ( db.tables().contains( QLatin1String("names") ) ) {
         QSqlQuery q;
         q.exec("select count(*) from names;");
         q.next();
         if(q.value(0).toInt()<21742)
         {
             populateDatabaseFromResource();
         }
    } else {
         populateDatabaseFromResource();
    }

}

QString DatabaseAdapter::nameFromCodepoint(quint32 character) const
{
    QSqlDatabase db = QSqlDatabase::database();
    QString unicode = QString("%1").arg(character,4,16,QLatin1Char('0')).toUpper();

    QString name="Codepoint not found in database!";
    QSqlQuery query(db);
    query.prepare("select name from names where codepoint=?;");
    query.bindValue(0, unicode);
    if(query.exec() && query.next())
        name = query.value(0).toString();

    return name;
}

quint32 DatabaseAdapter::codepointFromName(const QString &name) const
{
    QSqlDatabase db = QSqlDatabase::database();
    quint32 codepoint = 0xFFFF;
    QSqlQuery query(db);
    query.prepare("select codepoint from names where name=?;");
    query.bindValue(0, name);
    if(query.exec() && query.next())
        codepoint = uintFromHexCodepoint( query.value(0).toString() );
    return codepoint;
}

quint32 DatabaseAdapter::uintFromHexCodepoint(QString codepoint)
{
    bool ok;
    return codepoint.toUInt(&ok,16);
}

void DatabaseAdapter::populateDatabaseFromResource()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    db.exec("DROP TABLE IF EXISTS names;");
    db.exec("CREATE TABLE names ( codepoint text, name text);");
    db.exec("CREATE INDEX name_index ON names(name);");
    db.exec("CREATE UNIQUE INDEX codepoint_index ON names(name);");
    QFile data(":/resources/UnicodeData.txt");
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        QSqlQuery q(db);
        q.prepare("INSERT INTO names VALUES (?,?);");
        while(!in.atEnd()) {
            QStringList fields = in.readLine().split(';');
            q.bindValue(0,fields.at(0));
            q.bindValue(1,fields.at(1));
            q.exec();
        }
        db.commit();
    } else {
        db.rollback();
        qDebug() << "Error opening resource";
    }
}
