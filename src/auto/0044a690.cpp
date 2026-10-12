#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30,
         (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, S_00448830_FindControlGroup, 0x0044A580,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, FindEntryByHandle, 0x00449390,
         (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_NegativeAngularLimit, 0x00831168);
CSF_GLOBAL(float, g_PositiveAngularLimit, 0x0083116C);

static int __fastcall S_00448830_MarkAngularExtreme(
    S_00448830* self, void* /*edx*/, int handle)
{
    void* control = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!control)
        return 0;

    if ((CSF_AT(uint32_t, control, 0x170) & 0x2000) == 0)
        return 0;

    const int groupId = CSF_AT(int, control, 0x1F4);
    if (groupId == -1)
        return 0;

    void* group = reinterpret_cast<void*>(
        S_00448830_FindControlGroup(self, nullptr, groupId));
    void* entry = reinterpret_cast<void*>(
        FindEntryByHandle(group, nullptr, handle));

    bool positiveExtreme =
        CSF_AT(float, entry, 0x20) > g_fZero &&
        CSF_AT(float, entry, 0x20) < g_PositiveAngularLimit;
    bool negativeExtreme =
        CSF_AT(float, entry, 0x20) < g_fZero &&
        CSF_AT(float, entry, 0x20) > g_NegativeAngularLimit;

    if (!positiveExtreme && !negativeExtreme)
        return 0;

    const int count = CSF_AT(int, group, 0x0C);
    if (static_cast<uint32_t>(count) != 0)
    {
        auto* entries = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, group, 0x04));

        for (uint32_t index = 0; index < static_cast<uint32_t>(count); ++index)
        {
            void* candidate = entries + 36 * index;
            if (CSF_AT(int, candidate, 0x04) != CSF_AT(int, entry, 0x04) &&
                CSF_AT(int, candidate, 0x14) == CSF_AT(int, entry, 0x14))
            {
                positiveExtreme =
                    positiveExtreme &&
                    !(CSF_AT(float, entry, 0x20) <
                      CSF_AT(float, candidate, 0x20));
                negativeExtreme =
                    negativeExtreme &&
                    CSF_AT(float, entry, 0x20) <=
                        CSF_AT(float, candidate, 0x20);
            }
        }
    }

    int result = 0;
    if (positiveExtreme)
    {
        CSF_AT(int, entry, 0x0C) = 1;
        result = 1;
    }
    if (negativeExtreme)
    {
        CSF_AT(int, entry, 0x0C) = 2;
        return 2;
    }
    return result;
}

HOOK(0x0044A690, S_00448830_MarkAngularExtreme, "auto");
