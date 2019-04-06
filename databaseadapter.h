#ifndef DATABASEADAPTER_H
#define DATABASEADAPTER_H

#include <QSqlDatabase>

class DatabaseAdapter
{
public:
    DatabaseAdapter();

    static void initializeDatabase();
    static QString nameFromCodepoint(quint32 character);
    static quint32 codepointFromName(const QString & name);
    static quint32 uintFromHexCodepoint(QString codepoint);

private:
    static void populateDatabaseFromResource();
};

#endif // DATABASEADAPTER_H
