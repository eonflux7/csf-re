#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall C_008316d8_ForwardToResolvedEntry(
    void* self, const uint32_t* /*args*/)
{
    auto* weapon = static_cast<C_008316d8*>(self);
    if (weapon->vResolveEntryLookupValue() == 0)
        return hook::tail_ret(0, 4);  // eax is the lookup's 0; ret 4

    void* entry = reinterpret_cast<void*>(weapon->vResolveEntryLookupValue());
    return hook::tail_jump(entry, (*reinterpret_cast<void***>(entry))[0x64 / 4]);
}

HOOK_TAIL(0x00475B60, C_008316d8_ForwardToResolvedEntry, "auto");

