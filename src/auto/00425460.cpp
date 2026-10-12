#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0041f0a0_Serialize, 0x0041F0A0, (S_0041f0a0* self, void* , S_0041f0a0* stream));
CSF_FUNC(int, __fastcall, TaskStates_WalkVisitorOverFiveSubobjects, 0x0041F200, (void* self, void* , uint8_t* state));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall S_0041f0a0_SerializeWithVisitor(
    S_0041f0a0* self, void* /*edx*/, uint8_t* state)
{
    // The packet also labels the direct Serialize callee as a vtable reference.
    (void)CSF_VTBL(0x0041F0A0);

    bool failed = false;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* visitor = CSF_AT(void*, state, 0x04);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, uint8_t*)>(
            CSF_AT(void**, visitor, 0)[0x80 / 4])(
                visitor, nullptr, 0, state);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0) = failed;
    S_0041f0a0_Serialize(self, nullptr, reinterpret_cast<S_0041f0a0*>(state));

    bool canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canContinue)
    {
        void* visitor = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint8_t*, uint8_t*)>(
                CSF_AT(void**, visitor, 0)[0x58 / 4])(
                    visitor, nullptr, &CSF_AT(uint8_t, self, 0x1C), state);
        CSF_AT(uint8_t, state, 0) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(&CSF_AT(uint8_t, self, 0x20)),
            reinterpret_cast<S_00691860*>(state));
    }

    canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canContinue)
    {
        void* visitor = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint8_t*, uint8_t*)>(
                CSF_AT(void**, visitor, 0)[0x38 / 4])(
                    visitor, nullptr, &CSF_AT(uint8_t, self, 0x34), state);
        CSF_AT(uint8_t, state, 0) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Archive_SerializeObjectRef(&self->field_0040, reinterpret_cast<bool*>(state));
    }

    canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        TaskStates_WalkVisitorOverFiveSubobjects(
            &CSF_AT(uint8_t, self, 0x44), nullptr, state);
    }

    canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (canContinue)
    {
        void* visitor = CSF_AT(void*, state, 0x04);
        // The original tests AL but forwards the complete EAX on success.
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint8_t*)>(
                CSF_AT(void**, visitor, 0)[0x7C / 4])(
                    visitor, nullptr, state);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, state, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x00425460, S_0041f0a0_SerializeWithVisitor, "auto");
