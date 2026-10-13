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

CSF_FUNC(uint32_t*, __fastcall, C_008300c0_CopyRecordsToPad, 0x004288B0, (C_008300c0* self, void* , uint32_t* source, uint32_t count));
CSF_FUNC(C_00860130*, __fastcall, C_008300c0_ApplyControlBindings, 0x00428EC0, (C_008300c0* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());

// Copies 32 records of 5 dwords from `source` into the object's embedded table at +4,
// then, if the global's record holder at +0x1C is present, pushes the same 32 records
// into it and applies its control bindings.
static uint32_t* __fastcall OptionsFile_CopyRecordsAndApplyBindings(uint32_t* self, void* /*edx*/, uint32_t* source)
{
    uint32_t* result = source;
    if (source) {
        uint32_t* tableDest = self + 1;
        uint32_t* cursor = tableDest;
        const uint32_t* src = source;
        int remaining = 32;
        do {
            cursor[0] = src[0];
            cursor[1] = src[1];
            cursor[2] = src[2];
            cursor[3] = src[3];
            cursor[4] = src[4];
            src += 5;
            cursor += 5;
            --remaining;
        } while (remaining);

        uint32_t* globalRecord = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(GetGlobalValue_8B056C()));
        result = globalRecord;
        C_008300c0* recordHolder = reinterpret_cast<C_008300c0*>(globalRecord[7]);
        if (recordHolder) {
            C_008300c0_CopyRecordsToPad(recordHolder, nullptr, tableDest, 32);
            return reinterpret_cast<uint32_t*>(C_008300c0_ApplyControlBindings(recordHolder, nullptr));
        }
    }
    return result;
}

HOOK(0x50B280, OptionsFile_CopyRecordsAndApplyBindings, "auto");
