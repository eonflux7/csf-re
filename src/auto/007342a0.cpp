#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, PrepareState, 0x007333D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, PrepareDispatch, 0x00734220, ());

static hook::Tail __fastcall C_008670f0_PrepareAndDispatch(
    void* self, const uint32_t* /*args*/)
{
    auto* object = static_cast<C_008670f0*>(self);
    PrepareState(object, nullptr);
    if (!object->field_0018)
        return hook::tail_ret(0);

    PrepareDispatch();
    return hook::tail_jump(
        object, (*reinterpret_cast<void***>(object))[0x1C / 4]);
}

HOOK_TAIL(0x007342A0, C_008670f0_PrepareAndDispatch, "auto");
