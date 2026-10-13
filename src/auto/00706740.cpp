#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static char __fastcall C_008632c0_DispatchPointQuery(
    C_008632c0* self, void* /*edx*/, int x, int y, float duration)
{
    ((void(__fastcall*)(C_008632c0*, void*))self->vtbl[0x58 / 4])(
        self, nullptr);

    void* guard = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x24));
    if (!((bool(__fastcall*)(void*, void*, int, int, float))
              CSF_AT(void**, guard, 0)[0x10 / 4])(
            guard, nullptr, x, y, duration + g_HalfDurationScale))
    {
        return 0;
    }

    ((void(__fastcall*)(C_008632c0*, void*))self->vtbl[0x5C / 4])(
        self, nullptr);

    void* delegate = reinterpret_cast<void*>(self->field_004c);
    if (delegate)
    {
        return ((char(__fastcall*)(void*, void*, int, int, float))
                    CSF_AT(void**, delegate, 0)[0x10 / 4])(
            delegate, nullptr, x, y, 0.0f);
    }

    char result = 0;
    if (CSF_AT(uint32_t, self, 0x30) == 0)
        return result;

    uint32_t childIndex = 0;
    uint32_t childOffset = 0;
    do
    {
        if (result)
            break;

        auto* entries =
            reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x28));
        void* child = CSF_AT(void*, entries, childOffset);

        result = static_cast<char>(
            ((int(__fastcall*)(void*, void*, int, int, float))
                 CSF_AT(void**, child, 0)[0x10 / 4])(
                child, nullptr, x, y, duration));

        ++childIndex;
        childOffset += 16;
    } while (childIndex < CSF_AT(uint32_t, self, 0x30));

    return result;
}

HOOK(0x00706740, C_008632c0_DispatchPointQuery, "auto");
