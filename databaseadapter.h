#ifndef DATABASEADAPTER_H
#define DATABASEADAPTER_H

#include <QSqlDatabase>

class DatabaseAdapter
{
public:
    DatabaseAdapter();

    QString nameFromCodepoint(quint32 character) const;
    quint32 codepointFromName(const QString & name) const;
    QStringList searchGlyphName(const QString & searchString, int maxCount, bool substringSearch, bool sortByCodepoint) const;
    static quint32 uintFromHexCodepoint(QString codepoint);

    QSqlDatabase db() const;

private:
    void populateDatabaseFromResource();

    QSqlDatabase mDb;
};

#endif // DATABASEADAPTER_H
