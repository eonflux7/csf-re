#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall S_0047ea40_DestroyPointerStorage(S_0047ea40* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0004);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<std::uintptr_t>(result)));

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    return result;
}

HOOK(0x00558790, S_0047ea40_DestroyPointerStorage, "auto");
