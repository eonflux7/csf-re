#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// The caller pushes [this+0x68] as a stack argument before the call and the
// callee pops it, so the payload is part of the signature.
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));

static int __fastcall ResolveSceneHandleAndDispatch(int* self, void* /*edx*/, int /*a2*/, int /*a3*/, int /*a4*/)
{
    void* resolvedObject = nullptr;
    if (CSF_AT(int, self, 0x0C) >= 0)
    {
        int handleAddress = (int)((uint8_t*)self + 4);
        resolvedObject = (void*)CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress);
    }
    return DispatchSceneObjectWithPayload(resolvedObject, nullptr, CSF_AT(int, self, 0x68));
}

HOOK(0x0045B0C0, ResolveSceneHandleAndDispatch, "auto");
