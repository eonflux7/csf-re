#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char, __fastcall, ProcessLastChildMatchingId, 0x00408500, (void* self, void* , int targetId));
CSF_FUNC(char, __fastcall, C_0082e520_UpdateFromEvent, 0x0041E090, (C_0082e520* self, void* , int));

static char __fastcall C_0082db60_UpdateAndResolvePending(C_0082db60* self, void* /*edx*/, int a2)
{
    char result = (char)self->field_000c;
    if (result)
    {
        C_0082e520_UpdateFromEvent(self, nullptr, a2);

        void* child = reinterpret_cast<void*>(self->field_008c);
        void** childVtbl = CSF_AT(void**, child, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(childVtbl[5])(child, nullptr, a2);

        if (self->field_0090 == 1 || self->field_0090 == 2)
        {
            result = ProcessLastChildMatchingId(reinterpret_cast<void*>(self->field_008c), nullptr, 0);
            if (result)
                self->field_0090 = 0;
        }
        else
        {
            result = (char)(self->field_0090 - 2);
        }
    }
    return result;
}

HOOK(0x0040C850, C_0082db60_UpdateAndResolvePending, "auto");
