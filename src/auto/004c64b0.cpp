#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, TaskSetupBc3a0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, TaskSetupC5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, TaskSetupDC180, 0x004DC180, (void* self, void* edx, int a2));

static int GetLinkedObjectBase(void* task)
{
    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linked ? linked - 4 : 0;
}

// Appends a value to the dword array stored at arr (data, capacity, count, growth).
static int ArrayPushBack(uint8_t* arr, int value)
{
    if (CSF_AT(uint32_t, arr, 8) >= CSF_AT(uint32_t, arr, 4)) {
        GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xC));
    }
    int count = CSF_AT(int, arr, 8);
    int* slot = reinterpret_cast<int*>(CSF_AT(uint8_t*, arr, 0) + count * 4);
    CSF_AT(int, arr, 8) = count + 1;
    *slot = value;
    return count + 1;
}

static int __stdcall CoverAttackTask_SetupActionList(void* task)
{
    int base0 = GetLinkedObjectBase(task);
    TaskSetupDC180(task, nullptr, base0 + 0x440);

    int base1 = GetLinkedObjectBase(task);

    int base2 = GetLinkedObjectBase(task);
    int arg;
    if (CSF_AT(int, reinterpret_cast<void*>(base2), 0x630) < 0)
        arg = base2 + 0x610;
    else
        arg = base2 + 0x628;

    uint8_t* arr = reinterpret_cast<uint8_t*>(base1) + 0x50C;
    TaskSetupC5150(arr, nullptr, arg);

    ArrayPushBack(arr, 5);
    ArrayPushBack(arr, 10);
    ArrayPushBack(arr, 13);
    ArrayPushBack(arr, 26);
    TaskSetupBc3a0(arr, nullptr, 2.0f, 5.0f);
    return ArrayPushBack(arr, 14);
}

HOOK(0x004C64B0, CoverAttackTask_SetupActionList, "auto");
