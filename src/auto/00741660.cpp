#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __fastcall, ClearListAndFreeNodes, 0x004E9980, (uint32_t* self, void* edx));
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_7448B0, 0x007448B0, (void* self, void* edx));

static char __fastcall CMenuMpStats_RefreshSpawnStats(CMenuMpStats* self, void* /*edx*/)
{
    // First spawn-point list (sentinel at +0x6D4): free each node's data block.
    {
        uint32_t* sentinel = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x6D4));
        uint32_t* node = reinterpret_cast<uint32_t*>(sentinel[0]);
        if (node != sentinel) {
            do {
                if (node[2]) {
                    Mem_Free(reinterpret_cast<void*>(node[2]));
                }
                node = reinterpret_cast<uint32_t*>(node[0]);
            } while (node != reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x6D4)));
        }
    }
    ClearListAndFreeNodes(reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x6D0), nullptr);

    // Second list (sentinel at +0x6E0): same procedure.
    {
        uint32_t* sentinel = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x6E0));
        uint32_t* node = reinterpret_cast<uint32_t*>(sentinel[0]);
        if (node != sentinel) {
            do {
                if (node[2]) {
                    Mem_Free(reinterpret_cast<void*>(node[2]));
                }
                node = reinterpret_cast<uint32_t*>(node[0]);
            } while (node != reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x6E0)));
        }
    }
    ClearListAndFreeNodes(reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x6DC), nullptr);

    char result = static_cast<char>(Sub_64B250(self, nullptr));
    if (result) {
        CSF_AT(uint8_t, self, 0x452) &= static_cast<uint8_t>(~0x80u);
        CSF_AT(uint8_t, self, 0x453) &= static_cast<uint8_t>(1u);
        return sub_7448B0(self, nullptr);
    }
    return result;
}

HOOK(0x00741660, CMenuMpStats_RefreshSpawnStats, "auto");
