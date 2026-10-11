#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GameConfig_IsStateCodeAllowed, 0x00755810, (std::uint32_t stateCode));

static int __fastcall S_0048f360_ResetState(S_0048f360* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(self->field_0024);
    if (child)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[156 / 4])(child, nullptr);

    child = reinterpret_cast<void*>(self->field_0028);
    if (child)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[160 / 4])(child, nullptr);

    child = reinterpret_cast<void*>(self->field_002c);
    if (child)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[128 / 4])(child, nullptr);

    child = reinterpret_cast<void*>(self->field_0030);
    if (child)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[148 / 4])(child, nullptr);

    child = reinterpret_cast<void*>(self->field_0034);
    if (child)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[128 / 4])(child, nullptr);

    child = reinterpret_cast<void*>(self->field_0038);
    if (child)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[128 / 4])(child, nullptr);

    child = reinterpret_cast<void*>(self->field_003c);
    if (child)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[40 / 4])(child, nullptr);

    child = reinterpret_cast<void*>(self->field_0040);
    if (child)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[40 / 4])(child, nullptr);

    self->field_0164 = 0;
    self->field_0164 = 1;
    self->field_0164 = 17;
    self->field_0164 = 21;
    self->field_0164 = 277;
    self->field_0164 = 789;
    self->field_0164 = 917;
    self->field_0164 = 2965;

    const bool stateAllowed =
        static_cast<std::uint8_t>(GameConfig_IsStateCodeAllowed(self->field_0044)) != 0;
    if (stateAllowed)
        self->field_0164 |= 0x2000;
    else
        self->field_0164 &= 0xFFFFDFFF;

    const std::uint32_t initialCount = self->field_00a8;
    self->field_0088 = 0;
    self->field_0074 = -1;
    self->field_0169 = 0;

    if (initialCount)
    {
        std::uint32_t index = 0;
        do
        {
            reinterpret_cast<std::uint8_t*>(self->field_00a0)[index] = 0;
            const std::uint32_t count = self->field_00a8;
            ++index;
            if (index >= count)
                break;
        } while (true);
    }

    void* owner = reinterpret_cast<std::uint8_t*>(self) - 12;
    void** ownerVtable = CSF_AT(void**, owner, 0);
    self->field_00b0 = 0;
    const int result =
        ((int(__fastcall*)(void*, void*))ownerVtable[28 / 4])(owner, nullptr);

    self->field_016c = 0;
    self->field_0170 = 0;
    self->field_0174 = 0;
    return result;
}

HOOK(0x0048F360, S_0048f360_ResetState, "auto");
