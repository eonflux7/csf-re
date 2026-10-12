#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0083f1f8_SubmitInput, 0x004F9C20, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_ConsoleLineDivisor, 0x0083F248);
CSF_GLOBAL(float, g_MenuWidthScale, 0x0083F24C);

static void __fastcall C_0083f1f8_HandleKeyEvent(
    C_0083f1f8* self, void* /*edx*/, int key, int /*unused*/)
{
    switch (key)
    {
    case 14:
        if (self->field_0010 &&
            static_cast<int32_t>(self->field_00dc) > 0)
        {
            void* firstObject =
                LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            void* secondObject =
                LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);

            void* firstLayout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            void* secondLayout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

            int32_t* firstMetric =
                reinterpret_cast<int32_t* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, firstObject, 0)[0x18 / 4])(
                        firstObject, nullptr);
            int32_t* secondMetric =
                reinterpret_cast<int32_t* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, secondObject, 0)[0x18 / 4])(
                        secondObject, nullptr);

            const uint32_t layoutEnd = CSF_AT(uint32_t, firstLayout, 0x223C);
            const int32_t characterCount =
                static_cast<int32_t>(self->field_00dc);
            const int32_t availableWidth = static_cast<int32_t>(
                layoutEnd - CSF_AT(uint32_t, secondLayout, 0x2234));

            const int32_t scaledWidth = static_cast<int32_t>(
                static_cast<int64_t>(
                    static_cast<double>(availableWidth) *
                    (static_cast<double>(*firstMetric) * g_MenuWidthScale)));
            const int32_t lineWidth = static_cast<int32_t>(
                static_cast<int64_t>(
                    static_cast<double>(*secondMetric) *
                    g_MenuWidthScale * g_ConsoleLineDivisor));

            if (characterCount > scaledWidth / lineWidth)
            {
                const int32_t lineCount =
                    static_cast<int32_t>(self->field_00e8);
                if (lineCount > 0)
                    self->field_00e8 = lineCount - 1;
            }

            const uint32_t newCount =
                static_cast<uint32_t>(characterCount) - 1;
            self->field_00dc = newCount;
            CSF_AT(uint16_t, self, 0x12 + newCount * 2) = 0;
        }
        break;

    case 28:
        if (self->field_0010)
            C_0083f1f8_SubmitInput(self, nullptr);
        break;

    case 200:
        if (self->field_0010)
        {
            const int32_t position = static_cast<int32_t>(self->field_00e4);
            if (position > 0)
                self->field_00e4 = position - 1;
        }
        break;

    case 208:
        if (self->field_0010)
        {
            const int32_t position = static_cast<int32_t>(self->field_00e4);
            if (position < 50)
                self->field_00e4 = static_cast<uint32_t>(position) + 1;
        }
        break;

    default:
        return;
    }
}

HOOK(0x004FA0A0, C_0083f1f8_HandleKeyEvent, "auto");
