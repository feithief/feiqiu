/****************************************************************************
** Meta object code from reading C++ file 'ambientlinscheduler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../feiqiu-main/feiqiu-main/LIN10_ColorPalette_Host/ambientlinscheduler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ambientlinscheduler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AmbientLinScheduler_t {
    QByteArrayData data[34];
    char stringdata0[485];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AmbientLinScheduler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AmbientLinScheduler_t qt_meta_stringdata_AmbientLinScheduler = {
    {
QT_MOC_LITERAL(0, 0, 19), // "AmbientLinScheduler"
QT_MOC_LITERAL(1, 20, 18), // "SlaveStatusChanged"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 11), // "SlaveStatus"
QT_MOC_LITERAL(4, 52, 6), // "status"
QT_MOC_LITERAL(5, 59, 21), // "nodeConfigurationRead"
QT_MOC_LITERAL(6, 81, 9), // "requestId"
QT_MOC_LITERAL(7, 91, 15), // "SlaveConfigInfo"
QT_MOC_LITERAL(8, 107, 4), // "info"
QT_MOC_LITERAL(9, 112, 7), // "success"
QT_MOC_LITERAL(10, 120, 12), // "errorMessage"
QT_MOC_LITERAL(11, 133, 24), // "nodeConfigurationWritten"
QT_MOC_LITERAL(12, 158, 4), // "node"
QT_MOC_LITERAL(13, 163, 19), // "calibrationFinished"
QT_MOC_LITERAL(14, 183, 15), // "busStateChanged"
QT_MOC_LITERAL(15, 199, 5), // "ready"
QT_MOC_LITERAL(16, 205, 7), // "message"
QT_MOC_LITERAL(17, 213, 13), // "stopRequested"
QT_MOC_LITERAL(18, 227, 22), // "controlSignalRequested"
QT_MOC_LITERAL(19, 250, 9), // "BCMSignal"
QT_MOC_LITERAL(20, 260, 6), // "signal"
QT_MOC_LITERAL(21, 267, 28), // "switchControlSignalRequested"
QT_MOC_LITERAL(22, 296, 21), // "signalPresetRequested"
QT_MOC_LITERAL(23, 318, 11), // "presetIndex"
QT_MOC_LITERAL(24, 330, 17), // "readNodeRequested"
QT_MOC_LITERAL(25, 348, 18), // "writeNodeRequested"
QT_MOC_LITERAL(26, 367, 20), // "calibrationRequested"
QT_MOC_LITERAL(27, 388, 4), // "mode"
QT_MOC_LITERAL(28, 393, 15), // "cancelRequested"
QT_MOC_LITERAL(29, 409, 14), // "sleepRequested"
QT_MOC_LITERAL(30, 424, 13), // "wakeRequested"
QT_MOC_LITERAL(31, 438, 19), // "busEnabledRequested"
QT_MOC_LITERAL(32, 458, 7), // "enabled"
QT_MOC_LITERAL(33, 466, 18) // "flushControlSignal"

    },
    "AmbientLinScheduler\0SlaveStatusChanged\0"
    "\0SlaveStatus\0status\0nodeConfigurationRead\0"
    "requestId\0SlaveConfigInfo\0info\0success\0"
    "errorMessage\0nodeConfigurationWritten\0"
    "node\0calibrationFinished\0busStateChanged\0"
    "ready\0message\0stopRequested\0"
    "controlSignalRequested\0BCMSignal\0"
    "signal\0switchControlSignalRequested\0"
    "signalPresetRequested\0presetIndex\0"
    "readNodeRequested\0writeNodeRequested\0"
    "calibrationRequested\0mode\0cancelRequested\0"
    "sleepRequested\0wakeRequested\0"
    "busEnabledRequested\0enabled\0"
    "flushControlSignal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AmbientLinScheduler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,
       5,    4,  102,    2, 0x06 /* Public */,
      11,    4,  111,    2, 0x06 /* Public */,
      13,    4,  120,    2, 0x06 /* Public */,
      14,    2,  129,    2, 0x06 /* Public */,
      17,    0,  134,    2, 0x06 /* Public */,
      18,    1,  135,    2, 0x06 /* Public */,
      21,    1,  138,    2, 0x06 /* Public */,
      22,    1,  141,    2, 0x06 /* Public */,
      24,    2,  144,    2, 0x06 /* Public */,
      25,    2,  149,    2, 0x06 /* Public */,
      26,    3,  154,    2, 0x06 /* Public */,
      28,    1,  161,    2, 0x06 /* Public */,
      29,    0,  164,    2, 0x06 /* Public */,
      30,    0,  165,    2, 0x06 /* Public */,
      31,    1,  166,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      33,    0,  169,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 7, QMetaType::Bool, QMetaType::QString,    6,    8,    9,   10,
    QMetaType::Void, QMetaType::UInt, QMetaType::UChar, QMetaType::Bool, QMetaType::QString,    6,   12,    9,   10,
    QMetaType::Void, QMetaType::UInt, QMetaType::UChar, QMetaType::Bool, QMetaType::QString,    6,   12,    9,   10,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   15,   16,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void, QMetaType::UInt, QMetaType::UChar,    6,   12,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 7,    6,    8,
    QMetaType::Void, QMetaType::UInt, QMetaType::UChar, QMetaType::UChar,    6,   12,   27,
    QMetaType::Void, QMetaType::UInt,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   32,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void AmbientLinScheduler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AmbientLinScheduler *_t = static_cast<AmbientLinScheduler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SlaveStatusChanged((*reinterpret_cast< SlaveStatus(*)>(_a[1]))); break;
        case 1: _t->nodeConfigurationRead((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< SlaveConfigInfo(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 2: _t->nodeConfigurationWritten((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 3: _t->calibrationFinished((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 4: _t->busStateChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->stopRequested(); break;
        case 6: _t->controlSignalRequested((*reinterpret_cast< BCMSignal(*)>(_a[1]))); break;
        case 7: _t->switchControlSignalRequested((*reinterpret_cast< BCMSignal(*)>(_a[1]))); break;
        case 8: _t->signalPresetRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->readNodeRequested((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 10: _t->writeNodeRequested((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< SlaveConfigInfo(*)>(_a[2]))); break;
        case 11: _t->calibrationRequested((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3]))); break;
        case 12: _t->cancelRequested((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 13: _t->sleepRequested(); break;
        case 14: _t->wakeRequested(); break;
        case 15: _t->busEnabledRequested((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->flushControlSignal(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SlaveStatus >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SlaveConfigInfo >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BCMSignal >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BCMSignal >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SlaveConfigInfo >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AmbientLinScheduler::*_t)(SlaveStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::SlaveStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(quint32 , SlaveConfigInfo , bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::nodeConfigurationRead)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(quint32 , quint8 , bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::nodeConfigurationWritten)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(quint32 , quint8 , bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::calibrationFinished)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::busStateChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::stopRequested)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(BCMSignal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::controlSignalRequested)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(BCMSignal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::switchControlSignalRequested)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::signalPresetRequested)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(quint32 , quint8 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::readNodeRequested)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(quint32 , SlaveConfigInfo );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::writeNodeRequested)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(quint32 , quint8 , quint8 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::calibrationRequested)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(quint32 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::cancelRequested)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::sleepRequested)) {
                *result = 13;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::wakeRequested)) {
                *result = 14;
                return;
            }
        }
        {
            typedef void (AmbientLinScheduler::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AmbientLinScheduler::busEnabledRequested)) {
                *result = 15;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AmbientLinScheduler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AmbientLinScheduler.data,
      qt_meta_data_AmbientLinScheduler,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AmbientLinScheduler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AmbientLinScheduler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AmbientLinScheduler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AmbientLinScheduler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void AmbientLinScheduler::SlaveStatusChanged(SlaveStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AmbientLinScheduler::nodeConfigurationRead(quint32 _t1, SlaveConfigInfo _t2, bool _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AmbientLinScheduler::nodeConfigurationWritten(quint32 _t1, quint8 _t2, bool _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AmbientLinScheduler::calibrationFinished(quint32 _t1, quint8 _t2, bool _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AmbientLinScheduler::busStateChanged(bool _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AmbientLinScheduler::stopRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void AmbientLinScheduler::controlSignalRequested(BCMSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AmbientLinScheduler::switchControlSignalRequested(BCMSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AmbientLinScheduler::signalPresetRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AmbientLinScheduler::readNodeRequested(quint32 _t1, quint8 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void AmbientLinScheduler::writeNodeRequested(quint32 _t1, SlaveConfigInfo _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void AmbientLinScheduler::calibrationRequested(quint32 _t1, quint8 _t2, quint8 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void AmbientLinScheduler::cancelRequested(quint32 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void AmbientLinScheduler::sleepRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void AmbientLinScheduler::wakeRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void AmbientLinScheduler::busEnabledRequested(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
