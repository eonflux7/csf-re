#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Engine object global (Rw engine instance pointer).
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

// Engine function pointer stored at +0x20 of the engine object (slot 8 of a
// function-pointer table, not a vtable). Callee is __cdecl and takes two ints.
using RwEngineSlotFn = int(__cdecl*)(int, int);

// Forwards two stack arguments to the RW engine function at object offset 0x20.
// __stdcall: callee pops 8 bytes (ret 8).
static int __stdcall GfxSkin_ForwardToRwEngineSlot20(int a1, int a2) {
    RwEngineSlotFn engineFn = CSF_AT(RwEngineSlotFn, g_pRwEngineInstance, 0x20);
    return engineFn(a1, a2);
}

HOOK(0x006C7B40, GfxSkin_ForwardToRwEngineSlot20, "auto");
