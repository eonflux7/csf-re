#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetPlayerEntryId, 0x0073D0F0, (S_0073ecc0* self, void*));
CSF_FUNC(void, __fastcall, CMenuMpStats_ApplyEntryUpdate, 0x0073FFC0, (CMenuMpStats* self, void*, uint16_t a2, int a3));
CSF_FUNC(char, __fastcall, C_00858180_WriteRecogerArmaFields, 0x00742720, (C_00858180* self, void*, char a2, char a3));

static char __fastcall C_0083c478_ApplyEntryToMenuIfReady(C_0083c478* self, void* /*edx*/, int a2)
{
    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (sceneMenuContext)
    {
        C_00833908* sceneMenu = reinterpret_cast<C_00833908*>(sceneMenuContext);
        if (sceneMenu->field_0044)
        {
            sceneMenuContext = self->field_0014;
            if (sceneMenuContext)
            {
                if (a2)
                {
                    if (self->field_0044 != 0xFFFFFFFFu)
                    {
                        sceneMenuContext = self->field_0056;
                        if (!static_cast<uint8_t>(sceneMenuContext))
                        {
                            uint8_t* viewObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2)) + 0x78;
                            bool hasMpStats = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, viewObj, 0)[2])(viewObj, nullptr) != 0;
                            if (hasMpStats)
                            {
                                S_0073ecc0* spawnPoints = reinterpret_cast<S_0073ecc0*>(GetGlobalDword_73C020());
                                int playerEntryId = MpSpawnPoints_GetPlayerEntryId(spawnPoints, nullptr);
                                CMenuMpStats_ApplyEntryUpdate(reinterpret_cast<CMenuMpStats*>(static_cast<uintptr_t>(a2)),
                                                              nullptr,
                                                              static_cast<uint16_t>(self->field_0044),
                                                              playerEntryId);
                            }
                            else
                            {
                                S_0073ecc0* spawnPoints = reinterpret_cast<S_0073ecc0*>(GetGlobalDword_73C020());
                                char playerEntryByte = static_cast<char>(MpSpawnPoints_GetPlayerEntryId(spawnPoints, nullptr));
                                char slotByte = static_cast<char>(static_cast<uint16_t>(self->field_0044));
                                char writeResult = C_00858180_WriteRecogerArmaFields(
                                    reinterpret_cast<C_00858180*>(static_cast<uintptr_t>(a2)),
                                    nullptr,
                                    slotByte,
                                    playerEntryByte);
                                sceneMenuContext = static_cast<uint8_t>(writeResult);
                            }
                        }
                    }
                }
            }
        }
    }
    return static_cast<char>(sceneMenuContext);
}

HOOK(0x004D6980, C_0083c478_ApplyEntryToMenuIfReady, "auto");
