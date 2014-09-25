#include "databaseadapter.h"

#include <QMessageBox>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDir>

DatabaseAdapter::DatabaseAdapter()
    : mDatabaseError(false)
{
    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        QMessageBox::critical (0,QObject::tr("Fatal error"), QObject::tr("The driver for the database is not available. This can happen if the file sqldrivers/qsqlite.dll cannot be found."));
        mDatabaseError=true;
        return;
    }
    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setDatabaseName("names.db");
    if(!mDb.open())
    {
        QMessageBox::information (0,QObject::tr("Error Message"),QObject::tr("There was a problem in opening the database. The program said: %1. It is unlikely that you will solve this on your own. Rather you had better contact the developer.").arg(mDb.lastError().databaseText()) );
        mDatabaseError=true;
        return;
    }
    QSqlQuery q;
    q.exec("select count(*) from names;");
    q.next();
    if(q.value(0).toInt()<21742)
    {
        QMessageBox::information (0,QObject::tr("Error Message"),QObject::tr("There was a problem in reading names.db (I was looking for %1). The ability to look up glyph names will either be impaired or completely unavailable.").arg(QDir::current().filePath("names.db")) );
        mDatabaseError=true;
    }
}

bool DatabaseAdapter::databaseError() const
{
    return mDatabaseError;
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
