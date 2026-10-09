#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original callee at 0x46A3B0: __fastcall (ecx = self, one stack arg), ret 4.
CSF_FUNC(unsigned int, __fastcall, sub_46A3B0, 0x0046A3B0, (void* self, void* edx, uint32_t* a3));

// Original: __thiscall int(int* this, int a2, int a3), ret 8.
// Rewritten as __fastcall so ecx carries `this` and the two stack args pop as ret 8.
static int __fastcall MissionScript_BindRegionToSceneSlot(int* self, void* /*edx*/, int a2, int a3)
{
    uint32_t args[2];
    args[0] = static_cast<uint32_t>(a2);
    args[1] = static_cast<uint32_t>(a3);

    // this + 1 is the sub-object at +4 (address of member, not a pointer load).
    return static_cast<int>(sub_46A3B0(reinterpret_cast<void*>(self + 1), nullptr, args));
}

HOOK(0x0046A990, MissionScript_BindRegionToSceneSlot, "auto");
