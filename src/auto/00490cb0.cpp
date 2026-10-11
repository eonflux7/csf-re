#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_00833908_UpdateState, 0x00490720, (void* self, void* edx));

static hook::Tail __fastcall C_00833908_UpdateAndForward(void* self, const uint32_t* /*args*/)
{
    const unsigned int result = C_00833908_UpdateState(self, nullptr);
    auto* object = static_cast<C_00833908*>(self);
    void* child = reinterpret_cast<void*>(object->field_0034);
    if (child)
        return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x9C / 4]);
    return hook::tail_ret(result);
}

HOOK_TAIL(0x00490CB0, C_00833908_UpdateAndForward, "auto");
