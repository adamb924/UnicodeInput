#ifndef CODEPOINTPROXY_H
#define CODEPOINTPROXY_H

#include <QtWidgets>
#include <QFontMetrics>

class CodepointProxy : public QSortFilterProxyModel
{
public:
    CodepointProxy(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    void setFont(const QFont & font);
    void setUseDisplaySize(bool use);

private:
    QFont mFont;
    QFontMetrics mFontMetrics;
    bool mUseDisplaySize;
};

#endif // CODEPOINTPROXY_H
