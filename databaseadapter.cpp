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
    mDb = QSqlDatabase::addDatabase("QSQLITE");

    QString writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if( writableLocation.isEmpty() )
    {
        QMessageBox::information (nullptr,QObject::tr("Error Message"),QObject::tr("There was a problem in opening the database. No writable location could be found. It is unlikely that you will solve this on your own. Rather you had better contact the developer.").arg(mDb.lastError().databaseText()) );
        return;
    }

    QDir dir(writableLocation);
    dir.mkpath(writableLocation);
    mDb.setDatabaseName( dir.absoluteFilePath("names.db"));

    if(!mDb.open())
    {
        QMessageBox::information (nullptr,QObject::tr("Error Message"),QObject::tr("There was a problem in opening the database. The program said: %1. It is unlikely that you will solve this on your own. Rather you had better contact the developer.").arg(mDb.lastError().databaseText()) );
        return;
    }
    if ( mDb.tables().contains( QLatin1String("names") ) ) {
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

DatabaseAdapter::~DatabaseAdapter()
{
    mDb.close();
}

QString DatabaseAdapter::nameFromCodepoint(quint32 character) const
{
    QString unicode = QString("%1").arg(character,4,16,QLatin1Char('0')).toUpper();

    QString name="Codepoint not found in database!";
    QSqlQuery query(mDb);
    query.prepare("select name from names where codepoint=?;");
    query.bindValue(0, unicode);
    if(query.exec() && query.next())
        name = query.value(0).toString();

    return name;
}

quint32 DatabaseAdapter::codepointFromName(const QString &name) const
{
    quint32 codepoint = 0xFFFF;
    QSqlQuery query(mDb);
    query.prepare("select codepoint from names where name=?;");
    query.bindValue(0, name);
    if(query.exec() && query.next())
        codepoint = uintFromHexCodepoint( query.value(0).toString() );
    return codepoint;
}

QStringList DatabaseAdapter::searchGlyphName(const QString &searchString, int maxCount, bool substringSearch, bool sortByCodepoint) const
{
    if( searchString.isEmpty() ) return QStringList();

    QStringList results;

    QString order;
    if( sortByCodepoint )
        order = "length(codepoint),codepoint";
    else
        order = "name";

    QSqlQuery query(mDb);
    query.exec(QString("select name,codepoint from names where substr(name,1,%1)='%2' order by %3;").arg(searchString.length()).arg(searchString).arg(order));
    while(query.next())
    {
        results << QString("%1 (U+%2)").arg(query.value(0).toString()).arg(query.value(1).toString());
    }

    if(substringSearch && searchString.length() > 1 )
    {
        query.exec(QString("select name,codepoint from names where name like '%_%1%' order by %2 limit %3;").arg(searchString).arg(order).arg(maxCount) );
        while(query.next())
            results << QString("%1 (U+%2)").arg(query.value(0).toString()).arg(query.value(1).toString());
    }

    return results;
}

quint32 DatabaseAdapter::uintFromHexCodepoint(QString codepoint)
{
    bool ok;
    return codepoint.toUInt(&ok,16);
}

QSqlDatabase DatabaseAdapter::db() const
{
    return mDb;
}

void DatabaseAdapter::populateDatabaseFromResource()
{
    mDb.transaction();
    mDb.exec("DROP TABLE IF EXISTS names;");
    mDb.exec("CREATE TABLE names ( codepoint text, name text);");
    mDb.exec("CREATE INDEX name_index ON names(name);");
    mDb.exec("CREATE UNIQUE INDEX codepoint_index ON names(name);");
    QFile data(":/resources/UnicodeData.txt");
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        QSqlQuery q(mDb);
        q.prepare("INSERT INTO names VALUES (?,?);");
        while(!in.atEnd()) {
            QStringList fields = in.readLine().split(';');
            q.bindValue(0,fields.at(0));
            q.bindValue(1,fields.at(1));
            q.exec();
        }
        mDb.commit();
    } else {
        mDb.rollback();
        qDebug() << "Error opening resource";
    }
}
