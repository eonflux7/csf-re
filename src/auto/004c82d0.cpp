#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_4BC2A0, 0x004BC2A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, sub_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static uint8_t* AttackFromPosition_GetOwner(int a1)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a1), nullptr);
    if (linked)
        return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linked - 4));
    return nullptr;
}

static void AttackFromPosition_PushCommand(uint8_t* arr, int command)
{
    uint32_t count = CSF_AT(uint32_t, arr, 8);
    if (count >= CSF_AT(uint32_t, arr, 4))
        GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xC));
    count = CSF_AT(uint32_t, arr, 8);
    int* data = CSF_AT(int*, arr, 0);
    CSF_AT(uint32_t, arr, 8) = count + 1;
    data[count] = command;
}

static int __stdcall AttackFromPosition_BuildCoverSequence(int a1)
{
    uint8_t* owner = AttackFromPosition_GetOwner(a1);
    sub_4DC180(reinterpret_cast<void*>(a1), nullptr,
               static_cast<int>(reinterpret_cast<uintptr_t>(owner + 0x440)));

    uint8_t* target = AttackFromPosition_GetOwner(a1);

    uint8_t* src = AttackFromPosition_GetOwner(a1);
    uint8_t* srcField;
    if (CSF_AT(int, src, 0x630) < 0)
        srcField = src + 0x610;
    else
        srcField = src + 0x628;

    uint8_t* arr = target + 0x50C;
    sub_4C5150(arr, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(srcField)));

    AttackFromPosition_PushCommand(arr, 13);
    sub_4BC3A0(arr, nullptr, 1.0f, 3.0f);

    AttackFromPosition_PushCommand(arr, 14);
    return sub_4BC2A0(arr, nullptr, 1065353216);
}

HOOK(0x004C82D0, AttackFromPosition_BuildCoverSequence, "auto");
