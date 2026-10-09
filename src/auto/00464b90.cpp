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

CSF_FUNC(char, __fastcall, GatherEntryRecordsIntoOutput, 0x00462C00, (S_00462c00* self, void* , int queryKey, char queryFlagA, char queryFlagB, S_0069b1c0* outputList));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Referenced per the packet's vtable list. This is a dead constant (no store), so behavior is unchanged.
[[maybe_unused]] static const void* const kS00462c00VtableRef = CSF_VTBL(0x00462C00);

// Resolves a table entry (optionally via the current scene's table owner when bit 0x800000 is set)
// and forwards the query key and entry to GatherEntryRecordsIntoOutput with flags 1 and 0.
static char __fastcall S_00462c00_DispatchEntryWithSceneOverride(S_00462c00* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    void* requestHeader = reinterpret_cast<void*>(a4[0]);   // *a4
    void* requestBody = reinterpret_cast<void*>(a4[1]);     // a4[1]
    uint32_t controlWord = CSF_AT(uint32_t, requestBody, 8);

    void* operandNode = CSF_AT(void*, requestHeader, 4);     // *(*a4 + 4)
    int queryKey = CSF_AT(int, operandNode, 4);              // *(*(*a4 + 4) + 4)

    uint32_t ownerAddr = self->field_0020;
    if ((controlWord & 0x800000u) != 0)
    {
        void* sceneRecord = CSF_AT(void*, g_pCurrentScene, 0xBC);
        ownerAddr = CSF_AT(uint32_t, sceneRecord, 0x38);
        controlWord &= ~0x800000u;
    }

    void* tableOwner = reinterpret_cast<void*>(ownerAddr);
    void* entryTable = CSF_AT(void*, tableOwner, 8);
    S_0069b1c0* entry = CSF_AT(S_0069b1c0*, entryTable, 12 * controlWord + 4);

    return GatherEntryRecordsIntoOutput(self, nullptr, queryKey, 1, 0, entry);
}

HOOK(0x00464B90, S_00462c00_DispatchEntryWithSceneOverride, "auto");
