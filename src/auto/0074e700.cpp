#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings copied from the packet. NetPlayerDDL_ProcessEmbeddedField's
// original code loads its `this` from ecx, which a __stdcall binding cannot
// pass. See the notes in the JSON block.
CSF_FUNC(char, __stdcall, NetPlayerDDL_ProcessEmbeddedField, 0x00753410, (int a1));
CSF_FUNC(char, __stdcall, NetGameDDL_ProcessBaseField, 0x0065E740, (int a1, int a2));

// Routes a call by comparing the target pointer against the embedded field
// sub-objects at self+0x44, +0x58, +0xA8. Falls back to the base-class handler.
static char __fastcall NetControlDdl_DispatchEmbeddedField(uint8_t* self, void* /*edx*/,
                                                           uint32_t target, uint32_t param2) {
    if (target == 0)
        NetPlayerDDL_ProcessEmbeddedField(static_cast<int>(param2));
    if (target == static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self + 0x44)))
        return NetPlayerDDL_ProcessEmbeddedField(static_cast<int>(param2));

    if (target == 0)
        NetPlayerDDL_ProcessEmbeddedField(static_cast<int>(param2));
    if (target == static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self + 0x58)))
        return NetPlayerDDL_ProcessEmbeddedField(static_cast<int>(param2));

    if (target == 0)
        NetPlayerDDL_ProcessEmbeddedField(static_cast<int>(param2));
    if (target == static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self + 0xA8)))
        return NetPlayerDDL_ProcessEmbeddedField(static_cast<int>(param2));

    return NetGameDDL_ProcessBaseField(static_cast<int>(target), static_cast<int>(param2));
}

HOOK(0x0074E700, NetControlDdl_DispatchEmbeddedField, "auto");
