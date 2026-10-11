#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Visitor callback type for RpClumpForAllAtomics. The visitor's real signature is
// not declared in the packet, so this is a local function-pointer alias.
using AtomicVisitorFn = int(__cdecl*)(void* atomic, void* data);

// RpClumpForAllAtomics(clump, visitor, data): visits every atomic of the clump.
CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
// Post-visit step: takes the material/data block pointer read from clump+4.
CSF_FUNC(int, __cdecl, sub_6BB960, 0x006BB960, (void** a1));

// Visitor callback passed to RpClumpForAllAtomics (original at 0x006BBF10).
static AtomicVisitorFn AtomicVisitorCallback()
{
    return reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006BBF10));
}

static int __cdecl GfxObjectProps_VisitAtomicsThenProcessMaterials(int clumpHandle)
{
    RpClumpForAllAtomics(clumpHandle, AtomicVisitorCallback(), 0);

    void** materialBlock = CSF_AT(void**, reinterpret_cast<void*>(clumpHandle), 0x4);
    return sub_6BB960(materialBlock);
}

HOOK(0x006BBF30, GfxObjectProps_VisitAtomicsThenProcessMaterials, "auto");
