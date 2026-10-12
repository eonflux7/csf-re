#include <cstdint>

#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Scene lookup: resolves an id to its sub-object (null if absent).
CSF_FUNC(int, __fastcall, SceneFindSubObject, 0x004DE7A0, (void* self, void* edx, unsigned int a2));

// Path search between two resolved sub-objects (__thiscall, 8 stack args, ret 32).
CSF_FUNC(char, __fastcall, SearchPathBetweenSubObjects, 0x004E05E0,
         (void* self, void* edx, float* a2, int a3, int a4, int a5, uint32_t* a6, char a7, int* a8, int a9));

// Resolves both ids in the scene. Fails if either is missing or flagged with bit 0
// (byte at +4). Otherwise forwards everything to the path search.
static char __fastcall S_004dfbc0_FindPath(S_004dfbc0* self, void* /*edx*/,
                                           unsigned int a2, int a3, uint32_t* a4,
                                           char a5, int* a6, int a7)
{
    int subObjectFrom = SceneFindSubObject(reinterpret_cast<void*>(self->field_0004), nullptr, a2);
    int subObjectTo = SceneFindSubObject(reinterpret_cast<void*>(self->field_0004), nullptr, static_cast<unsigned int>(a3));

    if (!subObjectFrom || !subObjectTo ||
        (CSF_AT(uint8_t, subObjectFrom, 4) & 1) != 0 ||
        (CSF_AT(uint8_t, subObjectTo, 4) & 1) != 0)
    {
        return 0;
    }

    return SearchPathBetweenSubObjects(self, nullptr,
                                       reinterpret_cast<float*>(a2), a3,
                                       subObjectFrom, subObjectTo,
                                       a4, a5, a6, a7);
}

HOOK(0x004E06C0, S_004dfbc0_FindPath, "auto");
