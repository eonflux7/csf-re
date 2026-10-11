#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_GetEntryFlags, 0x0073CCE0, (int a1));
CSF_FUNC(char, __fastcall, EntryEventSink_Dispatch, 0x007435F0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Forwards a handle-based event: resolves the handle in the current scene,
// queries the global dword and entry flags, then dispatches to the sink object
// stored at +0x4C of this.
static int __fastcall C_0083e330_ForwardHandleEvent(C_0083e330* self, void* /*edx*/, uint32_t* a2)
{
    int result = (int)self->field_004c;
    if (result != 0)
    {
        int resolvedEntry = CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(a2));
        int v9 = (int)a2[8];
        int v8 = (int)a2[7];
        int v7 = (int)a2[6];
        int entryHandleValue = *reinterpret_cast<int*>(resolvedEntry + 0x18);
        GetGlobalDword_73C020();
        int entryFlags = NetSession_GetEntryFlags(entryHandleValue);
        return EntryEventSink_Dispatch(reinterpret_cast<void*>(self->field_004c), nullptr,
                                       (int)a2[9], (int)a2[5], (int)a2[3], (int)a2[4],
                                       entryFlags, v7, v8, v9);
    }
    return result;
}

HOOK(0x004E1460, C_0083e330_ForwardHandleEvent, "auto");
