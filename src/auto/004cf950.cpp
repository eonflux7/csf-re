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

// Advances the C_0083b4a0 state machine using the object held in field_0014.
// State 1 -> 3 when the probed object reports not busy; state 2 -> 0 under the same test.
// Any other state just returns (state - 2) unchanged.
static int __fastcall C_0083b4a0_AdvanceStateFromChild(C_0083b4a0* self, void* /*edx*/, int /*a2*/) {
    int result = 0;

    if (self->field_0044 == 1) {
        // this->field_0014->vGetField016c()   (vtable slot 0x28 / 4 = 10)
        void* child = reinterpret_cast<void*>(self->field_0014);
        void* fieldObj = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[10])(child, nullptr);

        // (*(obj + 76))(obj)  -> slot 0x4C / 4 = 19
        void* probe = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, fieldObj, 0)[19])(fieldObj, nullptr);
        result = static_cast<int>(reinterpret_cast<intptr_t>(probe));

        if (probe) {
            // (*(obj + 28))(obj)  -> slot 0x1C / 4 = 7 ; tested with test al, al
            bool isBusy = ((bool (__fastcall*)(void*, void*))CSF_AT(void**, probe, 0)[7])(probe, nullptr);
            result = isBusy;
            if (!isBusy) {
                self->field_0044 = 3;
            }
        }
    } else {
        result = static_cast<int>(self->field_0044) - 2;
        if (self->field_0044 == 2) {
            void* child = reinterpret_cast<void*>(self->field_0014);
            void* fieldObj = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[10])(child, nullptr);

            void* probe = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, fieldObj, 0)[19])(fieldObj, nullptr);
            result = static_cast<int>(reinterpret_cast<intptr_t>(probe));

            if (probe) {
                bool isBusy = ((bool (__fastcall*)(void*, void*))CSF_AT(void**, probe, 0)[7])(probe, nullptr);
                result = isBusy;
                if (!isBusy) {
                    self->field_0044 = 0;
                }
            }
        }
    }

    return result;
}

HOOK(0x004CF950, C_0083b4a0_AdvanceStateFromChild, "auto");
