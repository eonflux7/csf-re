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

// Consumes a pending "wait" request on the controller object reached through
// this->field_0014->field_0044. If the controller's slot 47 call (argument 30)
// does not return -1, the task is marked as ready and its timer is cleared.
// Afterwards, if the task is ready, its field_0074 is set to (a2 == 0).
// Returns a full 32-bit BOOL-style value (the original zeroes eax before setcc).
static int __fastcall C_0083dd58_UpdatePendingState(C_0083dd58* self, void* /*edx*/, char a2)
{
    if (self->field_0091)
    {
        uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
        void* controller = CSF_AT(void*, owner, 0x44);
        int status = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, controller, 0)[0xBC / 4])(controller, nullptr, 30);
        if (status != -1)
        {
            self->field_0092 = 1;
            self->field_0074 = 0;
        }
        self->field_0091 = 0;
    }

    int result = static_cast<int>(self->field_0092);
    if (self->field_0092)
    {
        result = (a2 == 0) ? 1 : 0;
        self->field_0074 = static_cast<uint32_t>(result);
    }
    return result;
}

HOOK(0x004DA7D0, C_0083dd58_UpdatePendingState, "auto");
