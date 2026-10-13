#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, PrepareSubObjectForDispatch, 0x006FD1F0, (void* self, void* edx));

static hook::Tail __fastcall C_00862390_ForwardSlot44Dispatch(void* self, const uint32_t* /*args*/)
{
    C_00862390* owner = static_cast<C_00862390*>(self);

    PrepareSubObjectForDispatch(reinterpret_cast<void*>(owner->field_0020), nullptr);

    void* target = reinterpret_cast<void*>(owner->field_0020);
    return hook::tail_jump(target, (*reinterpret_cast<void***>(target))[0x44 / 4]);
}

HOOK_TAIL(0x006F0E40, C_00862390_ForwardSlot44Dispatch, "game");
