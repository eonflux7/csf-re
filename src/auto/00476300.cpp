#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall C_008316d8_ForwardToResolvedEntry(
    void* self, const uint32_t* /*args*/)
{
    int entry = static_cast<C_008316d8*>(self)->vResolveEntryLookupValue();
    if (!entry)
        return hook::tail_ret(0, 4);  // eax is the lookup's 0; ret 4

    void* object = reinterpret_cast<void*>(entry);
    object = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x50 / 4])(object, nullptr);
    if (!object)
        return hook::tail_ret(0, 4);  // eax is the lookup's 0; ret 4

    return hook::tail_jump(object, (*reinterpret_cast<void***>(object))[0x04 / 4]);
}

HOOK_TAIL(0x00476300, C_008316d8_ForwardToResolvedEntry, "weapon_firearms");

