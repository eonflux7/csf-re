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

// Predicate at 0x69B590: __fastcall, ecx = table object, one stack argument.
CSF_FUNC(char, __fastcall, BlockList_TryHandle, 0x0069B590, (void* self, void* edx, unsigned int a2));
// Heap release used by the game for mission script blocks.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl MissionScript_ReleaseBlock(S_0069b1c0* Block)
{
    int result = 0;

    result = BlockList_TryHandle(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B032C)), nullptr,
                                 reinterpret_cast<unsigned int>(Block));
    if (!result) {
        result = BlockList_TryHandle(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B0370)), nullptr,
                                     reinterpret_cast<unsigned int>(Block));
        if (!result) {
            result = BlockList_TryHandle(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B03B4)), nullptr,
                                         reinterpret_cast<unsigned int>(Block));
            if (!result) {
                result = BlockList_TryHandle(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B03F8)), nullptr,
                                             reinterpret_cast<unsigned int>(Block));
                if (!result) {
                    result = BlockList_TryHandle(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B02E8)), nullptr,
                                                 reinterpret_cast<unsigned int>(Block));
                    if (!result)
                        return Mem_FreeDebug(Block);
                }
            }
        }
    }
    return result;
}

HOOK(0x004714E0, MissionScript_ReleaseBlock, "auto");
