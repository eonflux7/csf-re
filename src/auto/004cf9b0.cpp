#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual forwarding slots of the object held in C_0083b4a0::field_0014.
// 0x8C / 4 = 35 : forward request (type 2 path)
// 0x90 / 4 = 36 : forward request (type 25 path)
using ForwardRequestFn = int(__fastcall*)(void* self, void* edx, uint32_t a, uint32_t b, int c, int d);

static int __fastcall C_0083b4a0_ForwardRequestIfMatched(C_0083b4a0* self, void* /*edx*/, uint32_t* a2)
{
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(a2));

    if (a2[0] == 2)
    {
        self->field_0018 |= 1u;
        void* target = reinterpret_cast<void*>(self->field_0014);
        ForwardRequestFn forwardRequest =
            reinterpret_cast<ForwardRequestFn>(CSF_AT(void**, target, 0)[35]);
        return forwardRequest(target, nullptr, a2[0], a2[2], -1, 0);
    }
    else if (a2[0] == 25)
    {
        self->field_0018 |= 1u;
        void* target = reinterpret_cast<void*>(self->field_0014);
        ForwardRequestFn forwardRequest90 =
            reinterpret_cast<ForwardRequestFn>(CSF_AT(void**, target, 0)[36]);
        return forwardRequest90(target, nullptr, a2[1], a2[2], -1, 0);
    }
    return result;
}

HOOK(0x004CF9B0, C_0083b4a0_ForwardRequestIfMatched, "auto");
