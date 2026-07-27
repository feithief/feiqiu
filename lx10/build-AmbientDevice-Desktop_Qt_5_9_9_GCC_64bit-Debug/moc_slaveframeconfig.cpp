/****************************************************************************
** Meta object code from reading C++ file 'slaveframeconfig.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AmbientDevice/slaveframeconfig.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slaveframeconfig.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SlaveWriteThread_t {
    QByteArrayData data[3];
    char stringdata0[35];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SlaveWriteThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SlaveWriteThread_t qt_meta_stringdata_SlaveWriteThread = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SlaveWriteThread"
QT_MOC_LITERAL(1, 17, 16), // "WrittingFinished"
QT_MOC_LITERAL(2, 34, 0) // ""

    },
    "SlaveWriteThread\0WrittingFinished\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SlaveWriteThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void SlaveWriteThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SlaveWriteThread *_t = static_cast<SlaveWriteThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->WrittingFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SlaveWriteThread::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveWriteThread::WrittingFinished)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject SlaveWriteThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SlaveWriteThread.data,
      qt_meta_data_SlaveWriteThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SlaveWriteThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SlaveWriteThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SlaveWriteThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int SlaveWriteThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void SlaveWriteThread::WrittingFinished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_SlaveInitThread_t {
    QByteArrayData data[20];
    char stringdata0[234];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SlaveInitThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SlaveInitThread_t qt_meta_stringdata_SlaveInitThread = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SlaveInitThread"
QT_MOC_LITERAL(1, 16, 9), // "SWFetched"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 9), // "HWFetched"
QT_MOC_LITERAL(4, 37, 11), // "SuppFetched"
QT_MOC_LITERAL(5, 49, 11), // "FuncFetched"
QT_MOC_LITERAL(6, 61, 14), // "VariantFetched"
QT_MOC_LITERAL(7, 76, 9), // "SAFetched"
QT_MOC_LITERAL(8, 86, 9), // "GAFetched"
QT_MOC_LITERAL(9, 96, 15), // "PlatformFetched"
QT_MOC_LITERAL(10, 112, 16), // "IntensityFetched"
QT_MOC_LITERAL(11, 129, 11), // "LockFetched"
QT_MOC_LITERAL(12, 141, 8), // "RFetched"
QT_MOC_LITERAL(13, 150, 15), // "CalibrationInfo"
QT_MOC_LITERAL(14, 166, 8), // "GFetched"
QT_MOC_LITERAL(15, 175, 8), // "BFetched"
QT_MOC_LITERAL(16, 184, 9), // "PNFetched"
QT_MOC_LITERAL(17, 194, 13), // "SerialFetched"
QT_MOC_LITERAL(18, 208, 15), // "fechingFinished"
QT_MOC_LITERAL(19, 224, 9) // "ENodeType"

    },
    "SlaveInitThread\0SWFetched\0\0HWFetched\0"
    "SuppFetched\0FuncFetched\0VariantFetched\0"
    "SAFetched\0GAFetched\0PlatformFetched\0"
    "IntensityFetched\0LockFetched\0RFetched\0"
    "CalibrationInfo\0GFetched\0BFetched\0"
    "PNFetched\0SerialFetched\0fechingFinished\0"
    "ENodeType"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SlaveInitThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       3,    1,   97,    2, 0x06 /* Public */,
       4,    1,  100,    2, 0x06 /* Public */,
       5,    1,  103,    2, 0x06 /* Public */,
       6,    1,  106,    2, 0x06 /* Public */,
       7,    1,  109,    2, 0x06 /* Public */,
       8,    1,  112,    2, 0x06 /* Public */,
       9,    1,  115,    2, 0x06 /* Public */,
      10,    1,  118,    2, 0x06 /* Public */,
      11,    1,  121,    2, 0x06 /* Public */,
      12,    1,  124,    2, 0x06 /* Public */,
      14,    1,  127,    2, 0x06 /* Public */,
      15,    1,  130,    2, 0x06 /* Public */,
      16,    1,  133,    2, 0x06 /* Public */,
      17,    1,  136,    2, 0x06 /* Public */,
      18,    2,  139,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 19,    2,    2,

       0        // eod
};

void SlaveInitThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SlaveInitThread *_t = static_cast<SlaveInitThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SWFetched((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->HWFetched((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->SuppFetched((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->FuncFetched((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->VariantFetched((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->SAFetched((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->GAFetched((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->PlatformFetched((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->IntensityFetched((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->LockFetched((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->RFetched((*reinterpret_cast< CalibrationInfo(*)>(_a[1]))); break;
        case 11: _t->GFetched((*reinterpret_cast< CalibrationInfo(*)>(_a[1]))); break;
        case 12: _t->BFetched((*reinterpret_cast< CalibrationInfo(*)>(_a[1]))); break;
        case 13: _t->PNFetched((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->SerialFetched((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->fechingFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< ENodeType(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SlaveInitThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::SWFetched)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::HWFetched)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::SuppFetched)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::FuncFetched)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::VariantFetched)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::SAFetched)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::GAFetched)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::PlatformFetched)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::IntensityFetched)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::LockFetched)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(CalibrationInfo );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::RFetched)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(CalibrationInfo );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::GFetched)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(CalibrationInfo );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::BFetched)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::PNFetched)) {
                *result = 13;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::SerialFetched)) {
                *result = 14;
                return;
            }
        }
        {
            typedef void (SlaveInitThread::*_t)(bool , ENodeType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SlaveInitThread::fechingFinished)) {
                *result = 15;
                return;
            }
        }
    }
}

const QMetaObject SlaveInitThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SlaveInitThread.data,
      qt_meta_data_SlaveInitThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SlaveInitThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SlaveInitThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SlaveInitThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int SlaveInitThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void SlaveInitThread::SWFetched(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SlaveInitThread::HWFetched(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SlaveInitThread::SuppFetched(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SlaveInitThread::FuncFetched(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SlaveInitThread::VariantFetched(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SlaveInitThread::SAFetched(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SlaveInitThread::GAFetched(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void SlaveInitThread::PlatformFetched(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void SlaveInitThread::IntensityFetched(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void SlaveInitThread::LockFetched(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void SlaveInitThread::RFetched(CalibrationInfo _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void SlaveInitThread::GFetched(CalibrationInfo _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void SlaveInitThread::BFetched(CalibrationInfo _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void SlaveInitThread::PNFetched(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void SlaveInitThread::SerialFetched(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void SlaveInitThread::fechingFinished(bool _t1, ENodeType _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
struct qt_meta_stringdata_SlaveFrameConfig_t {
    QByteArrayData data[19];
    char stringdata0[259];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SlaveFrameConfig_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SlaveFrameConfig_t qt_meta_stringdata_SlaveFrameConfig = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SlaveFrameConfig"
QT_MOC_LITERAL(1, 17, 21), // "buttonCalibrateNormal"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 16), // "buttonCalibrateR"
QT_MOC_LITERAL(4, 57, 16), // "buttonCalibrateG"
QT_MOC_LITERAL(5, 74, 16), // "buttonCalibrateB"
QT_MOC_LITERAL(6, 91, 15), // "updateNodeState"
QT_MOC_LITERAL(7, 107, 11), // "SlaveStatus"
QT_MOC_LITERAL(8, 119, 6), // "status"
QT_MOC_LITERAL(9, 126, 15), // "exitSlaveConfig"
QT_MOC_LITERAL(10, 142, 13), // "getLockStatus"
QT_MOC_LITERAL(11, 156, 9), // "getRValue"
QT_MOC_LITERAL(12, 166, 15), // "CalibrationInfo"
QT_MOC_LITERAL(13, 182, 9), // "getGValue"
QT_MOC_LITERAL(14, 192, 9), // "getBValue"
QT_MOC_LITERAL(15, 202, 13), // "changeConfigs"
QT_MOC_LITERAL(16, 216, 15), // "processReadDone"
QT_MOC_LITERAL(17, 232, 9), // "ENodeType"
QT_MOC_LITERAL(18, 242, 16) // "processWriteDone"

    },
    "SlaveFrameConfig\0buttonCalibrateNormal\0"
    "\0buttonCalibrateR\0buttonCalibrateG\0"
    "buttonCalibrateB\0updateNodeState\0"
    "SlaveStatus\0status\0exitSlaveConfig\0"
    "getLockStatus\0getRValue\0CalibrationInfo\0"
    "getGValue\0getBValue\0changeConfigs\0"
    "processReadDone\0ENodeType\0processWriteDone"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SlaveFrameConfig[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    1,   83,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    1,   87,    2, 0x08 /* Private */,
      11,    1,   90,    2, 0x08 /* Private */,
      13,    1,   93,    2, 0x08 /* Private */,
      14,    1,   96,    2, 0x08 /* Private */,
      15,    0,   99,    2, 0x08 /* Private */,
      16,    2,  100,    2, 0x08 /* Private */,
      18,    1,  105,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 12,    2,
    QMetaType::Void, 0x80000000 | 12,    2,
    QMetaType::Void, 0x80000000 | 12,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 17,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void SlaveFrameConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SlaveFrameConfig *_t = static_cast<SlaveFrameConfig *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->buttonCalibrateNormal(); break;
        case 1: _t->buttonCalibrateR(); break;
        case 2: _t->buttonCalibrateG(); break;
        case 3: _t->buttonCalibrateB(); break;
        case 4: _t->updateNodeState((*reinterpret_cast< SlaveStatus(*)>(_a[1]))); break;
        case 5: _t->exitSlaveConfig(); break;
        case 6: _t->getLockStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->getRValue((*reinterpret_cast< CalibrationInfo(*)>(_a[1]))); break;
        case 8: _t->getGValue((*reinterpret_cast< CalibrationInfo(*)>(_a[1]))); break;
        case 9: _t->getBValue((*reinterpret_cast< CalibrationInfo(*)>(_a[1]))); break;
        case 10: _t->changeConfigs(); break;
        case 11: _t->processReadDone((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< ENodeType(*)>(_a[2]))); break;
        case 12: _t->processWriteDone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SlaveFrameConfig::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SlaveFrameConfig.data,
      qt_meta_data_SlaveFrameConfig,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SlaveFrameConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SlaveFrameConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SlaveFrameConfig.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SlaveFrameConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
