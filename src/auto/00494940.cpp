#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall CPlayerCharacter_DispatchField016cLookup(
    void* self, const uint32_t* args)
{
    (void)args;
    auto* player = static_cast<CPlayerCharacter*>(self);

    if (player->field_0228)
        return hook::tail_jump(player, (*reinterpret_cast<void***>(player))[0xA0 / 4]);

    return hook::tail_jump(player, CSF_ADDR_FN(0x0048E230));
}

HOOK_TAIL(0x00494940, CPlayerCharacter_DispatchField016cLookup, "auto");
