#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_008317c8_ResetActionState, 0x00479120,
         (C_008317c8* self, void* edx, C_00862490* context, uint32_t flags));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall CCharActionFsm_ClearActionResources(
    CCharActionFsm* self, void* /*edx*/)
{
    for (uint32_t index = 0; ; ++index)
    {
        const uint32_t begin = self->field_0018;
        if (!begin ||
            index >= static_cast<uint32_t>(
                static_cast<int32_t>(self->field_001c - begin) >> 2))
            break;

        C_008317c8* action =
            reinterpret_cast<C_008317c8**>(begin)[index];
        if (action)
            C_008317c8_ResetActionState(action, nullptr, nullptr, 0);
    }

    for (uint32_t index = 0; ; ++index)
    {
        const uint32_t begin = self->field_0078;
        if (!begin ||
            index >= static_cast<uint32_t>(
                static_cast<int32_t>(self->field_007c - begin) >> 2))
            break;

        void* object = reinterpret_cast<void**>(begin)[index];
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            reinterpret_cast<void**>(self->field_0078)[index] = nullptr;
        }
    }

    if (self->field_0078)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0078));

    int result = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;

    void* object = reinterpret_cast<void*>(self->field_0084);
    if (object)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0084 = 0;
    }

    object = reinterpret_cast<void*>(self->field_0088);
    if (object)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0088 = 0;
    }

    object = reinterpret_cast<void*>(self->field_008c);
    if (object)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_008c = 0;
    }

    return result;
}

HOOK(0x00486B40, CCharActionFsm_ClearActionResources, "auto");
