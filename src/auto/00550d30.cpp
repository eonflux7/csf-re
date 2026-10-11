#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Resolves the spawn-point entry for a request handle; if the entry is idle and bound,
// forwards the request's spawn parameters to the spawn-point handler.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, sub_744F70, 0x00744F70, (void* self, void* edx, char arg0, char a3, char a4, char a5, char a6, char a7, char a8, char a2, char a10));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __stdcall MpSpawn_TryActivateSpawnPoint(int spawnRequest)
{
    uint8_t* request = reinterpret_cast<uint8_t*>(spawnRequest);

    int entryHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, spawnRequest + 8);
    uint8_t* entry = reinterpret_cast<uint8_t*>(entryHandle);
    int result = entryHandle;

    if (!CSF_AT(int, entry, 0x50))
    {
        result = CSF_AT(int, entry, 0x18);
        if (result)
        {
            if (result != 1712)
            {
                void* spawnPoint = reinterpret_cast<void*>(result - 1712);
                result = static_cast<unsigned char>(sub_744F70(
                    spawnPoint,
                    nullptr,
                    static_cast<char>(CSF_AT(int, request, 0x4C)),
                    static_cast<char>(CSF_AT(int, request, 0x34)),
                    static_cast<char>(CSF_AT(int, request, 0x38)),
                    static_cast<char>(CSF_AT(int, request, 0x3C)),
                    static_cast<char>(CSF_AT(int, request, 0x40)),
                    static_cast<char>(CSF_AT(int, request, 0x44)),
                    static_cast<char>(CSF_AT(int, request, 0x48)),
                    static_cast<char>(CSF_AT(uint8_t, request, 0x50)),
                    static_cast<char>(CSF_AT(int, request, 0x54))));
            }
        }
    }
    return static_cast<char>(result);
}

HOOK(0x550D30, MpSpawn_TryActivateSpawnPoint, "auto");
