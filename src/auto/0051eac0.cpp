#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall C_00840f48_UpdateIfActive(
    void* object, const uint32_t* /*args*/)
{
    auto* self = static_cast<C_00840f48*>(object);
    const char active = self->vSlot14();
    if (!active)
        return hook::tail_ret(0);

    self->vSlot10();
    return hook::tail_jump(
        object, (*reinterpret_cast<void***>(object))[0xC / 4]);
}

HOOK_TAIL(0x0051EAC0, C_00840f48_UpdateIfActive, "fx_particle_manager");
