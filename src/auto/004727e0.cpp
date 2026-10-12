#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, MissionScript_ReleaseBlock, 0x004714E0, (S_0069b1c0* Block));
static CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl MissionScript_DestroyStringBlock(S_0069b1c0* block)
{
    if (!block)
        return 0;

    if (static_cast<std::uint32_t>(block->field_001c) >= 0x10)
    {
        if (block->field_0008)
            Mem_FreeDebug(reinterpret_cast<void*>(block->field_0008));
    }

    block->field_001c = 15;
    block->field_0018 = 0;
    reinterpret_cast<std::uint8_t*>(&block->field_0008)[0] = 0;
    return MissionScript_ReleaseBlock(block);
}

HOOK(0x004727E0, MissionScript_DestroyStringBlock, "auto");
