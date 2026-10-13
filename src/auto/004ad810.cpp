#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Returns true when the linked alert handle resolves to an object whose state (+0x50)
// is one of the "ready" states 9, 14, 15 or 16.
static bool __fastcall AiAlert_IsLinkedAlertReady(int* self, void* /*edx*/)
{
    // Handle slot at +0x14 is signed: a negative value means no link.
    if (CSF_AT(int, self, 0x14) < 0)
        return false;

    // Resolve the handle stored at +0x0C in the current scene's handle table.
    uint8_t* resolved = reinterpret_cast<uint8_t*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, (int)((uint8_t*)self + 0x0C)));
    if (resolved == nullptr)
        return false;

    int state = CSF_AT(int, resolved, 0x50);
    return state == 14 || state == 15 || state == 16 || state == 9;
}

HOOK(0x004AD810, AiAlert_IsLinkedAlertReady, "auto");
