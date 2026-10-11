#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, TaskArray_SetupBegin, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, TaskArray_AppendSource, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, Owner_ApplyOffset, 0x004DC180, (void* self, void* edx, int a2));

static uint8_t* GetLinkedOwnerBase(void* link)
{
    int value = GetLinkedObjectValue_4DB5C0(link, nullptr);
    if (value)
        return reinterpret_cast<uint8_t*>(value - 4);
    return nullptr;
}

// Returns the incremented count (as held in EAX by the original).
static int TaskArray_Push(uint8_t* arr, int value)
{
    if (CSF_AT(uint32_t, arr, 8) >= CSF_AT(uint32_t, arr, 4))
        GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xC));
    int index = CSF_AT(int, arr, 8);
    int* slot = reinterpret_cast<int*>(CSF_AT(uint8_t*, arr, 0) + index * 4);
    int newCount = index + 1;
    CSF_AT(int, arr, 8) = newCount;
    *slot = value;
    return newCount;
}

static int __stdcall CoverAttack_QueueParapetTasks(int a1)
{
    void* link = reinterpret_cast<void*>(a1);

    uint8_t* owner = GetLinkedOwnerBase(link);
    Owner_ApplyOffset(link, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(owner)) + 0x440);

    uint8_t* base = GetLinkedOwnerBase(link);

    uint8_t* src = GetLinkedOwnerBase(link);
    int srcPtr;
    if (CSF_AT(int, src, 0x630) < 0)
        srcPtr = static_cast<int>(reinterpret_cast<intptr_t>(src)) + 0x610;
    else
        srcPtr = static_cast<int>(reinterpret_cast<intptr_t>(src)) + 0x628;

    uint8_t* arr = base + 0x50C;
    TaskArray_AppendSource(arr, nullptr, srcPtr);

    TaskArray_Push(arr, 13);
    TaskArray_SetupBegin(arr, nullptr, 1.0f, 4.0f);
    int result = TaskArray_Push(arr, 14);

    return result;
}

HOOK(0x004C5B80, CoverAttack_QueueParapetTasks, "auto");
