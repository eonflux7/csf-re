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

// Calls the flag virtual (slot 0x34) and keeps its full eax value: the original
// returns eax as left by that call when the flag is set or field_002c is null.
// If the flag is clear and field_002c is set, copies the 3-dword record fetched
// from that object (vtable slot 4) into this+0x30..0x38 and returns its last dword.
static int __fastcall C_00862598_CopyTripleIfNotFlagged(C_00862598* self, void* /*edx*/, int /*a2*/)
{
    int flagResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x34 / 4])(self, nullptr);
    int result = flagResult;
    if (!((uint8_t)flagResult))
    {
        uint32_t recordOwner = self->field_002c;
        if (recordOwner)
        {
            void* ownerObject = reinterpret_cast<void*>(recordOwner);
            uint32_t* record = ((uint32_t*(__fastcall*)(void*, void*))
                                CSF_AT(void**, ownerObject, 0)[4])(ownerObject, nullptr);
            CSF_AT(uint32_t, self, 0x30) = record[0];
            CSF_AT(uint32_t, self, 0x34) = record[1];
            CSF_AT(uint32_t, self, 0x38) = record[2];
            result = (int)record[2];
        }
    }
    return result;
}

HOOK(0x6F2670, C_00862598_CopyTripleIfNotFlagged, "auto");
