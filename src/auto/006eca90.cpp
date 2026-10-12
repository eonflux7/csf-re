#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(int, __fastcall, GetCandidateCount, 0x00489C60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __fastcall, CGuiBuilder_GetEntryByIndex, 0x006BC9F0, (CGuiBuilder* self, void* , int index));
CSF_FUNC(int, __fastcall, C_00833708_GetEntryByIndex, 0x006BCA00, (C_00833708* self, void* , int index));
CSF_FUNC(int, __fastcall, C_00833708_GetCategoryClumpCount, 0x006BCA20, (C_00833708* self, void* , int category));
CSF_FUNC(int, __fastcall, C_00833708_GetCategoryClump, 0x006BCA40, (C_00833708* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, CharActionFsm_GetSecondaryCount, 0x0070E390, (void* self, void* edx));

static char __fastcall C_00862028_VisitCollisionAtomics(
    C_00862028* self, void* /*edx*/, C_00833708* object)
{
    const char enabled = CSF_AT(char, object, 0x91);
    if (!enabled)
        return enabled;

    const auto visitor =
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006ECA40));

    const uint32_t candidateCount =
        static_cast<uint32_t>(GetCandidateCount(object, nullptr));
    for (uint32_t index = 0; index < candidateCount; ++index)
    {
        const int clump = CGuiBuilder_GetEntryByIndex(
            reinterpret_cast<CGuiBuilder*>(object), nullptr,
            static_cast<int>(index));
        RpClumpForAllAtomics(clump, visitor, reinterpret_cast<int>(self));
    }

    const uint32_t secondaryCount =
        static_cast<uint32_t>(CharActionFsm_GetSecondaryCount(object, nullptr));
    for (uint32_t index = 0; index < secondaryCount; ++index)
    {
        const int clump = C_00833708_GetEntryByIndex(
            object, nullptr, static_cast<int>(index));
        RpClumpForAllAtomics(clump, visitor, reinterpret_cast<int>(self));
    }

    for (uint32_t category = 0; category < 3; ++category)
    {
        const uint32_t clumpCount =
            static_cast<uint32_t>(C_00833708_GetCategoryClumpCount(
                object, nullptr, static_cast<int>(category)));
        for (uint32_t index = 0; index < clumpCount; ++index)
        {
            const int clump = C_00833708_GetCategoryClump(
                object, nullptr, static_cast<int>(category),
                static_cast<int>(index));
            RpClumpForAllAtomics(clump, visitor, reinterpret_cast<int>(self));
        }
    }

    return 3;
}

HOOK(0x006ECA90, C_00862028_VisitCollisionAtomics, "auto");
