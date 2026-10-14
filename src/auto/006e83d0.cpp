#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x8B68C0);

// Calls slot 67 (byte offset 0x10C) of the RW engine instance's vtable with
// the argument, then reports success.
static int __cdecl HAnimForwardToRwEngineSlot67(int a1)
{
    void* engine = g_pRwEngineInstance;
    // the engine instance is a struct of function pointers (malloc at +0x108, free at +0x10C), not an object with a vtable
    // Original: mov ecx, [g_pRwEngineInstance]; push a1; call [ecx+0x10C]; add esp, 4. The cdecl callee gets the
    // engine instance in ecx as well, so the call goes through hook::call_regs (caller pops the one word).
    const uint32_t freeArgs[1] = {static_cast<uint32_t>(a1)};
    hook::Regs regs{};
    regs.ecx = reinterpret_cast<uint32_t>(engine);
    hook::call_regs(CSF_AT(uintptr_t, engine, 0x10C), &regs, freeArgs, 1);
    return 1;
}

HOOK(0x6E83D0, HAnimForwardToRwEngineSlot67, "auto");

