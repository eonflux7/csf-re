#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CMenuLoading_ApplyEntryTransformsAndDecrementIndex, 0x00538740,
         (CMenuLoading* self, void* edx, int context, int* remaining, int section));

static char __fastcall CMenuLoading_ProcessEnabledLoadingSections(
    CMenuLoading* self, void* /*edx*/)
{
    if (!self->field_0054)
        return 0;
    if (!self->field_0051)
        return 0;

    void* loadingObject = reinterpret_cast<void*>(self->field_0054);
    int context =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, loadingObject, 0)[0xDC / 4])(
                loadingObject, nullptr);

    uint8_t firstEnabled = self->field_0058;
    int remaining = 2;
    if (firstEnabled)
        CMenuLoading_ApplyEntryTransformsAndDecrementIndex(self, nullptr, context, &remaining, 0);

    if (self->field_0059)
        CMenuLoading_ApplyEntryTransformsAndDecrementIndex(self, nullptr, context, &remaining, 1);

    if (!self->field_005a)
        return 0;

    return static_cast<char>(
        CMenuLoading_ApplyEntryTransformsAndDecrementIndex(self, nullptr, context, &remaining, 2));
}

HOOK(0x00538870, CMenuLoading_ProcessEnabledLoadingSections, "auto");
