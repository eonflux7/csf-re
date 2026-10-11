#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bindings for the original callees (addresses from the packet).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));

// Reference to the S_00402350 vtable. InitS00402350Object installs it on the new block;
// this function stores nothing itself, so the binding is kept only for the address reference.
[[maybe_unused]] static const auto kS00402350Vtbl = CSF_VTBL(0x00402350);

// Returns the lazily created sub-block stored at self+0x14, creating it on first use.
// Returns nullptr (and clears the slot) if the allocation fails.
static uint32_t* __fastcall GfxObjectProps_GetOrCreatePropsBlock(void* self, void* /*edx*/) {
    uint32_t* result = CSF_AT(uint32_t*, self, 0x14);
    if (result == nullptr) {
        void* block = Mem_Alloc(60);
        if (block != nullptr) {
            result = InitS00402350Object(block, nullptr);
            CSF_AT(uint32_t*, self, 0x14) = result;
        } else {
            CSF_AT(uint32_t*, self, 0x14) = nullptr;
            return nullptr;
        }
    }
    return result;
}

HOOK(0x006B8AE0, GfxObjectProps_GetOrCreatePropsBlock, "auto");
