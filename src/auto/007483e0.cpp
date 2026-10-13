#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address. Returns char (al) in the original.
CSF_FUNC(char, __fastcall, MpSpawnPoints_WriteSpawnEntry, 0x0074CCA0,
         (void* self, void* edx, char a2, int a3, char a4, int a5, char a6, int a7));

// Bound by address. Returns int (eax) in the original.
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));

static int __fastcall MpSpawnPoints_SetSpawnIdentifiers(C_00858180* self, void* /*edx*/,
                                                        int a2, int a3, int a4,
                                                        int a5, int a6, int a7)
{
    Sub_64B250(self, nullptr);
    if (Sub_64B250(self, nullptr)) {
        MpSpawnPoints_WriteSpawnEntry(self, nullptr,
                                      (char)a2, a3, (char)a4, a5, (char)a6, a7);
    } else {
        CSF_AT(uint8_t, self, 0x331) = 1;
    }

    CSF_AT(int, self, 0x338) = a2;
    CSF_AT(int, self, 0x34C) = a7;
    CSF_AT(int, self, 0x348) = a6;
    CSF_AT(int, self, 0x33C) = a3;
    CSF_AT(int, self, 0x340) = a4;
    CSF_AT(int, self, 0x344) = a5;
    return a4;
}

HOOK(0x7483E0, MpSpawnPoints_SetSpawnIdentifiers, "auto");
