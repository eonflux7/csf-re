#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, CoverTask_SetFloatParam, 0x004BC2A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoverTask_SetRange, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, CoverTask_AddEntry, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverTask_Setup, 0x004DC180, (void* self, void* edx, int a2));

static int CoverTask_GetOwnerBase(void* task)
{
    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    if (linked)
        return linked - 4;
    return 0;
}

static void CoverTask_PushCommand(S_0041e5f0* arr, uint32_t value)
{
    if (arr->field_0008 >= arr->field_0004)
        GrowButtonArray(arr, nullptr, static_cast<int>(arr->field_000c));
    uint32_t index = arr->field_0008;
    uint32_t* slot = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(arr->field_0000)) + index;
    arr->field_0008 = index + 1;
    *slot = value;
}

static int __stdcall CoverTask_InitParapetAttack(void* task)
{
    int base = CoverTask_GetOwnerBase(task);
    CoverTask_Setup(task, nullptr, base + 0x440);

    int arrBase = CoverTask_GetOwnerBase(task);
    int obj = CoverTask_GetOwnerBase(task);
    int entry;
    if (CSF_AT(int, reinterpret_cast<void*>(obj), 0x630) < 0)
        entry = obj + 0x610;
    else
        entry = obj + 0x628;

    S_0041e5f0* arr = reinterpret_cast<S_0041e5f0*>(arrBase + 0x50c);
    CoverTask_AddEntry(arr, nullptr, entry);

    CoverTask_PushCommand(arr, 0);
    CoverTask_PushCommand(arr, 31);
    CoverTask_PushCommand(arr, 15);
    CoverTask_SetRange(arr, nullptr, 2.0f, 4.0f);
    CoverTask_PushCommand(arr, 16);
    CoverTask_SetFloatParam(arr, nullptr, 0x3f800000);

    int result = GetLinkedObjectValue_4DB5C0(task, nullptr);
    if (result) {
        CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x6c8) = 0;
        return result;
    }
    CSF_AT(uint8_t, reinterpret_cast<void*>(0), 0x6cc) = 0;
    return 0;
}

HOOK(0x004C9020, CoverTask_InitParapetAttack, "auto");
