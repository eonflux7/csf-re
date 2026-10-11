#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callback pointer stored at object+0x48: a plain __cdecl function taking one int.
using GfxEnvironment_StoredCallback = void(__cdecl*)(int);

// Calls the callback stored at object+0x48 with the object as its single
// __cdecl argument, then returns the object pointer unchanged.
static int __cdecl GfxEnvironment_RunStoredCallbackThenReturn(int a1) {
    void* object = reinterpret_cast<void*>(a1);
    GfxEnvironment_StoredCallback callback = CSF_AT(GfxEnvironment_StoredCallback, object, 0x48);
    callback(a1);
    return a1;
}

HOOK(0x6BC2D0, GfxEnvironment_RunStoredCallbackThenReturn, "auto");
