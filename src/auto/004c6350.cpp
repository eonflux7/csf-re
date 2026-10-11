#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, sub_4BC6B0, 0x004BC6B0, (void* self, void* edx, float a2, int a3));
CSF_FUNC(int, __fastcall, sub_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

// Returns the owner object (linked object minus 4), or 0 when there is none.
static uint8_t* GetOwnerObject(int task)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(task), nullptr);
    if (linked)
        return reinterpret_cast<uint8_t*>(linked - 4);
    return nullptr;
}

// Grows the dynamic int array (data +0, capacity +4, count +8, growth +0xC) if full.
static void EnsureArrayCapacity(uint8_t* arr)
{
    if (CSF_AT(uint32_t, arr, 8) >= CSF_AT(uint32_t, arr, 4))
        GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xC));
}

// Appends a value; the capacity check is done separately by the caller.
static int PushArrayValue(uint8_t* arr, int value)
{
    int count = CSF_AT(int, arr, 8);
    int* slot = CSF_AT(int*, arr, 0) + count;
    CSF_AT(int, arr, 8) = count + 1;
    *slot = value;
    return count + 1;
}

static int __stdcall CoverAttackTask_BuildParapetSequence(int a1)
{
    uint8_t* owner = GetOwnerObject(a1);
    sub_4DC180(reinterpret_cast<void*>(a1), nullptr,
               static_cast<int>(reinterpret_cast<intptr_t>(owner)) + 0x440);

    uint8_t* base = GetOwnerObject(a1);

    uint8_t* owner2 = GetOwnerObject(a1);
    int arg;
    if (CSF_AT(int, owner2, 0x630) < 0)
        arg = static_cast<int>(reinterpret_cast<intptr_t>(owner2)) + 0x610;
    else
        arg = static_cast<int>(reinterpret_cast<intptr_t>(owner2)) + 0x628;

    uint8_t* arr = base + 0x50C;
    sub_4C5150(arr, nullptr, arg);

    EnsureArrayCapacity(arr);
    PushArrayValue(arr, 5);

    EnsureArrayCapacity(arr);
    PushArrayValue(arr, 10);

    int state = GetGlobalState_50D190();
    int value = GetStateValue(reinterpret_cast<void*>(state), nullptr, 2);

    float dir;
    if (value == 0) {
        EnsureArrayCapacity(arr);
        dir = 1.0f;
    } else {
        EnsureArrayCapacity(arr);
        dir = -1.0f;
    }
    PushArrayValue(arr, 13);
    sub_4BC6B0(arr, nullptr, dir, 0x44480000);
    sub_4BC3A0(arr, nullptr, 2.0f, 5.0f);

    EnsureArrayCapacity(arr);
    return PushArrayValue(arr, 14);
}

HOOK(0x004C6350, CoverAttackTask_BuildParapetSequence, "auto");
