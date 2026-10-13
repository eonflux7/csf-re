#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: S_0047ea40 pointer-storage destructor (__fastcall, ecx = this, edx unused).
CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790, (S_0047ea40* self, void*));

// Destroys the pointer storage embedded at +0x08, then tail-calls the one at +0x18.
// The tail call returns the callee's result directly, matching the original `jmp`.
static int __fastcall C_00832eec_DestroyEmbeddedPointerStorages(C_00832eec* self, void* /*edx*/)
{
    S_0047ea40_DestroyPointerStorage(reinterpret_cast<S_0047ea40*>((uint8_t*)self + 0x8), nullptr);
    return S_0047ea40_DestroyPointerStorage(reinterpret_cast<S_0047ea40*>((uint8_t*)self + 0x18), nullptr);
}

HOOK(0x6F1650, C_00832eec_DestroyEmbeddedPointerStorages, "auto");
