#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));

// Returns the LTM matrix pointer of the provider frame object, offset by 0x30.
static int __fastcall C_00862900_GetProviderLtmPlus30(C_00862900* self, void* /*edx*/) {
    int providerFrameObject = GfxFrames_GetProviderFrameObject(static_cast<int>(self->field_0004));
    auto* providerBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(providerFrameObject));
    std::uint32_t frame = CSF_AT(std::uint32_t, providerBase, 0x4);
    std::uint32_t* ltm = RwFrameGetLTM(frame);
    return static_cast<int>(reinterpret_cast<uintptr_t>(ltm) + 0x30);
}

HOOK(0x006F7300, C_00862900_GetProviderLtmPlus30, "auto");
