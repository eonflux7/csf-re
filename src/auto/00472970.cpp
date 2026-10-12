#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MissionScript_ReleaseBlock, 0x004714E0, (S_0069b1c0* Block));
CSF_FUNC(void, __fastcall, StateRecord_Prepare, 0x00471E00, (uint32_t* a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl MissionScript_DestroyStateRecord(S_0069b1c0* block)
{
    if (!block)
        return 0;

    StateRecord_Prepare(reinterpret_cast<uint32_t*>(block));
    if (block->field_0010)
        Mem_FreeDebug(reinterpret_cast<void*>(block->field_0010));

    block->field_0010 = 0;
    CSF_AT(uint32_t, block, 0x14) = 0;
    block->field_0018 = 0;
    return MissionScript_ReleaseBlock(block);
}

HOOK(0x00472970, MissionScript_DestroyStateRecord, "auto");
