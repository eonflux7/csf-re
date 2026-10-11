#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
static CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

static hook::Tail __fastcall C_008317c8_ForwardToResolvedWeapon(
    void* object, const uint32_t* /*args*/)
{
    auto* self = static_cast<C_008317c8*>(object);
    const int state = static_cast<int32_t>(self->field_006c);
    if (state < 0)
        return hook::tail_ret(state);

    const int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64));
    if (!resolved)
        return hook::tail_ret(0);

    self->vSlotE4();
    void* obj = reinterpret_cast<void*>(resolved);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0xA0 / 4]);
}

HOOK_TAIL(0x004782A0, C_008317c8_ForwardToResolvedWeapon, "weapon_firearms");
