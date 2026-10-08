#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee (bound by address; a real function start, reached by a tail jump in the original)
CSF_FUNC(int, __fastcall, CMenuMpSessions_ApplySessionControlStates, 0x00560500, (void* self, void* edx));

// Calls the virtual setter at vtbl+0xA8 with one stack argument (ecx = object).
// Returns the callee's eax, which the original leaves as the function's result.
static int CallControlSetter(void* control, int arg)
{
    void** vtbl = CSF_AT(void**, control, 0);
    return ((int(__fastcall*)(void*, void*, int))vtbl[0xA8 / 4])(control, nullptr, arg);
}

// Body of the default-control-states sequence (the code the original reaches via `jmp`
// after the phase check). Each embedded control at a fixed offset of self receives a
// setter call; the controls at +0x78 and +0x90 get a flag derived from self->+0x18 where
// only the low byte is replaced (sete dl), so the upper bytes of the original value are kept.
static int ApplyDefaultControlStatesInline(void* self)
{
    void* control;
    int last = 0;

    control = CSF_AT(void*, self, 0xA8);
    if (control) CallControlSetter(control, 1);

    control = CSF_AT(void*, self, 0x7C);
    if (control) CallControlSetter(control, 1);

    control = CSF_AT(void*, self, 0x88);
    if (control) CallControlSetter(control, 1);

    control = CSF_AT(void*, self, 0x80);
    if (control) CallControlSetter(control, 1);

    control = CSF_AT(void*, self, 0x8C);
    if (control) CallControlSetter(control, 1);

    control = CSF_AT(void*, self, 0x84);
    if (control) CallControlSetter(control, 1);

    {
        uint32_t flagWord = CSF_AT(uint32_t, self, 0x18);
        int flag = static_cast<int>((flagWord & 0xFFFFFF00u) | (flagWord == 0 ? 1u : 0u));
        control = CSF_AT(void*, self, 0x78);
        if (control) CallControlSetter(control, flag);
    }

    {
        uint32_t flagWord = CSF_AT(uint32_t, self, 0x18);
        int flag = static_cast<int>((flagWord & 0xFFFFFF00u) | (flagWord == 0 ? 1u : 0u));
        control = CSF_AT(void*, self, 0x90);
        if (control) CallControlSetter(control, flag);
    }

    control = CSF_AT(void*, self, 0x94);
    if (control) CallControlSetter(control, 0);

    control = CSF_AT(void*, self, 0x24);
    if (control) CallControlSetter(control, 0);

    control = CSF_AT(void*, self, 0x98);
    if (control) CallControlSetter(control, 0);

    control = CSF_AT(void*, self, 0x28);
    if (control) CallControlSetter(control, 0);

    control = CSF_AT(void*, self, 0x9C);
    if (control) CallControlSetter(control, 0);

    control = CSF_AT(void*, self, 0x2C);
    if (control) CallControlSetter(control, 0);

    control = CSF_AT(void*, self, 0x30);
    if (control) CallControlSetter(control, 1);

    // Final control: its call result is what the original returns in eax.
    control = CSF_AT(void*, self, 0xA0);
    if (control) last = CallControlSetter(control, 1);

    return last;
}

// Dispatches on the session phase object stored at +0xAC: calls its vtable slot 171
// (+0x2AC) to get a phase code. Phase 0 applies the session-control states, phase 1
// applies the default control states, any other value is returned as (phase - 1).
static int __fastcall CMenuMpSessions_UpdateSessionPhase(void* self, void* /*edx*/)
{
    void* phaseObj = CSF_AT(void*, self, 0xAC);
    if (!phaseObj)
        return 0;

    int phase = ((int(__fastcall*)(void*, void*))CSF_AT(void**, phaseObj, 0)[171])(phaseObj, nullptr);

    if (phase == 0)
        return CMenuMpSessions_ApplySessionControlStates(self, nullptr);

    int result = phase - 1;
    if (result == 0)
        return ApplyDefaultControlStatesInline(self);

    return result;
}

HOOK(0x005607D0, CMenuMpSessions_UpdateSessionPhase, "auto");
