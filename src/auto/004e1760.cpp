#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binding: first callee, takes ecx = object (uint32 handle), returns byte-ish flag
CSF_FUNC(int, __fastcall, QueryObjectLockedState_48E210, 0x0048E210, (void* self, void* edx));
// Binding: linked-object state reader on C_0082ddf8
CSF_FUNC(char, __fastcall, GetLinkedObjectState_40F490, 0x0040F490, (void* self, void* edx));
// Binding: base-class message dispatcher (already reimplemented as unnamed sub)
CSF_FUNC(int, __fastcall, DispatchBaseMessage_4E1180, 0x004E1180, (void* self, void* edx, uint32_t* a2));
// Binding: predicate on an action code
CSF_FUNC(int, __cdecl, IsActionCodeEnabled_761260, 0x00761260, (unsigned int a1));

static int __fastcall C_0083e418_HandleCountdownMessage(C_0083e418* self, void* /*edx*/, uint32_t* a2)
{
    int result = QueryObjectLockedState_48E210(reinterpret_cast<void*>(self->field_000c), nullptr);
    if (!static_cast<uint8_t>(result))
    {
        if (a2[5] == 21 && static_cast<uint8_t>(IsActionCodeEnabled_761260(a2[3])))
        {
            uint32_t delay = a2[4];
            int32_t remaining = static_cast<int32_t>(self->field_0018) - static_cast<int32_t>(delay);
            bool reachedZero = (self->field_0018 == delay);
            self->field_0018 = static_cast<uint32_t>(remaining);
            if (remaining < 0 || reachedZero)
            {
                uint8_t* linkedHolder = reinterpret_cast<uint8_t*>(self->field_000c);
                self->field_0018 = 0;
                C_0082ddf8* linked = reinterpret_cast<C_0082ddf8*>(CSF_AT(uint32_t, linkedHolder, 0x4C));
                GetLinkedObjectState_40F490(linked, nullptr);
            }
        }
        return DispatchBaseMessage_4E1180(self, nullptr, a2);
    }
    return result;
}

HOOK(0x004E1760, C_0083e418_HandleCountdownMessage, "auto");
