/****************************************************************************
** Meta object code from reading C++ file 'bcmmasterframe.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AmbientDevice/bcmmasterframe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bcmmasterframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BCMMasterFrame_t {
    QByteArrayData data[24];
    char stringdata0[260];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BCMMasterFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BCMMasterFrame_t qt_meta_stringdata_BCMMasterFrame = {
    {
QT_MOC_LITERAL(0, 0, 14), // "BCMMasterFrame"
QT_MOC_LITERAL(1, 15, 11), // "changeColor"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 1), // "i"
QT_MOC_LITERAL(4, 30, 7), // "drawCIE"
QT_MOC_LITERAL(5, 38, 17), // "change_RGBW_color"
QT_MOC_LITERAL(6, 56, 15), // "syncModeChanged"
QT_MOC_LITERAL(7, 72, 16), // "CheckModeChanged"
QT_MOC_LITERAL(8, 89, 19), // "TenColorModeChanged"
QT_MOC_LITERAL(9, 109, 10), // "changeAddr"
QT_MOC_LITERAL(10, 120, 5), // "value"
QT_MOC_LITERAL(11, 126, 7), // "changeR"
QT_MOC_LITERAL(12, 134, 1), // "r"
QT_MOC_LITERAL(13, 136, 7), // "changeG"
QT_MOC_LITERAL(14, 144, 1), // "g"
QT_MOC_LITERAL(15, 146, 7), // "changeB"
QT_MOC_LITERAL(16, 154, 1), // "b"
QT_MOC_LITERAL(17, 156, 7), // "changeI"
QT_MOC_LITERAL(18, 164, 13), // "changeDimRamp"
QT_MOC_LITERAL(19, 178, 7), // "dimramp"
QT_MOC_LITERAL(20, 186, 6), // "Accept"
QT_MOC_LITERAL(21, 193, 6), // "Cancel"
QT_MOC_LITERAL(22, 200, 29), // "on_pushButtonCancel_2_clicked"
QT_MOC_LITERAL(23, 230, 29) // "on_pushButtonCancel_3_clicked"

    },
    "BCMMasterFrame\0changeColor\0\0i\0drawCIE\0"
    "change_RGBW_color\0syncModeChanged\0"
    "CheckModeChanged\0TenColorModeChanged\0"
    "changeAddr\0value\0changeR\0r\0changeG\0g\0"
    "changeB\0b\0changeI\0changeDimRamp\0dimramp\0"
    "Accept\0Cancel\0on_pushButtonCancel_2_clicked\0"
    "on_pushButtonCancel_3_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BCMMasterFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x08 /* Private */,
       4,    0,   97,    2, 0x08 /* Private */,
       5,    0,   98,    2, 0x08 /* Private */,
       6,    0,   99,    2, 0x08 /* Private */,
       7,    0,  100,    2, 0x08 /* Private */,
       8,    0,  101,    2, 0x08 /* Private */,
       9,    1,  102,    2, 0x08 /* Private */,
      11,    1,  105,    2, 0x08 /* Private */,
      13,    1,  108,    2, 0x08 /* Private */,
      15,    1,  111,    2, 0x08 /* Private */,
      17,    1,  114,    2, 0x08 /* Private */,
      18,    1,  117,    2, 0x08 /* Private */,
      20,    0,  120,    2, 0x08 /* Private */,
      21,    0,  121,    2, 0x08 /* Private */,
      22,    0,  122,    2, 0x08 /* Private */,
      23,    0,  123,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BCMMasterFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BCMMasterFrame *_t = static_cast<BCMMasterFrame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeColor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->drawCIE(); break;
        case 2: _t->change_RGBW_color(); break;
        case 3: _t->syncModeChanged(); break;
        case 4: _t->CheckModeChanged(); break;
        case 5: _t->TenColorModeChanged(); break;
        case 6: _t->changeAddr((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->changeR((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->changeG((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->changeB((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->changeI((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->changeDimRamp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->Accept(); break;
        case 13: _t->Cancel(); break;
        case 14: _t->on_pushButtonCancel_2_clicked(); break;
        case 15: _t->on_pushButtonCancel_3_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject BCMMasterFrame::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_BCMMasterFrame.data,
      qt_meta_data_BCMMasterFrame,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BCMMasterFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BCMMasterFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BCMMasterFrame.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int BCMMasterFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
