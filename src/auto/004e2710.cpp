#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(char*, __cdecl, GetSelectedCharacterSpawnName, 0x00551310, ());
CSF_FUNC(bool, __cdecl, MpSpawn_IsCharacterSelectStateActive, 0x005515A0, ());
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __fastcall C_0083e8c8_CanTargetForSpawnAction(C_0083e8c8* self, void* /*edx*/, int a2, int a3, int a4)
{
    (void)a3;
    bool allowed = true;

    if (a2 == 16 && (CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(self->field_000c), 496) & 2) != 0)
        return false;

    GetGlobal8B0688();
    if (CSF_AT(uint8_t, GetSelectedCharacterSpawnName(), 3) == 0)
    {
        int globalValue = GetGlobal8B0688();
        (void)globalValue;
        if (MpSpawn_IsCharacterSelectStateActive())
        {
            switch (a2)
            {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 10:
            case 11:
                if (CSF_AT(int, reinterpret_cast<void*>(a4), 8) >= 0)
                {
                    int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, a4);
                    if (resolved)
                    {
                        allowed = CSF_AT(int, reinterpret_cast<void*>(resolved), 120)
                               != CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 120);
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    return allowed;
}

HOOK(0x004E2710, C_0083e8c8_CanTargetForSpawnAction, "auto");
