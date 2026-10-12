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

// Callees of 0x425A00 (bound by address, __thiscall shims).
CSF_FUNC(int, __fastcall, C_0082fdb8_ReleasePending, 0x00425900, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082fdb8_RunPhase1, 0x00425800, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082fdb8_RunPhase2, 0x004255C0, (void* self, void* edx));

// Runs one step of the task's state machine. The stack argument (a2) is unused
// by the original, but the callee pops it (ret 4), so it is kept in the signature.
static int __fastcall C_0082fdb8_RunStateMachine(C_0082fdb8* self, void* /*edx*/, int /*a2*/)
{
    // Pending flag: release it once, then clear it.
    if (self->field_0024)
    {
        C_0082fdb8_ReleasePending(self, nullptr);
        self->field_0024 = 0;
    }

    // State 1 and 2 hand control to their phase routine and return its result.
    if (self->field_0020 == 1)
        return C_0082fdb8_RunPhase1(self, nullptr);
    if (self->field_0020 == 2)
        return C_0082fdb8_RunPhase2(self, nullptr);

    // Default result is state - 3 (zero when state == 3).
    int result = static_cast<int>(self->field_0020) - 3;
    if (self->field_0020 == 3)
    {
        void* waitObject = reinterpret_cast<void*>(self->field_0028);
        if (!waitObject)
        {
            self->field_0020 = 0;
        }
        else
        {
            // vtable slot 60 / 4 = 15: "is still busy" style query returning bool in al.
            result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, waitObject, 0)[15])(waitObject, nullptr);
            if (!static_cast<char>(result))
                self->field_0020 = 0;
        }
    }
    return result;
}

HOOK(0x425A00, C_0082fdb8_RunStateMachine, "auto");
