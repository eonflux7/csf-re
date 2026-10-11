#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x005129A0 is __cdecl with four forwarded values: the two original
// stack arguments, the incoming ECX (context), and the second argument repeated.
CSF_FUNC(int, __cdecl, MenuSaveGame_ReleaseSlotBuffer, 0x005129A0,
         (uint32_t slotBuffer, uint32_t slotCount, void* slotOwnerSubobject, void* owner));

// 0x00513480: __stdcall thunk. Original pushes (arg1, ECX, arg1, arg0) so the callee
// sees (arg0, arg1, ECX, arg1). ECX arrives as the first __fastcall parameter and the
// two stack arguments are callee-cleaned (ret 8).
static int __fastcall MenuSaveGame_ForwardReleaseSlotArguments(
    void* context, void* /*edx*/, uint32_t slotBuffer, uint32_t slotCount)
{
    return MenuSaveGame_ReleaseSlotBuffer(
        slotBuffer,
        slotCount,
        context,
        reinterpret_cast<void*>(slotCount));
}

HOOK(0x00513480, MenuSaveGame_ForwardReleaseSlotArguments, "auto");
