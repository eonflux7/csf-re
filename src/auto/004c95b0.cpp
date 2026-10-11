#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, CoverTask_SetValue4BC2A0, 0x004BC2A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoverTask_SetRange4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, CoverTask_Init4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverTask_Update4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Returns the owner object (linked value - 4) or 0.
static uint8_t* CoverTask_GetOwner(void* task)
{
    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    if (linked)
        return reinterpret_cast<uint8_t*>(linked - 4);
    return nullptr;
}

// Appends one dword to the dynamic array (data +0, capacity +4, count +8, growth +0xC).
static void CoverTask_PushValue(uint8_t* arr, int value)
{
    if (static_cast<uint32_t>(CSF_AT(int, arr, 8)) >= static_cast<uint32_t>(CSF_AT(int, arr, 4)))
        GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xC));
    int count = CSF_AT(int, arr, 8);
    int* slot = reinterpret_cast<int*>(CSF_AT(int, arr, 0) + 4 * count);
    CSF_AT(int, arr, 8) = count + 1;
    *slot = value;
}

static int __stdcall CoverTask_QueueCoverActions(void* task)
{
    uint8_t* owner = CoverTask_GetOwner(task);
    CoverTask_Update4DC180(task, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(owner)) + 0x440);

    uint8_t* target = CoverTask_GetOwner(task);
    uint8_t* src = CoverTask_GetOwner(task);

    int srcAddr;
    if (CSF_AT(int, src, 0x630) < 0)
        srcAddr = static_cast<int>(reinterpret_cast<uintptr_t>(src)) + 0x610;
    else
        srcAddr = static_cast<int>(reinterpret_cast<uintptr_t>(src)) + 0x628;

    uint8_t* arr = target + 0x50C;
    CoverTask_Init4C5150(arr, nullptr, srcAddr);

    CoverTask_PushValue(arr, 31);
    CoverTask_PushValue(arr, 13);
    CoverTask_SetRange4BC3A0(arr, nullptr, 1.0f, 3.0f);
    CoverTask_PushValue(arr, 14);
    return CoverTask_SetValue4BC2A0(arr, nullptr, 1065353216);
}

HOOK(0x004C95B0, CoverTask_QueueCoverActions, "auto");
