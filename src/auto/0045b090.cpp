#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Bound by address: CHandleTable::Resolve (__thiscall, 1 stack arg, ret 4).
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Bound by address: scalar getter taking the resolved object (__thiscall, no stack args, returns double in st0).
CSF_FUNC(double, __fastcall, ComputeScalarForResolvedObject, 0x0048F220, (void* self, void* edx));

// Current scene pointer, global at 0x008AFC90.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Refreshes the float at +0x68 from the scalar of the object resolved through the handle at +4.
// The handle is only resolved when the signed int at +0x0C is non-negative; otherwise the object is null.
// Three stack arguments (a2..a4) are unused by the original but are popped by the callee (ret 12).
static void __fastcall Entity_RefreshScalarFromHandle(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, int /*a4*/)
{
    void* resolvedObject = nullptr;

    if (CSF_AT(int, self, 0x0C) >= 0)
    {
        // Original passes the address this+4 as the handle value.
        int handle = static_cast<int>(reinterpret_cast<uintptr_t>(self) + 4);
        resolvedObject = reinterpret_cast<void*>(CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    }

    CSF_AT(float, self, 0x68) = static_cast<float>(ComputeScalarForResolvedObject(resolvedObject, nullptr));
}

HOOK(0x0045B090, Entity_RefreshScalarFromHandle, "auto");
