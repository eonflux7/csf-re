#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, CoverTask_SetMode_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoverTask_AppendTarget_4B7150, 0x004B7150, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, CoverTask_Finish_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, CoverTask_AppendPoint_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverTask_Apply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Returns the owning object (linked value - 4) or 0.
static uint8_t* GetOwner(void* task)
{
    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linked ? reinterpret_cast<uint8_t*>(linked - 4) : nullptr;
}

static void PushCommand(uint8_t* arr, uint32_t value)
{
    if (CSF_AT(uint32_t, arr, 8) >= CSF_AT(uint32_t, arr, 4))
        GrowButtonArray(arr, nullptr, (int)CSF_AT(uint32_t, arr, 0xC));
    uint32_t count = CSF_AT(uint32_t, arr, 8);
    uint32_t* slot = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, arr, 0) + 4 * count);
    CSF_AT(uint32_t, arr, 8) = count + 1;
    *slot = value;
}

static int __stdcall CoverAttackTasks_QueueParapetMove(int a1)
{
    void* task = reinterpret_cast<void*>(a1);

    uint8_t* owner = GetOwner(task);
    void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x2C));
    CoverTask_SetMode_492720(obj, nullptr, 2);

    owner = GetOwner(task);
    void* inner = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x2C));
    void** vtbl = CSF_AT(void**, inner, 0);
    ((void(__fastcall*)(void*, void*, int, int, int, int))vtbl[0x8C / 4])(inner, nullptr, 0, 0, -1, 0);

    owner = GetOwner(task);
    CoverTask_Apply_4DC180(task, nullptr, (int)(reinterpret_cast<uintptr_t>(owner) + 0x440));

    uint8_t* base = GetOwner(task);

    uint8_t* o = GetOwner(task);
    uint8_t* sel = (CSF_AT(int, o, 0x630) < 0) ? (o + 0x610) : (o + 0x628);

    uint8_t* arr;
    if (CSF_AT(int, sel, 8) < 0)
    {
        uint8_t* o2 = GetOwner(task);
        arr = base + 0x50C;
        PushCommand(arr, 4);
        uint32_t* extra = reinterpret_cast<uint32_t*>(o2 + 0x64C);
        CoverTask_AppendTarget_4B7150(arr, nullptr, extra);
    }
    else
    {
        uint8_t* o2 = GetOwner(task);
        arr = base + 0x50C;
        if (CSF_AT(int, o2, 0x630) < 0)
            CoverTask_AppendPoint_4C5150(arr, nullptr, (int)(reinterpret_cast<uintptr_t>(o2) + 0x610));
        else
            CoverTask_AppendPoint_4C5150(arr, nullptr, (int)(reinterpret_cast<uintptr_t>(o2) + 0x628));
    }

    PushCommand(arr, 0x1F);
    return CoverTask_Finish_4BC3A0(arr, nullptr, 0.5f, 1.0f);
}

HOOK(0x004C6050, CoverAttackTasks_QueueParapetMove, "auto");
