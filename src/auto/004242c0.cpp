#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __fastcall, C_Unknown_LookupEntry, 0x00426D70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_Unknown_ComputeIndexedElement, 0x00425B60, (void* self, void*));
CSF_GLOBAL(float, g_ScaleY_0_6, 0x0082E038);

// Recomputes the clamped range value (+0x5C) and the derived factor (+0x58)
// from the entry returned by C_Unknown_LookupEntry.
static void __fastcall C_Unknown_UpdateClampedRangeFactor(void* self, void* /*edx*/)
{
    uint8_t* obj = static_cast<uint8_t*>(self);
    uint8_t* subObj = obj + 36;

    uint8_t* entry = reinterpret_cast<uint8_t*>(C_Unknown_LookupEntry(subObj, nullptr));
    if (entry == nullptr)
        return;

    // Candidate value: entry float at +0x18 scaled by g_ScaleY_0_6.
    float clampCandidate = CSF_AT(float, entry, 0x18) * g_ScaleY_0_6;

    if (CSF_AT(float, obj, 0x50) <= clampCandidate)
    {
        if (CSF_AT(float, obj, 0x50) < CSF_AT(float, obj, 0x54))
        {
            uint8_t* owner = CSF_AT(uint8_t*, obj, 0x14);
            CSF_AT(uint32_t, obj, 0x58) = CSF_AT(uint32_t, owner, 0x194);

            float rangeValue = CSF_AT(float, obj, 0x54);
            if (rangeValue >= clampCandidate)
                rangeValue = clampCandidate;
            CSF_AT(float, obj, 0x5C) = rangeValue;
        }
    }
    else
    {
        int elemRaw = C_Unknown_ComputeIndexedElement(subObj, nullptr);
        uint8_t* elem = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(elemRaw));

        float span = (clampCandidate + CSF_AT(float, obj, 0x50))
                   * (clampCandidate - CSF_AT(float, obj, 0x50));
        float halfWidth = CSF_AT(float, elem, 0x10) - CSF_AT(float, elem, 0x0C);

        CSF_AT(float, obj, 0x5C) = clampCandidate;
        CSF_AT(float, obj, 0x58) = span / (halfWidth + halfWidth);
    }
}

HOOK(0x004242C0, C_Unknown_UpdateClampedRangeFactor, "auto");
