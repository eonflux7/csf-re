#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, S_004d1e30_ResetState, 0x004D1E30, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, S_004d1e30_ExecuteAction, 0x004D1ED0, (void* self, void* edx, int a2, float a3, int a4));
CSF_FUNC(int, __fastcall, S_004d1e30_StopAction, 0x004D2910, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

[[maybe_unused]] static const auto g_ResetStateVtableReference =
    CSF_VTBL(0x004D1E30);

static void __fastcall S_004d1e30_UpdateTimer(
    S_004d1e30* self, void* /*edx*/, float elapsedTime)
{
    if (self->field_0044 < g_fZero && !self->field_0048)
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        void* index = CSF_AT(void*, context, 0x1FC0);

        context = reinterpret_cast<void*>(GetValue8B0554());
        void* definitions = CSF_AT(void*, context, 0x1FC0);

        const uint32_t definitionId = self->field_0088;
        void** definitionsVtable = CSF_AT(void**, definitions, 0);
        void** indexVtable = CSF_AT(void**, index, 0);

        int key = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
            indexVtable[12 / 4])(index, nullptr, definitionId);

        void* definition = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            definitionsVtable[8 / 4])(definitions, nullptr, key);

        self->field_0044 =
            g_HalfDurationScale < CSF_AT(float, definition, 0x128)
                ? CSF_AT(float, definition, 0x128)
                : 0.5f;

        self->field_0094 = CSF_AT(int, definition, 0x7C) == 2;
        S_004d1e30_ExecuteAction(self, nullptr, 11, 0.0f,
                                self->field_0094 != 0);

        if (self->field_0094)
            goto UpdateRemainingTime;

        S_004d1e30_StopAction(self, nullptr, 0);
        S_004d1e30_ResetState(self, nullptr);
    }

    if (!self->field_0094)
        return;

UpdateRemainingTime:
    float remainingTime = self->field_0098 - elapsedTime;
    self->field_0098 = remainingTime;
    if (remainingTime <= g_fZero)
    {
        S_004d1e30_StopAction(self, nullptr, 0);
        S_004d1e30_ResetState(self, nullptr);
    }
}

HOOK(0x004D2B40, S_004d1e30_UpdateTimer, "auto");
