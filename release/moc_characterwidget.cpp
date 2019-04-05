/****************************************************************************
** Meta object code from reading C++ file 'characterwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../characterwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'characterwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CharacterWidget_t {
    QByteArrayData data[18];
    char stringdata0[212];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CharacterWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CharacterWidget_t qt_meta_stringdata_CharacterWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CharacterWidget"
QT_MOC_LITERAL(1, 16, 17), // "characterSelected"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 9), // "character"
QT_MOC_LITERAL(4, 45, 22), // "characterDoubleClicked"
QT_MOC_LITERAL(5, 68, 10), // "updateFont"
QT_MOC_LITERAL(6, 79, 4), // "font"
QT_MOC_LITERAL(7, 84, 26), // "updateCharacterDisplayFont"
QT_MOC_LITERAL(8, 111, 14), // "cursorPosition"
QT_MOC_LITERAL(9, 126, 3), // "old"
QT_MOC_LITERAL(10, 130, 6), // "newpos"
QT_MOC_LITERAL(11, 137, 15), // "updateSelection"
QT_MOC_LITERAL(12, 153, 4), // "left"
QT_MOC_LITERAL(13, 158, 6), // "length"
QT_MOC_LITERAL(14, 165, 18), // "updateHasSelection"
QT_MOC_LITERAL(15, 184, 12), // "hasSelection"
QT_MOC_LITERAL(16, 197, 10), // "updateText"
QT_MOC_LITERAL(17, 208, 3) // "str"

    },
    "CharacterWidget\0characterSelected\0\0"
    "character\0characterDoubleClicked\0"
    "updateFont\0font\0updateCharacterDisplayFont\0"
    "cursorPosition\0old\0newpos\0updateSelection\0"
    "left\0length\0updateHasSelection\0"
    "hasSelection\0updateText\0str"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CharacterWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   60,    2, 0x0a /* Public */,
       7,    1,   63,    2, 0x0a /* Public */,
       8,    2,   66,    2, 0x0a /* Public */,
      11,    2,   71,    2, 0x0a /* Public */,
      14,    1,   76,    2, 0x0a /* Public */,
      16,    1,   79,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::UInt,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QFont,    6,
    QMetaType::Void, QMetaType::QFont,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   12,   13,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::QString,   17,

       0        // eod
};

void CharacterWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CharacterWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->characterSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->characterDoubleClicked((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 2: _t->updateFont((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 3: _t->updateCharacterDisplayFont((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 4: _t->cursorPosition((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->updateSelection((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->updateHasSelection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->updateText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CharacterWidget::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CharacterWidget::characterSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CharacterWidget::*)(quint32 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CharacterWidget::characterDoubleClicked)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CharacterWidget::staticMetaObject = { {
    &QFrame::staticMetaObject,
    qt_meta_stringdata_CharacterWidget.data,
    qt_meta_data_CharacterWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CharacterWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CharacterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CharacterWidget.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int CharacterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CharacterWidget::characterSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CharacterWidget::characterDoubleClicked(quint32 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
