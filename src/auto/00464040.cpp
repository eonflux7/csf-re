#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Polls the request state record `a2` (passed on the stack, stdcall-style ret 4).
// The record holds a byte status at +0, a sub-object pointer at +4, and an int at +0x1C.
// Returns 1 when the request is still pending, 0 when it has finished.
static int __fastcall C_00831414_PollRequestStatus(C_00831414* self, void* /*edx*/, int a2)
{
    void* state = reinterpret_cast<void*>(a2);

    // v3 = !*a2 && !obj->vtbl[32](obj, 0, a2)
    uint8_t probeDone = 0;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* obj = CSF_AT(void*, state, 4);
        auto probe = reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int, int)>(CSF_AT(void**, obj, 0)[32]);
        probeDone = (probe(obj, nullptr, 0, a2) == 0) ? 1 : 0;
    }

    int savedField1C = CSF_AT(int, state, 0x1C);
    CSF_AT(uint8_t, state, 0) = probeDone;

    uint32_t requestOffset = self->field_0010 + 4;
    CSF_AT(int, state, 0x1C) = 0;

    if (!probeDone)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;

        void* obj = CSF_AT(void*, state, 4);
        auto query = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t, int)>(CSF_AT(void**, obj, 0)[18]);
        CSF_AT(uint8_t, state, 0) = (query(obj, nullptr, requestOffset, a2) == -1) ? 1 : 0;
    }

    bool stillNotDone = (CSF_AT(uint8_t, state, 0) == 0);
    CSF_AT(int, state, 0x1C) = savedField1C;

    if (stillNotDone)
    {
        void* obj = CSF_AT(void*, state, 4);
        auto isFinished = reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[31]);
        if (isFinished(obj, nullptr, a2))
        {
            CSF_AT(uint8_t, state, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x00464040, C_00831414_PollRequestStatus, "auto");
