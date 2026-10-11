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
    using RwFreeFn = void (__cdecl*)(int);
    auto freeFn = CSF_AT(RwFreeFn, engine, 0x10C);
    freeFn(a1);
    return 1;
}

HOOK(0x6E83D0, HAnimForwardToRwEngineSlot67, "auto");

