#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCore_GetSceneEntry, 0x0040F790, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCore_GetSceneEntryCount, 0x0052E420, (void* self, void* edx));

static int __fastcall C_0083b850_HandleSceneCommand(
    C_0083b850* self, void* /*edx*/, int* command)
{
    int result = command[0];
    if (result != 0)
    {
        if (result == 38 && self->field_0070 == 1)
        {
            void* owner = reinterpret_cast<void*>(self->field_0014);
            void* controller = CSF_AT(void*, owner, 0x30);
            result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[0xA0 / 4])(controller, nullptr);
            self->field_0070 = 2;
            self->field_0018 &= ~1u;
        }
        return result;
    }

    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* entries = CSF_AT(void*, owner, 0x4C);
    for (int index = 0; index < AiCore_GetSceneEntryCount(entries, nullptr); ++index)
    {
        void* entry = reinterpret_cast<void*>(
            AiCore_GetSceneEntry(entries, nullptr, index));
        if (CSF_AT(uint8_t, entry, 0x0C) != 0)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, entries, 0)[0x4C / 4])(entries, nullptr, index);
        }
    }

    self->field_0018 |= 1u;
    self->field_0074 = command[1];
    CSF_AT(uint32_t, self, 0x78) = command[2];
    CSF_AT(uint32_t, self, 0x7C) = command[3];

    if (self->field_0070 == 1)
    {
        owner = reinterpret_cast<void*>(self->field_0014);
        void* controller = CSF_AT(void*, owner, 0x30);
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0xA0 / 4])(controller, nullptr);
    }

    owner = reinterpret_cast<void*>(self->field_0014);
    self->field_0070 = 1;
    void* controller = CSF_AT(void*, owner, 0x30);
    return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
        CSF_AT(void**, controller, 0)[0xAC / 4])(
            controller, nullptr, &self->field_0074, 1);
}

HOOK(0x004D0F40, C_0083b850_HandleSceneCommand, "auto");
