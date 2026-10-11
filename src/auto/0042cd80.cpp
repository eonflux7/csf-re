#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bindings (by address)
CSF_FUNC(void, __fastcall, Cheats_SetSubobjectFloat, 0x00428B90, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, Cheats_RefreshOwnerState, 0x0042CA40, (void* self, void* edx));

// Original: char __thiscall sub_42CD80(char** this, float* a2); ret 4
// Forwards the float argument to the embedded sub-object at +0x10, then
// refreshes the owning object (this - 4) and returns its char result.
static char __fastcall Cheats_ApplyFloatThenRefresh(void* self, void* /*edx*/, float* a2)
{
    void* subObject = CSF_AT(void*, self, 0x10);
    Cheats_SetSubobjectFloat(subObject, nullptr, a2);

    void* owner = static_cast<void*>(static_cast<uint8_t*>(self) - 4);
    return Cheats_RefreshOwnerState(owner, nullptr);
}

HOOK(0x0042CD80, Cheats_ApplyFloatThenRefresh, "auto");
