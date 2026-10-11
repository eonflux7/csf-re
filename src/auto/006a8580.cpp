#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Resets a connection/session state machine when its sub-state is ready.
// Two-stage path: releases the session object at +0x90 via its vtable slot 0 (scalar deleting dtor, arg 1)
// and returns that call's result (or the sub-state value when there is no session object).
static int __fastcall GatheringDDL_ResetStateIfReady(void* self, void* /*edx*/, int /*a2*/)
{
    int result = CSF_AT(int, self, 0x08);
    if (result == 3 && CSF_AT(int, self, 0x0C) == 3) {
        void* sessionObj = CSF_AT(void*, self, 0x90);
        result = CSF_AT(int, sessionObj, 0x0C);
        if (result != 1) {
            if (result <= 1 || result > 3) {
                CSF_AT(uint32_t, self, 0x2B0) = 0x494c3133;
                CSF_AT(int, self, 0x08) = 2;
                CSF_AT(int, self, 0x0C) = 0;
                CSF_AT(uint8_t, self, 0x334) = 0;
                if (!sessionObj) {
                    return result;
                }
            } else {
                CSF_AT(int, self, 0x08) = 2;
                CSF_AT(int, self, 0x0C) = 0;
                CSF_AT(uint8_t, self, 0x334) = 0;
                if (!sessionObj) {
                    return result;
                }
            }
            result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, sessionObj, 0)[0])(sessionObj, nullptr, 1);
            CSF_AT(uint32_t, self, 0x90) = 0;
        }
    }
    return result;
}

HOOK(0x006A8580, GatheringDDL_ResetStateIfReady, "auto");
