#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x8B68C0);

// Calls slot 67 (byte offset 0x10C) of the RW engine instance's vtable with
// the argument, then reports success.
static int __cdecl HAnimForwardToRwEngineSlot67(int a1)
{
    void* engine = g_pRwEngineInstance;
    auto slotFn = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, engine, 0)[0x10C / 4]);
    slotFn(engine, nullptr, a1);
    return 1;
}

HOOK(0x6E83D0, HAnimForwardToRwEngineSlot67, "auto");
