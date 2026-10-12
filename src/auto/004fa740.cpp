#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static char __fastcall C_0083f2f0_AppendConsoleCharacter(
    C_0083f2f0* self, void* /*edx*/, int16_t character)
{
    const int global = GetGlobalInt();
    if (CSF_AT(uint32_t, reinterpret_cast<void*>(global), 0x8C) != 0)
        return static_cast<char>(global);

    const uint8_t enabled = self->field_0010;
    if (!enabled || static_cast<int32_t>(self->field_0144) >= 100)
        return static_cast<char>(enabled);

    void* object = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    int32_t* limit = reinterpret_cast<int32_t* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

    const int32_t length = static_cast<int32_t>(self->field_0144);
    if (length > *limit / 11)
        ++self->field_0164;

    CSF_AT(int16_t, self, 0x18 + static_cast<uint32_t>(length) * 2) = character;
    const uint32_t newLength = self->field_0144 + 1;
    self->field_0144 = newLength;
    CSF_AT(int16_t, self, 0x18 + newLength * 2) = 0;
    return static_cast<char>(newLength);
}

HOOK(0x004FA740, C_0083f2f0_AppendConsoleCharacter, "auto");
