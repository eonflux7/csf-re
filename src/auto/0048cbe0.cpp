#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(double, __fastcall, AngleTable_GetAbsoluteAngleForIndex, 0x00406E10, (void* self, void* , int index));
CSF_GLOBAL(float, g_RotationGateAngleTolerance, 0x00833830);

static bool __fastcall TCharTask_CheckRotationGate(
    void* self, void* /*edx*/, char checks)
{
    if ((CSF_AT(uint8_t, self, 0x39) & 3) == 0)
        return true;

    void* rotationProvider =
        CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x4C);
    void* angleTable =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, rotationProvider, 0)[0x3C / 4])(
                rotationProvider, nullptr);

    bool allowed = true;

    if ((checks & 1) != 0 && CSF_AT(int16_t, self, 0x20) > 0)
    {
        void* characterState =
            CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x30);
        allowed = CSF_AT(int32_t, characterState, 0x1FC) != 2;
    }

    if ((checks & 2) != 0 && CSF_AT(int16_t, self, 0x22) > 0)
    {
        allowed = allowed &&
            AngleTable_GetAbsoluteAngleForIndex(angleTable, nullptr, 0)
                <= g_RotationGateAngleTolerance;
    }

    if ((checks & 4) == 0 || CSF_AT(int16_t, self, 0x24) <= 0)
        return allowed;

    return allowed &&
        AngleTable_GetAbsoluteAngleForIndex(angleTable, nullptr, 2)
            <= g_RotationGateAngleTolerance;
}

HOOK(0x0048CBE0, TCharTask_CheckRotationGate, "auto");
