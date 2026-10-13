#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00858180_WriteAddPuntoRespawnFields, 0x0074CB50, (void* self, void* edx, char a2, char a3));

static char __fastcall C_00858180_RecordRespawnPointAndAdvanceCounter(
    C_00858180* self, void* /*edx*/, char category, int value)
{
    const int capacity = CSF_AT(int8_t, self, 0x69);

    if (category == 1)
    {
        int32_t* entries = CSF_AT(int32_t*, self, 0x350);
        if (entries)
        {
            entries[CSF_AT(int32_t, self, 0x354)] = value;
            const int32_t next = static_cast<int32_t>(
                CSF_AT(uint32_t, self, 0x354) + 1u);
            CSF_AT(int32_t, self, 0x354) = next;
            if (next >= capacity)
            {
                CSF_AT(uint8_t, self, 0x360) = 1;
                CSF_AT(int32_t, self, 0x354) = 0;
            }
        }
    }
    else
    {
        int32_t* entries = CSF_AT(int32_t*, self, 0x358);
        if (entries)
        {
            entries[CSF_AT(int32_t, self, 0x35C)] = value;
            const int32_t next = static_cast<int32_t>(
                CSF_AT(uint32_t, self, 0x35C) + 1u);
            CSF_AT(int32_t, self, 0x35C) = next;
            if (next >= capacity)
            {
                CSF_AT(uint8_t, self, 0x361) = 1;
                CSF_AT(int32_t, self, 0x35C) = 0;
            }
        }
    }

    const char result = static_cast<char>(Sub_64B250(self, nullptr));
    if (result)
    {
        return C_00858180_WriteAddPuntoRespawnFields(
            self, nullptr, category, static_cast<char>(value));
    }
    return result;
}

HOOK(0x007481B0, C_00858180_RecordRespawnPointAndAdvanceCounter, "auto");
