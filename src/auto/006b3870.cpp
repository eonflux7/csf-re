#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_6B30B0, 0x006B30B0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00861c08_SetStateFlags, 0x006E68E0, (void* self, void* edx, uint8_t stateFlags, char enableFlag));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, C_008656d0_ApplyEntryParameters, 0x006C63D0, (C_008656d0* self, void* edx, int entryAddress));

[[maybe_unused]] static const auto kGfxSkinVtbl = CSF_VTBL(0x006B30B0);

// Returns nonzero when at least one of the object's entries accepted its parameters.
static char __fastcall GfxSkin_CheckObjectEligibility(S_006b30b0* self, void* /*edx*/)
{
    if (self->field_000c == 0 || (self->field_0064 & 0x140) != 0x140)
        return 0;

    char stateFlags = static_cast<char>(sub_6B30B0(self, nullptr));

    void* ownerObject = reinterpret_cast<void*>(self->field_0028);
    if (ownerObject)
    {
        // Virtual slot 0x24 / 4 = 9, stdcall taking one argument (1).
        auto ownerCall = reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownerObject, 0)[9]);
        ownerCall(ownerObject, nullptr, 1);
    }

    char anyApplied = 0;
    for (unsigned int index = 0; index < self->field_001c; ++index)
    {
        auto* entries = reinterpret_cast<C_00861c08**>(self->field_0018);
        C_00861c08* entry = entries[index];
        if (entry)
        {
            C_00862490* entryHandle = reinterpret_cast<C_00862490*>(entry);
            C_00861c08_SetStateFlags(entry, nullptr, static_cast<uint8_t>(stateFlags), 1);
            int valuePointer = Handle_GetValuePointer(entryHandle, nullptr);
            auto* globalValue = reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
            anyApplied = static_cast<char>(anyApplied | C_008656d0_ApplyEntryParameters(globalValue, nullptr, valuePointer));
        }
    }
    return anyApplied;
}

HOOK(0x006B3870, GfxSkin_CheckObjectEligibility, "auto");
