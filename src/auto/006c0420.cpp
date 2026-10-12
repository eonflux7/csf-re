#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Visitor callback type for RpClumpForAllAtomics: (RpAtomic*, void* data) -> RpAtomic*.
typedef void* (__cdecl* AtomicVisitorFn)(void* atomic, void* data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));

// Walks every atomic of the clump with the visitor at 0x6C03F0; the visitor
// clears the flag when an atomic is not eligible. Returns that flag.
static char __cdecl GfxScene_IsClumpEligible(int clump) {
    char eligible = 0;
    RpClumpForAllAtomics(clump,
                         reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006C03F0)),
                         reinterpret_cast<int>(&eligible));
    return eligible;
}

HOOK(0x006C0420, GfxScene_IsClumpEligible, "auto");
