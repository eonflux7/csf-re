#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (addresses from the packet, copied verbatim).
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(char, __fastcall, S_004ff0f0_SetRecordNameAndLoad, 0x00501A00,
         (S_004ff0f0* self, void* edx, C_0082d410* name));

// Original: char __thiscall sub_512710(CSaveGameManager* this, std::_String_base* a2); ret 4
// a2 is the caller's std::string object (a 4-byte pointer on the stack); it is passed through
// untouched to both callees, so it is carried here as an opaque void* to avoid needing the std type.
static char __fastcall CSaveGameManager_LoadNamedSave(CSaveGameManager* self, void* /*edx*/,
                                                     void* a2) {
    // Copy the name string into the embedded member at +0x94 (npos-length substring from 0).
    AssignStringSubstring(reinterpret_cast<C_0082d410*>(self->pad_0094), nullptr,
                          a2, 0, static_cast<std::uint32_t>(-1));

    // Fetch the global record manager and load the named record.
    S_004ff0f0* recordManager = reinterpret_cast<S_004ff0f0*>(GetGlobalValue_8B0568());
    S_004ff0f0_SetRecordNameAndLoad(recordManager, nullptr, reinterpret_cast<C_0082d410*>(a2));

    return 1;
}

HOOK(0x00512710, CSaveGameManager_LoadNamedSave, "auto");
