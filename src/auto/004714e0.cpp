#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Predicate at 0x69B590: __fastcall, ecx = table object, one stack argument. Its answer is al; the original
// hands the whole of eax on to its own caller, so it is bound returning int and only the low byte is tested.
CSF_FUNC(int, __fastcall, S_0069b1c0_TryReleaseBlock, 0x0069B590, (void* self, void* edx, unsigned int a2));
// Heap release used by the game for mission script blocks.
CSF_FUNC(int, __cdecl, MemDebug_FreeDebugBlock, 0x00690E60, (void* block));

static int __cdecl MissionScript_ReleaseBlock(S_0069b1c0* Block)
{
    int result = 0;

    result = S_0069b1c0_TryReleaseBlock(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B032C)), nullptr,
                                 reinterpret_cast<unsigned int>(Block));
    if (!static_cast<uint8_t>(result)) {
        result = S_0069b1c0_TryReleaseBlock(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B0370)), nullptr,
                                     reinterpret_cast<unsigned int>(Block));
        if (!static_cast<uint8_t>(result)) {
            result = S_0069b1c0_TryReleaseBlock(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B03B4)), nullptr,
                                         reinterpret_cast<unsigned int>(Block));
            if (!static_cast<uint8_t>(result)) {
                result = S_0069b1c0_TryReleaseBlock(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B03F8)), nullptr,
                                             reinterpret_cast<unsigned int>(Block));
                if (!static_cast<uint8_t>(result)) {
                    result = S_0069b1c0_TryReleaseBlock(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B02E8)), nullptr,
                                                 reinterpret_cast<unsigned int>(Block));
                    if (!static_cast<uint8_t>(result))
                        return MemDebug_FreeDebugBlock(Block);
                }
            }
        }
    }
    return result;
}

HOOK(0x004714E0, MissionScript_ReleaseBlock, "auto");

