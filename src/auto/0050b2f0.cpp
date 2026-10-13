#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address with the exact signatures from the packet.
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(uint32_t*, __fastcall, C_008300c0_CopyRecordsToPad, 0x004288B0,
         (C_008300c0* self, void* /*edx*/, uint32_t* source, uint32_t count));
CSF_FUNC(C_00860130*, __fastcall, C_008300c0_ApplyControlBindings, 0x00428EC0,
         (C_008300c0* self, void* /*edx*/));

// Copies 32 dwords from (a1 + 4) into the record pad of the options object
// returned by GetGlobalValue_8B056C, then applies the control bindings.
// If that object has no record pad (field 0x1C is null), returns the global
// value unchanged.
static int __fastcall OptionsFile_CopyPadAndApplyBindings(void* a1, void* /*a2*/)
{
    int result = GetGlobalValue_8B056C();
    C_008300c0* recordPad = CSF_AT(C_008300c0*, reinterpret_cast<uint8_t*>(result), 0x1C);

    if (recordPad) {
        uint32_t* source = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(a1) + 4);
        C_008300c0_CopyRecordsToPad(recordPad, nullptr, source, 32);
        // Tail call in the original: its return value is passed through.
        return reinterpret_cast<int>(C_008300c0_ApplyControlBindings(recordPad, nullptr));
    }
    return result;
}

HOOK(0x0050B2F0, OptionsFile_CopyPadAndApplyBindings, "auto");
