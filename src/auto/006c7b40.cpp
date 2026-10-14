#include <cstdint>
#include "globals.h"
#include "hook.h"

// Engine object global (Rw engine instance pointer).
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

// The original: mov eax, [ebp+0xc] (a2); mov ecx, [ebp+8] (a1); mov edx, [g_pRwEngineInstance];
// push eax; push ecx; call [edx+0x20]; add esp, 8; ret 8. The callee is __cdecl (the caller pops the two words), so
// the stack carries a1 then a2, and ecx = a1, eax = a2, edx = the engine instance are what it holds at the call.
// A plain C++ call leaves ecx at the compiler's own scratch value (a2), so the call goes through hook::call_regs with
// those three registers loaded as the original loads them.
static uint32_t __cdecl GfxSkin_ForwardToRwEngineSlot20(hook::Regs* regs, const uint32_t* args)
{
    regs->ecx = args[0];
    regs->eax = args[1];
    regs->edx = reinterpret_cast<uint32_t>(g_pRwEngineInstance);
    const uintptr_t engineFn = reinterpret_cast<uintptr_t>(CSF_AT(void*, g_pRwEngineInstance, 0x20));
    return hook::call_regs(engineFn, regs, args, 2);
}

HOOK_REGS(0x006C7B40, GfxSkin_ForwardToRwEngineSlot20, 8, "auto");
