#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __cdecl, RwFrameRemoveChild, 0x00623750, (int a1));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));

static void __fastcall C_00860700_RemoveChildFrameIfReady(
    C_00860700* self, void* /*edx*/, int childIdentifier)
{
    alignas(8) uint8_t stateBytes[0x20];
    auto* state = reinterpret_cast<C_00860630*>(stateBytes);

    C_00860178_Construct(reinterpret_cast<C_00860178*>(state), nullptr);

    CSF_AT(void*, stateBytes, 0x00) = CSF_VTBL(0x00860700);
    CSF_AT(uint32_t, stateBytes, 0x04) =
        reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008606F8));
    CSF_AT(uint32_t, stateBytes, 0x08) =
        reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008606DC));
    CSF_AT(uint32_t, stateBytes, 0x14) = 0;
    CSF_AT(uint32_t, stateBytes, 0x18) = 0;
    CSF_AT(uint32_t, stateBytes, 0x1C) = 0;
    CSF_AT(uint32_t, stateBytes, 0x0C) = 0;
    CSF_AT(uint8_t, stateBytes, 0x10) = 0;

    // Inline the typed dispatch wrapper: the only call here must be the
    // original virtual target, not an additional C++ wrapper boundary.
    [[clang::always_inline]]
    const char ready = self->vSlot78(
        childIdentifier, static_cast<int>(reinterpret_cast<uintptr_t>(state)));

    if (ready != 0) {
        const uint32_t childFrame = CSF_AT(uint32_t, stateBytes, 0x0C);
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(childFrame), 0x04) != 0) {
            RwFrameRemoveChild(static_cast<int>(childFrame));
        }
    }

    GfxObject_DestroyEmbeddedState(state, nullptr);
}

HOOK(0x006BFF40, C_00860700_RemoveChildFrameIfReady, "auto");
