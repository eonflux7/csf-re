#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// Game-global accessor used to fetch the sound options' global value object.
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());

// __thiscall callee (object in ecx, one stack argument popped by callee).
CSF_FUNC(void, __fastcall, SetGlobalOptionValue_5231D0, 0x5231D0, (int globalValueObject, void* edx, int value));

// Empty cdecl helper called for its side effects (none).
CSF_FUNC(void, __cdecl, DoNothing_506AC0, 0x506AC0, ());

// Returns the global state object used by the concurrency scheduler lookup.
CSF_FUNC(int, __cdecl, GetGlobalState, 0x50D190, ());

// __thiscall method of Concurrency::details::SchedulerBase (object in ecx, no stack args).
CSF_FUNC(int, __fastcall, GetSchedulerProxy_734A10, 0x734A10, (int schedulerBase, void* edx));

// Stores a new slider value, pushes it to the global option object, and, when the
// pending flag at +0x58 is set and the scheduler proxy reports exactly one, clears the
// flag and invokes the virtual slot at +0x5C. Returns the low byte result via al.
static char __fastcall CMenuSoundOptions_StoreSliderValueAndRefresh(CMenuSoundOptions* self, void* /*edx*/, int sliderValue)
{
    char result = 0;

    self->field_0054 = static_cast<uint32_t>(sliderValue);

    int globalValueObject = GetGlobalValue();
    SetGlobalOptionValue_5231D0(globalValueObject, nullptr, sliderValue);

    DoNothing_506AC0();

    if (self->field_0058 != 0)
    {
        int globalState = GetGlobalState();
        int schedulerProxy = GetSchedulerProxy_734A10(globalState, nullptr);
        int remaining = schedulerProxy - 1;
        result = static_cast<char>(remaining);

        if (remaining == 0)
        {
            self->field_0058 = 0;
            int virtualResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[23])(self, nullptr);
            result = static_cast<char>(virtualResult);
        }
    }

    return result;
}

HOOK(0x53CEE0, CMenuSoundOptions_StoreSliderValueAndRefresh, "auto");
