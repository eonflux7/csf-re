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

// Fills a 8-byte output record { float value; uint32_t tag; } at outRecord.
// When the player flag (field_0228) is set, the value comes from the sub-object
// returned by vGetField016c(), read through two virtual getters; otherwise the
// original delegates to virtual slot 0xAC with the output pointer.
static int __fastcall CPlayerCharacter_FillValueAndTagOut(CPlayerCharacter* self, void* /*edx*/, int outRecord)
{
    if (self->field_0228)
    {
        void* subObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->vGetField016c()));
        void* innerObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[6])(subObject, nullptr);
        uint32_t tag = CSF_AT(uint32_t, self, 0x1b0);
        float value = ((float (__fastcall*)(void*, void*))CSF_AT(void**, innerObject, 0)[10])(innerObject, nullptr);

        uint8_t* out = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(outRecord));
        CSF_AT(float, out, 0) = value;
        CSF_AT(uint32_t, out, 4) = tag;
        return outRecord;
    }

    ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[43])(self, nullptr, outRecord);
    return outRecord;
}

HOOK(0x494BC0, CPlayerCharacter_FillValueAndTagOut, "auto");
