/****************************************************************************
** Meta object code from reading C++ file 'productionverify.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AmbientDevice/productionverify.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'productionverify.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ProductionVerify_t {
    QByteArrayData data[9];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProductionVerify_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProductionVerify_t qt_meta_stringdata_ProductionVerify = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ProductionVerify"
QT_MOC_LITERAL(1, 17, 13), // "changeAddress"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 5), // "index"
QT_MOC_LITERAL(4, 38, 11), // "changeColor"
QT_MOC_LITERAL(5, 50, 15), // "changeColorAuto"
QT_MOC_LITERAL(6, 66, 17), // "changeColorManual"
QT_MOC_LITERAL(7, 84, 16), // "intensityChanged"
QT_MOC_LITERAL(8, 101, 9) // "intensity"

    },
    "ProductionVerify\0changeAddress\0\0index\0"
    "changeColor\0changeColorAuto\0"
    "changeColorManual\0intensityChanged\0"
    "intensity"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProductionVerify[] = {

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
       1,    1,   39,    2, 0x08 /* Private */,
       4,    0,   42,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    1,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,

       0        // eod
};

void ProductionVerify::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ProductionVerify *_t = static_cast<ProductionVerify *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeAddress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->changeColor(); break;
        case 2: _t->changeColorAuto(); break;
        case 3: _t->changeColorManual(); break;
        case 4: _t->intensityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ProductionVerify::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ProductionVerify.data,
      qt_meta_data_ProductionVerify,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ProductionVerify::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProductionVerify::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProductionVerify.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProductionVerify::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
