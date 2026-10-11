#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_GetField016c, 0x0048E230, (C_00833908* self, void*));

CSF_GLOBAL(float, g_RelatedObjectScaleY, 0x0082DC7C);
CSF_GLOBAL(float, g_WheelRecordScale, 0x0082DCE8);

static char __fastcall C_0083e8c8_UpdateRelatedObject(C_0083e8c8* self, void* /*edx*/)
{
    const char pending = CSF_AT(char, self, 0x30);
    if (!pending)
        return pending;

    alignas(8) uint8_t values[0xC];

    void* owner = reinterpret_cast<void*>(self->field_000c);
    void* entry =
        ((void*(__fastcall*)(void*, void*))
         CSF_AT(void**, owner, 0)[0xA0 / 4])(owner, nullptr);
    void* related =
        ((void*(__fastcall*)(void*, void*))
         CSF_AT(void**, entry, 0)[0x70 / 4])(entry, nullptr);

    if (related)
    {
        void* source =
            ((void*(__fastcall*)(void*, void*))
             CSF_AT(void**, related, 0)[0x30 / 4])(related, nullptr);

        CSF_AT(uint32_t, values, 0) = CSF_AT(uint32_t, source, 0);
        CSF_AT(float, values, 4) = CSF_AT(float, source, 4);
        CSF_AT(float, values, 8) = CSF_AT(float, source, 8);
        CSF_AT(float, values, 4) *= g_RelatedObjectScaleY;
        CSF_AT(float, values, 8) *= g_WheelRecordScale;

        ((int(__fastcall*)(void*, void*, void*))
         CSF_AT(void**, related, 0)[0x2C / 4])(related, nullptr, values);
    }

    entry = reinterpret_cast<void*>(
        C_00833908_GetField016c(
            reinterpret_cast<C_00833908*>(self->field_000c), nullptr));
    related =
        ((void*(__fastcall*)(void*, void*))
         CSF_AT(void**, entry, 0)[0x70 / 4])(entry, nullptr);

    char result = 0;
    if (related)
    {
        void* source =
            ((void*(__fastcall*)(void*, void*))
             CSF_AT(void**, related, 0)[0x30 / 4])(related, nullptr);

        CSF_AT(uint32_t, values, 0) = CSF_AT(uint32_t, source, 0);
        CSF_AT(float, values, 4) = CSF_AT(float, source, 4);
        CSF_AT(float, values, 8) = CSF_AT(float, source, 8);
        CSF_AT(float, values, 4) *= g_RelatedObjectScaleY;
        CSF_AT(float, values, 8) *= g_WheelRecordScale;

        result = static_cast<char>(
            ((int(__fastcall*)(void*, void*, void*))
             CSF_AT(void**, related, 0)[0x2C / 4])(related, nullptr, values));
    }

    CSF_AT(char, self, 0x30) = 0;
    return result;
}

HOOK(0x004E1E40, C_0083e8c8_UpdateRelatedObject, "auto");
