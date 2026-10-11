#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CommandArray_AddParams, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, CommandArray_Init, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, Task_SetObject, 0x004DC180, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Appends a value to the dynamic dword array, growing it when full. Returns the new count.
static uint32_t CommandArray_Push(S_0041e5f0* arr, uint32_t value)
{
    if (arr->field_0008 >= arr->field_0004)
        GrowButtonArray(arr, nullptr, static_cast<int>(arr->field_000c));
    uint32_t index = arr->field_0008;
    uint32_t* slot = reinterpret_cast<uint32_t*>(arr->field_0000) + index;
    arr->field_0008 = index + 1;
    *slot = value;
    return index + 1;
}

static uint32_t __stdcall CoverAttack_SetupParapetTask(int a1)
{
    void* task = reinterpret_cast<void*>(a1);

    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    int base = linked ? linked - 4 : 0;
    Task_SetObject(task, nullptr, base + 0x440);

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    uint8_t* obj = reinterpret_cast<uint8_t*>(linked ? linked - 4 : 0);

    uint32_t menuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (menuContext)
        CommandArray_Init(obj + 0x50C, nullptr, static_cast<int>(menuContext + 0x1C));

    S_0041e5f0* arr = reinterpret_cast<S_0041e5f0*>(obj + 0x50C);

    CommandArray_Push(arr, 31);
    CommandArray_Push(arr, 13);
    CommandArray_AddParams(arr, nullptr, 1.0f, 5.0f);
    return CommandArray_Push(arr, 14);
}

HOOK(0x004C5D60, CoverAttack_SetupParapetTask, "auto");
