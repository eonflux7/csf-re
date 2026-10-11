#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetIndexedTableValue_6AF8A0, 0x006AF8A0, (void* self, void* , int index));
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall C_008670f0_CenterVerticalPosition(
    C_008670f0* self, void* /*edx*/, int destination, int source,
    uint32_t* descriptor, float availableHeight)
{
    float height = g_fZero;
    if (descriptor[0] == 0)
    {
        void* object = reinterpret_cast<void*>(self->field_0014);
        height = static_cast<float>(
            reinterpret_cast<double(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 1));
    }
    else if (descriptor[0] == 1)
    {
        int entry = GetIndexedTableValue_6AF8A0(
            reinterpret_cast<void*>(self->field_0008), nullptr,
            static_cast<int>(descriptor[1]));
        if (entry)
        {
            void* object = reinterpret_cast<void*>(entry);
            void* dimensions =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x40 / 4])(object, nullptr);
            height = CSF_AT(float, dimensions, 4);
        }
        else
        {
            height = g_fZero;
        }
    }

    uint32_t horizontal = CSF_AT(uint32_t, reinterpret_cast<void*>(source), 0);
    float vertical = (availableHeight - height) * g_HalfDurationScale
        + CSF_AT(float, reinterpret_cast<void*>(source), 4);
    CSF_AT(uint32_t, reinterpret_cast<void*>(destination), 0) = horizontal;
    CSF_AT(float, reinterpret_cast<void*>(destination), 4) = vertical;
    return destination;
}

HOOK(0x007336B0, C_008670f0_CenterVerticalPosition, "auto");
