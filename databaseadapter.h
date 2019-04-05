#ifndef DATABASEADAPTER_H
#define DATABASEADAPTER_H

#include <QSqlDatabase>

class DatabaseAdapter
{
public:
    DatabaseAdapter();

    QString nameFromCodepoint(quint32 character) const;
    quint32 codepointFromName(const QString & name) const;
    static quint32 uintFromHexCodepoint(QString codepoint);

private:
    void populateDatabaseFromResource();
};

#endif // DATABASEADAPTER_H
