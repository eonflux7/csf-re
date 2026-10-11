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

// Forwards to the linked child object (field_008c), and when the flag byte of a2
// is set, looks up the secondary object at field_0008->field_0044. If that object
// reports ready (slot 0xD8), fetches its sub-object (slot 0xD8 again) and asks it
// to run slot 0x30 with argument 0. Returns the last call result.
static int __fastcall C_00834598_ForwardToLinkedAndNotify(C_00834598* self, void* /*edx*/, int a2)
{
    int result = 0;

    uint32_t linkedObject = self->field_008c;
    if (linkedObject != 0)
    {
        void* linked = reinterpret_cast<void*>(linkedObject);
        result = ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, linked, 0)[0x3C / 4]))(linked, nullptr, a2);

        if (static_cast<uint8_t>(a2) != 0)
        {
            C_00833908* secondary = reinterpret_cast<C_00833908*>(self->field_0008);
            uint32_t readyObject = secondary->field_0044;
            if (readyObject != 0)
            {
                void* ready = reinterpret_cast<void*>(readyObject);
                result = ((int(__fastcall*)(void*, void*))(CSF_AT(void**, ready, 0)[0xD8 / 4]))(ready, nullptr);
                if (result != 0)
                {
                    int subObject = ((int(__fastcall*)(void*, void*))(CSF_AT(void**, ready, 0)[0xD8 / 4]))(ready, nullptr);
                    void* sub = reinterpret_cast<void*>(subObject);
                    result = ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, sub, 0)[0x30 / 4]))(sub, nullptr, 0);
                }
            }
        }
    }

    return result;
}

HOOK(0x4A4AA0, C_00834598_ForwardToLinkedAndNotify, "auto");
