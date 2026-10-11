#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Engine object global (Rw engine instance pointer).
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

// Engine function pointer stored at +0x24 of the engine object (slot 9 of a
// function-pointer table, not a vtable). Callee is __cdecl and takes two ints.
using RwEngineSlotFn = int(__cdecl*)(int, int);

// Forwards two stack arguments to the RW engine function at object offset 0x24.
// __stdcall: callee pops 8 bytes (ret 8).
static int __stdcall GfxSkin_ForwardToRwEngineSlot24(int a1, int a2) {
    RwEngineSlotFn engineFn = CSF_AT(RwEngineSlotFn, g_pRwEngineInstance, 0x24);
    return engineFn(a1, a2);
}

HOOK(0x006C7B60, GfxSkin_ForwardToRwEngineSlot24, "auto");
