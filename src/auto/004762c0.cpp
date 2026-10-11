#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static hook::Tail __fastcall WeaponFirearms_ForwardToAssociatedObject(
    void* self, const uint32_t* /*args*/)
{
    void* object =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xA8 / 4])(self, nullptr);

    if (!object)
        return hook::tail_ret(0);

    return hook::tail_jump(
        object, (*reinterpret_cast<void***>(object))[0x38 / 4]);
}

HOOK_TAIL(0x004762C0, WeaponFirearms_ForwardToAssociatedObject, "weapon_firearms");
