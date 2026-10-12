#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void, __cdecl, HudStatusIcons_UpdateIcon, 0x00458210, (int a1, float a2, int a3));
CSF_FUNC(uint32_t*, __cdecl, HudStatusIcons_ShiftIconsLeft, 0x004583D0, (char* a1, char* a2, uint32_t* a3));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static int __fastcall S_00458f40_UpdateStatusIcons(
    S_00458f40* self, void* /*edx*/, float deltaTime)
{
    int count = 0;
    if (self->field_000c)
        count = static_cast<int32_t>(self->field_0010 - self->field_000c) / 40;

    int result = count - 1;
    if (result < 0)
        return result;

    uint32_t offset = 40 * static_cast<uint32_t>(result);
    int remaining = result + 1;

    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_000c + offset);
        bool active = CSF_AT(int32_t, entry, 0x10) == -1082130432;

        if (!active)
        {
            CSF_AT(float, entry, 0x14) =
                deltaTime + CSF_AT(float, entry, 0x14);

            entry = reinterpret_cast<uint8_t*>(self->field_000c + offset);
            // The original also takes the active branch for unordered comparisons.
            active = !(CSF_AT(float, entry, 0x14) >=
                       CSF_AT(float, entry, 0x10));
        }

        if (active)
        {
            int resolved = CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                reinterpret_cast<int>(entry + 0x1C));

            void* object = reinterpret_cast<void*>(resolved);
            float* position;
            if (object &&
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr))
            {
                position = GetObjectPositionVector(object, nullptr);
            }
            else
            {
                position = &g_ZeroVectorX;
            }

            HudStatusIcons_UpdateIcon(
                static_cast<int>(self->field_000c + offset),
                deltaTime, reinterpret_cast<int>(position));
        }
        else
        {
            HudStatusIcons_ShiftIconsLeft(
                reinterpret_cast<char*>(entry + 40),
                reinterpret_cast<char*>(self->field_0010),
                reinterpret_cast<uint32_t*>(entry));

            uint32_t end = self->field_0010;
            uint32_t trailingEntry = end - 40;
            while (trailingEntry != end)
            {
                CSF_AT(void*, reinterpret_cast<void*>(trailingEntry), 0x1C) =
                    CSF_VTBL(0x0082D410);
                trailingEntry += 40;
            }
            self->field_0010 -= 40;
        }

        offset -= 40;
        result = --remaining;
    }
    while (remaining);

    return result;
}

HOOK(0x00459140, S_00458f40_UpdateStatusIcons, "auto");
