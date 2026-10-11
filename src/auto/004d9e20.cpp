#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

// Serializes the link state of an AI node: a2 is the saved-state record, self owns the
// embedded sub-archive at +0x10. Returns 1 when the record is still active, 0 otherwise.
static int __fastcall AiAutonomous_SerializeLinkedState(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* a2)
{
    // Pointer stored at +0x1C: saved parent/link target, restored at the end.
    S_0041f0a0* savedLinkTarget = CSF_AT(S_0041f0a0*, a2, 0x1C);

    // Flag at +0: set when the linked object is not present and the virtual check at slot 32 fails.
    if (CSF_AT(uint8_t, a2, 0x00) == 0)
    {
        void* linkedObj = CSF_AT(void*, a2, 0x04);
        bool linkedObjHasFlag =
            ((bool(__fastcall*)(void*, void*, int, S_0041f0a0*))(CSF_AT(void**, linkedObj, 0)[32]))(
                linkedObj, nullptr, 0, a2);
        CSF_AT(uint8_t, a2, 0x00) = (!linkedObjHasFlag) ? 1 : 0;
    }
    else
    {
        CSF_AT(uint8_t, a2, 0x00) = 0;
    }

    Archive_SerializeBaseFields(self, nullptr, reinterpret_cast<int>(a2));

    CSF_AT(S_0041f0a0*, a2, 0x1C) = nullptr;
    if (CSF_AT(uint8_t, a2, 0x00) == 0)
    {
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        Archive_SerializeObjectRef(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x10),
                                   reinterpret_cast<bool*>(a2));
    }

    CSF_AT(S_0041f0a0*, a2, 0x1C) = savedLinkTarget;

    if (CSF_AT(uint8_t, a2, 0x00) != 0)
    {
        CSF_AT(uint8_t, a2, 0x00) = 1;
        return 1;
    }

    void* linkedObj = CSF_AT(void*, a2, 0x04);
    bool linkedObjIsActive =
        ((bool(__fastcall*)(void*, void*, S_0041f0a0*))(CSF_AT(void**, linkedObj, 0)[31]))(
            linkedObj, nullptr, a2);
    if (!linkedObjIsActive)
    {
        CSF_AT(uint8_t, a2, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, a2, 0x00) = 0;
    return 0;
}

HOOK(0x4D9E20, AiAutonomous_SerializeLinkedState, "auto");
