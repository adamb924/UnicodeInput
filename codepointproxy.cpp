#include "codepointproxy.h"

#include "databaseadapter.h"

CodepointProxy::CodepointProxy() : mFontMetrics(mFont),  mUseDisplaySize(false)
{

}

QVariant CodepointProxy::data(const QModelIndex &index, int role) const
{
    if( role == Qt::TextAlignmentRole)
    {
        if( index.column() > 0 )
        {
            return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
        }
        else
        {
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    if ( index.column() != 1 )
    {
        return QSortFilterProxyModel::data(index, role);
    }
    else
    {
        if( role == Qt::DisplayRole )
        {
            return QString( DatabaseAdapter::uintFromHexCodepoint( sourceModel()->data(index).toString() ) );
        }
        else if ( role == Qt::FontRole )
        {
            QFont f;
            f.fromString(QSortFilterProxyModel::data(index, role).toString());
            f.setFamily( mFont.family() );
            if( mUseDisplaySize )
            {
                f.setPointSize( mFont.pointSize() );
            }
            return QVariant::fromValue(f);
        }
        else if ( role == Qt::SizeHintRole )
        {
            if( mUseDisplaySize )
            {
                return QSize( mFontMetrics.averageCharWidth(), mFontMetrics.height() );
            }
            else
            {
                return QSortFilterProxyModel::data(index, role);
            }
        }
        else
        {
            return QSortFilterProxyModel::data(index, role);
        }
    }
}

void CodepointProxy::setFont(const QFont &font)
{
    mFont = font;
    mFontMetrics = QFontMetrics(mFont);
}

void CodepointProxy::setUseDisplaySize(bool use)
{
    mUseDisplaySize = use;
}
