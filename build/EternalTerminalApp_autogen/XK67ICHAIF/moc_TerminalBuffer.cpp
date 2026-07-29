/****************************************************************************
** Meta object code from reading C++ file 'TerminalBuffer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/terminal/TerminalBuffer.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TerminalBuffer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN14TerminalBufferE_t {};
} // unnamed namespace

template <> constexpr inline auto TerminalBuffer::qt_create_metaobjectdata<qt_meta_tag_ZN14TerminalBufferE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TerminalBuffer",
        "screenChanged",
        "",
        "cursorChanged",
        "scrollChanged",
        "resized",
        "rows",
        "columns",
        "characterAt",
        "row",
        "column",
        "scrollbackSize",
        "append",
        "data",
        "resizeTerminal",
        "totalRows",
        "visibleOffset",
        "bufferRowFromScreenRow",
        "screenRow",
        "scroll",
        "amount",
        "scrollOffset",
        "wordAt",
        "QVariantList",
        "selectedText",
        "TerminalSelection*",
        "selection",
        "cursorX",
        "cursorY"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'screenChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cursorChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'scrollChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'resized'
        QtMocHelpers::SignalData<void(int, int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Int, 7 },
        }}),
        // Method 'characterAt'
        QtMocHelpers::MethodData<QString(int, int) const>(8, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 9 }, { QMetaType::Int, 10 },
        }}),
        // Method 'scrollbackSize'
        QtMocHelpers::MethodData<int() const>(11, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'append'
        QtMocHelpers::MethodData<void(QString)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'resizeTerminal'
        QtMocHelpers::MethodData<void(int, int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Int, 7 },
        }}),
        // Method 'totalRows'
        QtMocHelpers::MethodData<int() const>(15, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'visibleOffset'
        QtMocHelpers::MethodData<int() const>(16, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'bufferRowFromScreenRow'
        QtMocHelpers::MethodData<int(int) const>(17, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'scroll'
        QtMocHelpers::MethodData<void(int)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 20 },
        }}),
        // Method 'scrollOffset'
        QtMocHelpers::MethodData<int() const>(21, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'wordAt'
        QtMocHelpers::MethodData<QVariantList(int, int) const>(22, 2, QMC::AccessPublic, 0x80000000 | 23, {{
            { QMetaType::Int, 9 }, { QMetaType::Int, 10 },
        }}),
        // Method 'selectedText'
        QtMocHelpers::MethodData<QString(TerminalSelection *) const>(24, 2, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 25, 26 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'cursorX'
        QtMocHelpers::PropertyData<int>(27, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'cursorY'
        QtMocHelpers::PropertyData<int>(28, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'rows'
        QtMocHelpers::PropertyData<int>(6, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'columns'
        QtMocHelpers::PropertyData<int>(7, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TerminalBuffer, qt_meta_tag_ZN14TerminalBufferE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TerminalBuffer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14TerminalBufferE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14TerminalBufferE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14TerminalBufferE_t>.metaTypes,
    nullptr
} };

void TerminalBuffer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TerminalBuffer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->screenChanged(); break;
        case 1: _t->cursorChanged(); break;
        case 2: _t->scrollChanged(); break;
        case 3: _t->resized((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 4: { QString _r = _t->characterAt((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { int _r = _t->scrollbackSize();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->append((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->resizeTerminal((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 8: { int _r = _t->totalRows();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->visibleOffset();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->bufferRowFromScreenRow((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->scroll((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: { int _r = _t->scrollOffset();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 13: { QVariantList _r = _t->wordAt((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 14: { QString _r = _t->selectedText((*reinterpret_cast<std::add_pointer_t<TerminalSelection*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TerminalBuffer::*)()>(_a, &TerminalBuffer::screenChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalBuffer::*)()>(_a, &TerminalBuffer::cursorChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalBuffer::*)()>(_a, &TerminalBuffer::scrollChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalBuffer::*)(int , int )>(_a, &TerminalBuffer::resized, 3))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->cursorX(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->cursorY(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->rows(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->columns(); break;
        default: break;
        }
    }
}

const QMetaObject *TerminalBuffer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TerminalBuffer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14TerminalBufferE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TerminalBuffer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TerminalBuffer::screenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TerminalBuffer::cursorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TerminalBuffer::scrollChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TerminalBuffer::resized(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}
QT_WARNING_POP
