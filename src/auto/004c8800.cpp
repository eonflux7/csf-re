#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, UnknownTask_4BC2A0, 0x004BC2A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnknownTask_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, UnknownTask_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnknownTask_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static uint8_t* GetLinkedOwner(int a1)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a1), nullptr);
    if (linked)
        return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linked - 4));
    return nullptr;
}

static void PushBackValue(uint8_t* arr, int value)
{
    if (CSF_AT(uint32_t, arr, 8) >= CSF_AT(uint32_t, arr, 4))
        GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xC));
    uint32_t count = CSF_AT(uint32_t, arr, 8);
    int* slot = CSF_AT(int*, arr, 0) + count;
    CSF_AT(uint32_t, arr, 8) = count + 1;
    *slot = value;
}

static int __stdcall CoverAttack_SetupParapetTaskList(int a1)
{
    uint8_t* owner = GetLinkedOwner(a1);
    UnknownTask_4DC180(reinterpret_cast<void*>(a1), nullptr,
                       static_cast<int>(reinterpret_cast<uintptr_t>(owner) + 0x440));

    uint8_t* base = GetLinkedOwner(a1);

    uint8_t* src = GetLinkedOwner(a1);
    int srcField;
    if (CSF_AT(int, src, 0x630) < 0)
        srcField = static_cast<int>(reinterpret_cast<uintptr_t>(src) + 0x610);
    else
        srcField = static_cast<int>(reinterpret_cast<uintptr_t>(src) + 0x628);

    uint8_t* arr = base + 0x50C;
    UnknownTask_4C5150(arr, nullptr, srcField);

    PushBackValue(arr, 5);
    PushBackValue(arr, 10);
    PushBackValue(arr, 31);
    PushBackValue(arr, 13);
    PushBackValue(arr, 26);
    UnknownTask_4BC3A0(arr, nullptr, 1.0f, 4.0f);
    PushBackValue(arr, 14);
    return UnknownTask_4BC2A0(arr, nullptr, 0x3F800000);
}

HOOK(0x004C8800, CoverAttack_SetupParapetTaskList, "auto");
