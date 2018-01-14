/****************************************************************************
** Meta object code from reading C++ file 'finderwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/finderwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'finderwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FinderWindow_t {
    QByteArrayData data[8];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FinderWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FinderWindow_t qt_meta_stringdata_FinderWindow = {
    {
QT_MOC_LITERAL(0, 0, 12), // "FinderWindow"
QT_MOC_LITERAL(1, 13, 12), // "searchResult"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 6), // "launch"
QT_MOC_LITERAL(4, 34, 13), // "newConnection"
QT_MOC_LITERAL(5, 48, 23), // "on_searchBar_textEdited"
QT_MOC_LITERAL(6, 72, 4), // "arg1"
QT_MOC_LITERAL(7, 77, 4) // "exit"

    },
    "FinderWindow\0searchResult\0\0launch\0"
    "newConnection\0on_searchBar_textEdited\0"
    "arg1\0exit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FinderWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    1,   42,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,

       0        // eod
};

void FinderWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FinderWindow *_t = static_cast<FinderWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->searchResult(); break;
        case 1: _t->launch(); break;
        case 2: _t->newConnection(); break;
        case 3: _t->on_searchBar_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->exit(); break;
        default: ;
        }
    }
}

const QMetaObject FinderWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FinderWindow.data,
      qt_meta_data_FinderWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FinderWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FinderWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FinderWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FinderWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
