#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* , std::int32_t candidate));
CSF_FUNC(bool, __fastcall, sub_494E50, 0x00494E50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Hud_GetAlternateSceneSelection, 0x004950B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_InitializeFallbackQuery, 0x004B93B0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_4F13B0, 0x004F13B0, (void* self, void* edx, int a2, int a3, uint32_t* a4));

static char __fastcall C_0083eb70_CanUseOnTarget(C_0083eb70* self, void* /*edx*/, int a2)
{
    alignas(8) uint8_t queryBytes[0x28];

    uint8_t* target = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CHandleTable_Resolve(g_pCurrentScene, nullptr, a2)));
    if (!target
        || CSF_AT(uint32_t, target, 0x50)
        || !(uint8_t)IsDistinctNonSentinelValue(
               reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr,
               (std::int32_t)CSF_AT(uint32_t, target, 0x78))
        || !Hud_GetAlternateSceneSelection(target, nullptr))
    {
        return 0;
    }

    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));
    uint32_t state = CSF_AT(uint32_t, owner, 0x1D8);
    if (state == 2)
        return 1;
    if (state == 3)
        return 1;

    AiCore_InitializeFallbackQuery(queryBytes, nullptr);

    owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));
    void* query = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, owner, 0x40)));
    if (query) {
        if (sub_4F13B0(query, nullptr, 2, a2, reinterpret_cast<uint32_t*>(queryBytes)))
            return 1;
    }

    if (sub_494E50(target, nullptr))
        return 1;
    return 0;
}

HOOK(0x004E7E30, C_0083eb70_CanUseOnTarget, "auto");
