#ifndef DATABASEADAPTER_H
#define DATABASEADAPTER_H

#include <QSqlDatabase>

class DatabaseAdapter
{
public:
    DatabaseAdapter();

    bool databaseError() const;

    QString nameFromCodepoint(quint32 character) const;
    QStringList searchGlyphName(const QString & searchString, int maxCount, bool substringSearch, bool sortByCodepoint) const;
    static quint32 uintFromHexCodepoint(QString codepoint);

    QSqlDatabase db() const;

private:
    QSqlDatabase mDb;
    bool mDatabaseError;
};

#endif // DATABASEADAPTER_H
