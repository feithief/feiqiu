/****************************************************************************
** Meta object code from reading C++ file 'linbusworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../feiqiu-main/feiqiu-main/LIN10_ColorPalette_Host/linbusworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'linbusworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LinBusWorker_t {
    QByteArrayData data[37];
    char stringdata0[469];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LinBusWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LinBusWorker_t qt_meta_stringdata_LinBusWorker = {
    {
QT_MOC_LITERAL(0, 0, 12), // "LinBusWorker"
QT_MOC_LITERAL(1, 13, 18), // "slaveStatusChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 11), // "SlaveStatus"
QT_MOC_LITERAL(4, 45, 6), // "status"
QT_MOC_LITERAL(5, 52, 21), // "nodeConfigurationRead"
QT_MOC_LITERAL(6, 74, 9), // "requestId"
QT_MOC_LITERAL(7, 84, 15), // "SlaveConfigInfo"
QT_MOC_LITERAL(8, 100, 4), // "info"
QT_MOC_LITERAL(9, 105, 7), // "success"
QT_MOC_LITERAL(10, 113, 12), // "errorMessage"
QT_MOC_LITERAL(11, 126, 24), // "nodeConfigurationWritten"
QT_MOC_LITERAL(12, 151, 4), // "node"
QT_MOC_LITERAL(13, 156, 19), // "calibrationFinished"
QT_MOC_LITERAL(14, 176, 15), // "busStateChanged"
QT_MOC_LITERAL(15, 192, 5), // "ready"
QT_MOC_LITERAL(16, 198, 7), // "message"
QT_MOC_LITERAL(17, 206, 7), // "stopped"
QT_MOC_LITERAL(18, 214, 10), // "initialize"
QT_MOC_LITERAL(19, 225, 10), // "stopWorker"
QT_MOC_LITERAL(20, 236, 19), // "updateControlSignal"
QT_MOC_LITERAL(21, 256, 9), // "BCMSignal"
QT_MOC_LITERAL(22, 266, 6), // "signal"
QT_MOC_LITERAL(23, 273, 19), // "switchControlSignal"
QT_MOC_LITERAL(24, 293, 17), // "applySignalPreset"
QT_MOC_LITERAL(25, 311, 11), // "presetIndex"
QT_MOC_LITERAL(26, 323, 15), // "enqueueReadNode"
QT_MOC_LITERAL(27, 339, 16), // "enqueueWriteNode"
QT_MOC_LITERAL(28, 356, 18), // "enqueueCalibration"
QT_MOC_LITERAL(29, 375, 4), // "mode"
QT_MOC_LITERAL(30, 380, 13), // "cancelRequest"
QT_MOC_LITERAL(31, 394, 8), // "sleepBus"
QT_MOC_LITERAL(32, 403, 7), // "wakeBus"
QT_MOC_LITERAL(33, 411, 13), // "setBusEnabled"
QT_MOC_LITERAL(34, 425, 7), // "enabled"
QT_MOC_LITERAL(35, 433, 19), // "processScheduleSlot"
QT_MOC_LITERAL(36, 453, 15) // "processTaskStep"

    },
    "LinBusWorker\0slaveStatusChanged\0\0"
    "SlaveStatus\0status\0nodeConfigurationRead\0"
    "requestId\0SlaveConfigInfo\0info\0success\0"
    "errorMessage\0nodeConfigurationWritten\0"
    "node\0calibrationFinished\0busStateChanged\0"
    "ready\0message\0stopped\0initialize\0"
    "stopWorker\0updateControlSignal\0BCMSignal\0"
    "signal\0switchControlSignal\0applySignalPreset\0"
    "presetIndex\0enqueueReadNode\0"
    "enqueueWriteNode\0enqueueCalibration\0"
    "mode\0cancelRequest\0sleepBus\0wakeBus\0"
    "setBusEnabled\0enabled\0processScheduleSlot\0"
    "processTaskStep"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LinBusWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  114,    2, 0x06 /* Public */,
       5,    4,  117,    2, 0x06 /* Public */,
      11,    4,  126,    2, 0x06 /* Public */,
      13,    4,  135,    2, 0x06 /* Public */,
      14,    2,  144,    2, 0x06 /* Public */,
      17,    0,  149,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    0,  150,    2, 0x0a /* Public */,
      19,    0,  151,    2, 0x0a /* Public */,
      20,    1,  152,    2, 0x0a /* Public */,
      23,    1,  155,    2, 0x0a /* Public */,
      24,    1,  158,    2, 0x0a /* Public */,
      26,    2,  161,    2, 0x0a /* Public */,
      27,    2,  166,    2, 0x0a /* Public */,
      28,    3,  171,    2, 0x0a /* Public */,
      30,    1,  178,    2, 0x0a /* Public */,
      31,    0,  181,    2, 0x0a /* Public */,
      32,    0,  182,    2, 0x0a /* Public */,
      33,    1,  183,    2, 0x0a /* Public */,
      35,    0,  186,    2, 0x08 /* Private */,
      36,    0,  187,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 7, QMetaType::Bool, QMetaType::QString,    6,    8,    9,   10,
    QMetaType::Void, QMetaType::UInt, QMetaType::UChar, QMetaType::Bool, QMetaType::QString,    6,   12,    9,   10,
    QMetaType::Void, QMetaType::UInt, QMetaType::UChar, QMetaType::Bool, QMetaType::QString,    6,   12,    9,   10,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   15,   16,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::UInt, QMetaType::UChar,    6,   12,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 7,    6,    8,
    QMetaType::Void, QMetaType::UInt, QMetaType::UChar, QMetaType::UChar,    6,   12,   29,
    QMetaType::Void, QMetaType::UInt,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   34,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LinBusWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LinBusWorker *_t = static_cast<LinBusWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slaveStatusChanged((*reinterpret_cast< SlaveStatus(*)>(_a[1]))); break;
        case 1: _t->nodeConfigurationRead((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< SlaveConfigInfo(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 2: _t->nodeConfigurationWritten((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 3: _t->calibrationFinished((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 4: _t->busStateChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->stopped(); break;
        case 6: _t->initialize(); break;
        case 7: _t->stopWorker(); break;
        case 8: _t->updateControlSignal((*reinterpret_cast< BCMSignal(*)>(_a[1]))); break;
        case 9: _t->switchControlSignal((*reinterpret_cast< BCMSignal(*)>(_a[1]))); break;
        case 10: _t->applySignalPreset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->enqueueReadNode((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 12: _t->enqueueWriteNode((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< SlaveConfigInfo(*)>(_a[2]))); break;
        case 13: _t->enqueueCalibration((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3]))); break;
        case 14: _t->cancelRequest((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 15: _t->sleepBus(); break;
        case 16: _t->wakeBus(); break;
        case 17: _t->setBusEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->processScheduleSlot(); break;
        case 19: _t->processTaskStep(); break;
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
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BCMSignal >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BCMSignal >(); break;
            }
            break;
        case 12:
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
            typedef void (LinBusWorker::*_t)(SlaveStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LinBusWorker::slaveStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (LinBusWorker::*_t)(quint32 , SlaveConfigInfo , bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LinBusWorker::nodeConfigurationRead)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (LinBusWorker::*_t)(quint32 , quint8 , bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LinBusWorker::nodeConfigurationWritten)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (LinBusWorker::*_t)(quint32 , quint8 , bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LinBusWorker::calibrationFinished)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (LinBusWorker::*_t)(bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LinBusWorker::busStateChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (LinBusWorker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LinBusWorker::stopped)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LinBusWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LinBusWorker.data,
      qt_meta_data_LinBusWorker,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LinBusWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LinBusWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LinBusWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LinBusWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void LinBusWorker::slaveStatusChanged(SlaveStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LinBusWorker::nodeConfigurationRead(quint32 _t1, SlaveConfigInfo _t2, bool _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LinBusWorker::nodeConfigurationWritten(quint32 _t1, quint8 _t2, bool _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void LinBusWorker::calibrationFinished(quint32 _t1, quint8 _t2, bool _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void LinBusWorker::busStateChanged(bool _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void LinBusWorker::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
