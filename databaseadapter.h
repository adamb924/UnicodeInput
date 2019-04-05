#ifndef DATABASEADAPTER_H
#define DATABASEADAPTER_H

#include <QSqlDatabase>

class DatabaseAdapter
{
public:
    DatabaseAdapter();
    ~DatabaseAdapter();

    QString nameFromCodepoint(quint32 character) const;
    quint32 codepointFromName(const QString & name) const;
    static quint32 uintFromHexCodepoint(QString codepoint);

    QSqlDatabase db() const;

private:
    void populateDatabaseFromResource();

    QSqlDatabase mDb;
};

#endif // DATABASEADAPTER_H
