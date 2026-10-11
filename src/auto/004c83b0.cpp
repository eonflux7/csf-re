#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_4BC2A0, 0x004BC2A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, sub_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

// Grows the dynamic array when count >= capacity (unsigned compare).
static void CoverAttack_EnsureCapacity(void* arr)
{
    uint32_t count = CSF_AT(uint32_t, arr, 8);
    uint32_t capacity = CSF_AT(uint32_t, arr, 4);
    if (count >= capacity) {
        GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xC));
    }
}

// Appends a value without capacity check (reloads count and data pointer).
static void CoverAttack_AppendRaw(void* arr, uint32_t value)
{
    uint32_t count = CSF_AT(uint32_t, arr, 8);
    uint32_t* data = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, arr, 0));
    uint32_t* slot = data + count;
    CSF_AT(uint32_t, arr, 8) = count + 1;
    *slot = value;
}

static int __stdcall CoverAttack_BuildActionSequence(void* owner)
{
    int linked = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    int base = linked ? linked - 4 : 0;
    sub_4DC180(owner, nullptr, base + 0x440);

    linked = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    int target = linked ? linked - 4 : 0;

    linked = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    int src = linked ? linked - 4 : 0;

    int arg;
    if (CSF_AT(int, reinterpret_cast<void*>(src), 0x630) < 0)
        arg = src + 0x610;
    else
        arg = src + 0x628;

    void* arr = reinterpret_cast<void*>(target + 0x50C);
    sub_4C5150(arr, nullptr, arg);

    int global = GetGlobalState_50D190();
    int state = GetStateValue(reinterpret_cast<void*>(global), nullptr, 2);

    float p1, p2;
    if (state == 0) {
        CoverAttack_EnsureCapacity(arr);
        p1 = 2.0f;
        p2 = 4.0f;
    } else {
        CoverAttack_EnsureCapacity(arr);
        CoverAttack_AppendRaw(arr, 13);
        sub_4BC3A0(arr, nullptr, 1.0f, 3.0f);
        CoverAttack_EnsureCapacity(arr);
        CoverAttack_AppendRaw(arr, 14);
        CoverAttack_EnsureCapacity(arr);
        p1 = 1.0f;
        p2 = 3.0f;
    }

    CoverAttack_AppendRaw(arr, 15);
    sub_4BC3A0(arr, nullptr, p1, p2);
    CoverAttack_EnsureCapacity(arr);
    CoverAttack_AppendRaw(arr, 16);
    return sub_4BC2A0(arr, nullptr, 1065353216);
}

HOOK(0x004C83B0, CoverAttack_BuildActionSequence, "auto");
