#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, UnnamedSetLimits_4BC6B0, 0x004BC6B0, (void* self, void* edx, float a2, int a3));
CSF_FUNC(int, __fastcall, UnnamedAssign_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Returns the owner object (linked value minus 4), or 0 when there is none.
static int GetLinkedOwner(void* self)
{
    int linked = GetLinkedObjectValue_4DB5C0(self, nullptr);
    return linked ? linked - 4 : 0;
}

// Appends one dword to the dynamic array {data, capacity, count, growth}; returns the new count.
static int PushBackDword(void* arr, int value)
{
    if (CSF_AT(uint32_t, arr, 8) >= CSF_AT(uint32_t, arr, 4))
        GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xC));
    int count = CSF_AT(int, arr, 8);
    int* slot = reinterpret_cast<int*>(CSF_AT(uint32_t, arr, 0) + count * 4);
    CSF_AT(int, arr, 8) = count + 1;
    *slot = value;
    return count + 1;
}

static int __stdcall CoverAttack_BuildMoveStepList(void* a1)
{
    int owner = GetLinkedOwner(a1);
    UnnamedApply_4DC180(a1, nullptr, owner + 0x440);

    int base = GetLinkedOwner(a1);

    int src = GetLinkedOwner(a1);
    int srcArg;
    if (CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(src)), 0x630) < 0)
        srcArg = src + 0x610;
    else
        srcArg = src + 0x628;

    void* arr = reinterpret_cast<void*>(static_cast<uintptr_t>(base + 0x50C));
    UnnamedAssign_4C5150(arr, nullptr, srcArg);

    PushBackDword(arr, 5);
    PushBackDword(arr, 10);
    PushBackDword(arr, 13);

    UnnamedSetLimits_4BC6B0(arr, nullptr, 1.0f, 0x44480000);
    UnnamedSetRangeParams_4BC3A0(arr, nullptr, 2.0f, 5.0f);

    return PushBackDword(arr, 14);
}

HOOK(0x004C6760, CoverAttack_BuildMoveStepList, "auto");
