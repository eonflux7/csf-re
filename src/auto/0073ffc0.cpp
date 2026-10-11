#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetPlayerEntryId, 0x0073D0F0, (S_0073ecc0* self, void*));
CSF_FUNC(char, __fastcall, sub_742780, 0x00742780, (void* self, void* edx, char a2, char a3));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall CMenuMpStats_ApplyEntryUpdate(CMenuMpStats* self, void* /*edx*/, uint16_t a2, int a3)
{
    if (CSF_AT(float, self, 0xB0) < g_fZero)
    {
        if (Sub_64B250(self, nullptr) != 0)
            sub_742780(self, nullptr, static_cast<char>(a2), static_cast<char>(a3));

        S_0073ecc0* entryHolder = reinterpret_cast<S_0073ecc0*>(GetGlobalDword_73C020());
        if (a3 == MpSpawnPoints_GetPlayerEntryId(entryHolder, nullptr))
            CSF_AT(uint32_t, self, 0xA4) = a2;

        CSF_AT(float, self, 0xB0) = 1.0f;
    }
}

HOOK(0x0073FFC0, CMenuMpStats_ApplyEntryUpdate, "auto");
