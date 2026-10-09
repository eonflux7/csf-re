#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, SetFlagBit40, 0x0045BC90, (void* self, void* , char enable));
CSF_FUNC(int, __cdecl, Object_IsStateAllowed, 0x00755810, (unsigned int a1));

// Resolves the handle stored in the record chain, checks the resolved object's
// state, and if allowed sets its bit-40 flag from the byte in the second record.
static int __stdcall SceneHandle_ApplyFlagBitFromRecord(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    uint8_t* headerRecord = reinterpret_cast<uint8_t*>(a3[0]);
    uint8_t* entryRecord = reinterpret_cast<uint8_t*>(*CSF_AT(uint32_t*, headerRecord, 4));
    int32_t entryFlags = *CSF_AT(int32_t*, entryRecord, 12);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(entryRecord) + 4);

    if (entryFlags >= 0)
    {
        void* scene = g_pCurrentScene;
        uint8_t* resolved = reinterpret_cast<uint8_t*>(CHandleTable_Resolve(scene, nullptr, result));
        result = static_cast<int>(reinterpret_cast<uintptr_t>(resolved));
        if (resolved)
        {
            result = Object_IsStateAllowed(*CSF_AT(uint32_t*, resolved, 0x50));
            if (static_cast<uint8_t>(result))
            {
                uint8_t* flagRecord = reinterpret_cast<uint8_t*>(a3[1]);
                uint8_t* flagSource = reinterpret_cast<uint8_t*>(*CSF_AT(uint32_t*, flagRecord, 4));
                uint8_t flagBit = *CSF_AT(uint8_t*, flagSource, 1);
                return static_cast<int>(SetFlagBit40(resolved, nullptr, static_cast<char>(flagBit)));
            }
        }
    }
    return result;
}

HOOK(0x4610B0, SceneHandle_ApplyFlagBitFromRecord, "auto");
