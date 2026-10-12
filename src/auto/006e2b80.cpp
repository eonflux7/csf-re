#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_6E0FD0, 0x006E0FD0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, sub_6E2B30, 0x006E2B30, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_6DF290, 0x006DF290, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_6E1220, 0x006E1220, (void* self, void* edx, void* a2, int a3));

// Vtable of the object created by sub_6E2B30 (referenced at the call site 0x6E2BA8).
[[maybe_unused]] static const void* kEmitterChildVtable =
    reinterpret_cast<const void*>(CSF_VTBL(0x006E2B30));

static int __fastcall ParticleEmitters_AddEmitterIfNotPresent(void* self, void* /*edx*/, C_0082d410* a2)
{
    // Sub-object embedded at +0x40 of the owner.
    void* childSlot = static_cast<uint8_t*>(self) + 0x40;

    int result = sub_6E0FD0(childSlot, nullptr, reinterpret_cast<uint32_t*>(a2));
    if (result != 0)
        return result;

    void* block = Mem_Alloc(0x44);
    int newObject = 0;
    if (block != nullptr)
        newObject = sub_6E2B30(block, nullptr);

    // Choose the source argument: inline buffer address when small, else the pointer stored at +4.
    uint32_t sourceArg;
    if (CSF_AT(uint32_t, a2, 0x18) < 0x10u)
        sourceArg = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(static_cast<void*>(a2)) + 4));
    else
        sourceArg = CSF_AT(uint32_t, a2, 0x4);

    sub_6DF290(reinterpret_cast<void*>(static_cast<uintptr_t>(newObject)), nullptr,
               static_cast<int>(sourceArg));

    sub_6E1220(childSlot, nullptr, a2, newObject);

    return newObject;
}

HOOK(0x006E2B80, ParticleEmitters_AddEmitterIfNotPresent, "auto");
