#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetPlayerEntryId, 0x0073D0F0, (S_0073ecc0* self, void*));
CSF_FUNC(void, __fastcall, CMenuMpStats_HandlePlayerSpawnEvent, 0x00740020, (void* self, void* , uint16_t playerSlot, int playerEntryId));
CSF_FUNC(char, __fastcall, C_00858180_WriteRecogerMunicionFields_742820, 0x00742820, (C_00858180* self, void* , char a2, char a3));

static int __fastcall C_0083c478_HandleMpSpawnPointEvent(C_0083c478* self, void* /*edx*/, int a2)
{
    int result = static_cast<int>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (result)
    {
        uint8_t* contextBytes = reinterpret_cast<uint8_t*>(result);
        if (CSF_AT(uint32_t, contextBytes, 0x44) != 0)
        {
            result = static_cast<int>(self->field_0014);
            if (result)
            {
                if (a2 && self->field_0048 != 0xFFFFFFFFu)
                {
                    uint8_t* statsSubobject = reinterpret_cast<uint8_t*>(a2) + 0x78;
                    bool recognized = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, statsSubobject, 0)[2])(statsSubobject, nullptr);
                    if (recognized)
                    {
                        S_0073ecc0* globalObject = reinterpret_cast<S_0073ecc0*>(GetGlobalDword_73C020());
                        int playerEntryId = MpSpawnPoints_GetPlayerEntryId(globalObject, nullptr);
                        CMenuMpStats_HandlePlayerSpawnEvent(reinterpret_cast<void*>(a2), nullptr,
                                                            static_cast<uint16_t>(self->field_0048),
                                                            playerEntryId);
                    }
                    else
                    {
                        S_0073ecc0* globalObject = reinterpret_cast<S_0073ecc0*>(GetGlobalDword_73C020());
                        char playerEntryId = static_cast<char>(MpSpawnPoints_GetPlayerEntryId(globalObject, nullptr));
                        char writeResult = C_00858180_WriteRecogerMunicionFields_742820(
                            reinterpret_cast<C_00858180*>(a2), nullptr,
                            static_cast<char>(self->field_0048), playerEntryId);
                        result = static_cast<int>((static_cast<uint32_t>(result) & 0xFFFFFF00u)
                                                  | static_cast<uint8_t>(writeResult));
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x004D6A10, C_0083c478_HandleMpSpawnPointEvent, "auto");
