/****************************************************************************
** Meta object code from reading C++ file 'QT_Platform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QT_Platform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QT_Platform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QT_Platform_t {
    QByteArrayData data[17];
    char stringdata0[192];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QT_Platform_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QT_Platform_t qt_meta_stringdata_QT_Platform = {
    {
QT_MOC_LITERAL(0, 0, 11), // "QT_Platform"
QT_MOC_LITERAL(1, 12, 13), // "serial_dector"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 14), // "serial_connect"
QT_MOC_LITERAL(4, 42, 12), // "periphChange"
QT_MOC_LITERAL(5, 55, 10), // "set_Config"
QT_MOC_LITERAL(6, 66, 11), // "set_Config2"
QT_MOC_LITERAL(7, 78, 8), // "switch_1"
QT_MOC_LITERAL(8, 87, 8), // "switch_2"
QT_MOC_LITERAL(9, 96, 8), // "switch_3"
QT_MOC_LITERAL(10, 105, 8), // "switch_4"
QT_MOC_LITERAL(11, 114, 8), // "switch_5"
QT_MOC_LITERAL(12, 123, 15), // "contral1_adjust"
QT_MOC_LITERAL(13, 139, 15), // "contral2_adjust"
QT_MOC_LITERAL(14, 155, 15), // "contral3_adjust"
QT_MOC_LITERAL(15, 171, 11), // "adc_refresh"
QT_MOC_LITERAL(16, 183, 8) // "set_Init"

    },
    "QT_Platform\0serial_dector\0\0serial_connect\0"
    "periphChange\0set_Config\0set_Config2\0"
    "switch_1\0switch_2\0switch_3\0switch_4\0"
    "switch_5\0contral1_adjust\0contral2_adjust\0"
    "contral3_adjust\0adc_refresh\0set_Init"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QT_Platform[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    0,   91,    2, 0x08 /* Private */,
       5,    0,   92,    2, 0x08 /* Private */,
       6,    0,   93,    2, 0x08 /* Private */,
       7,    0,   94,    2, 0x08 /* Private */,
       8,    0,   95,    2, 0x08 /* Private */,
       9,    0,   96,    2, 0x08 /* Private */,
      10,    0,   97,    2, 0x08 /* Private */,
      11,    0,   98,    2, 0x08 /* Private */,
      12,    0,   99,    2, 0x08 /* Private */,
      13,    0,  100,    2, 0x08 /* Private */,
      14,    0,  101,    2, 0x08 /* Private */,
      15,    0,  102,    2, 0x08 /* Private */,
      16,    0,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QT_Platform::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QT_Platform *_t = static_cast<QT_Platform *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->serial_dector(); break;
        case 1: _t->serial_connect(); break;
        case 2: _t->periphChange(); break;
        case 3: _t->set_Config(); break;
        case 4: _t->set_Config2(); break;
        case 5: _t->switch_1(); break;
        case 6: _t->switch_2(); break;
        case 7: _t->switch_3(); break;
        case 8: _t->switch_4(); break;
        case 9: _t->switch_5(); break;
        case 10: _t->contral1_adjust(); break;
        case 11: _t->contral2_adjust(); break;
        case 12: _t->contral3_adjust(); break;
        case 13: _t->adc_refresh(); break;
        case 14: _t->set_Init(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QT_Platform::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QT_Platform.data,
      qt_meta_data_QT_Platform,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QT_Platform::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QT_Platform::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QT_Platform.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int QT_Platform::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
