#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, Controller_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ArrayAppendRef_4B7150, 0x004B7150, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, ArraySetRange_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, ArrayAssignFrom_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, Task_4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Returns the owning object (linked value minus 4), or null.
static uint8_t* GetOwnerObject(void* task)
{
    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linked ? reinterpret_cast<uint8_t*>(linked - 4) : nullptr;
}

static int __stdcall CoverAttack_InitParapetAttack(void* task)
{
    uint8_t* owner = GetOwnerObject(task);
    Controller_492720(CSF_AT(void*, owner, 0x2c), nullptr, 1);

    owner = GetOwnerObject(task);
    void* ctrl = CSF_AT(void*, owner, 0x2c);
    void** vt = CSF_AT(void**, ctrl, 0);
    ((void(__fastcall*)(void*, void*, int, int, int, int))vt[0x8c / 4])(ctrl, nullptr, 0, 0, -1, 0);

    owner = GetOwnerObject(task);
    Task_4DC180(task, nullptr, (int)(reinterpret_cast<uintptr_t>(owner) + 0x440));

    uint8_t* base = GetOwnerObject(task);

    uint8_t* o = GetOwnerObject(task);
    uint8_t* sel;
    if (CSF_AT(int, o, 0x630) < 0)
        sel = o + 0x610;
    else
        sel = o + 0x628;

    uint8_t* arr;
    if (CSF_AT(int, sel, 8) < 0) {
        uint8_t* o2 = GetOwnerObject(task);
        uint32_t size = CSF_AT(uint32_t, base, 0x514);
        uint32_t cap = CSF_AT(uint32_t, base, 0x510);
        arr = base + 0x50c;
        if (size >= cap)
            GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xc));
        uint32_t count = CSF_AT(uint32_t, arr, 8);
        uint32_t* slot = reinterpret_cast<uint32_t*>(CSF_AT(uintptr_t, arr, 0) + count * 4);
        CSF_AT(uint32_t, arr, 8) = count + 1;
        *slot = 4;
        ArrayAppendRef_4B7150(arr, nullptr, reinterpret_cast<uint32_t*>(o2 + 0x64c));
    } else {
        uint8_t* o2 = GetOwnerObject(task);
        arr = base + 0x50c;
        if (CSF_AT(int, o2, 0x630) < 0)
            ArrayAssignFrom_4C5150(arr, nullptr, (int)(reinterpret_cast<uintptr_t>(o2) + 0x610));
        else
            ArrayAssignFrom_4C5150(arr, nullptr, (int)(reinterpret_cast<uintptr_t>(o2) + 0x628));
    }

    ArraySetRange_4BC3A0(arr, nullptr, 1.0f, 2.0f);

    uint8_t* first = GetOwnerObject(task);
    uint8_t* second = GetOwnerObject(task);
    void* charTask = CSF_AT(void*, CSF_AT(void*, second, 0x2c), 0x204);
    return TaskCharAction_48CFE0(charTask, nullptr, (int)reinterpret_cast<uintptr_t>(first), 5, 1);
}

HOOK(0x004C5EC0, CoverAttack_InitParapetAttack, "auto");
