#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops). Bound as used by the original.
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Base-class initializer for the shape allocator object (__fastcall: ecx=this, edx unused).
CSF_FUNC(uint32_t*, __fastcall, ShapeAllocatorBase_Construct, 0x006C1490, (void* self, void* edx));

// Registers the texture read callback (cdecl, one argument).
CSF_FUNC(int, __cdecl, RwTextureSetReadCallBack, 0x006315D0, (int a1));

// Registers the texture find callback (cdecl, two arguments).
CSF_FUNC(int, __cdecl, RwTextureSetFindCallBack, 0x006315B0, (int a1, int a2));

// Allocates the 12-byte shape allocator handle, runs its base constructor, sets its vtable,
// and registers the PTank texture callbacks. Returns nullptr if the allocation fails.
static uint32_t* __cdecl GetShapeAllocatorHandle()
{
    int allocatedAddress = CoreMemory_AllocUntagged(12);
    if (!allocatedAddress)
        return nullptr;

    void* handleObject = reinterpret_cast<void*>(static_cast<std::uintptr_t>(allocatedAddress));
    ShapeAllocatorBase_Construct(handleObject, nullptr);

    *reinterpret_cast<void***>(handleObject) = CSF_VTBL(0x008633E0);

    int loadTextureCallback = static_cast<int>(reinterpret_cast<std::uintptr_t>(CSF_ADDR_FN(0x00707D60)));
    int findTextureCallback = static_cast<int>(reinterpret_cast<std::uintptr_t>(CSF_ADDR_FN(0x00707D30)));

    RwTextureSetReadCallBack(loadTextureCallback);
    // The original never pops the read callback pushed above (cdecl, caller pops nothing there),
    // so the find call's second argument is that same leftover stack value.
    RwTextureSetFindCallBack(findTextureCallback, loadTextureCallback);

    return static_cast<uint32_t*>(handleObject);
}

HOOK(0x006EEA50, GetShapeAllocatorHandle, "auto");
