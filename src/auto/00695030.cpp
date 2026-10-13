#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, sub_694CD0, 0x00694CD0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_694D50, 0x00694D50, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, sub_694DC0, 0x00694DC0, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, sub_694EA0, 0x00694EA0, (void* self, void* edx, char* a2));

CSF_GLOBAL(char, g_FormatIndentUnit, 0x008BAF88);

static int __fastcall S_00694cd0_PushFormatScope(
    S_00694cd0* self, void* /*edx*/, int scopeId,
    const char* text, char inlineMode, char startLine, int indentCount)
{
    // The packet's vtable annotation identifies this direct call target.
    // It is not an object vtable and must not be installed in the object.
    (void)CSF_VTBL(0x00694CD0);

    if (self->field_0004 < 0xFF)
    {
        self->field_0004 += 1;
        CSF_AT(int, self, 0x118 + self->field_0004 * 8) = scopeId;

        const uint32_t depth = self->field_0004;
        CSF_AT(uint8_t, self, 0x11C + depth * 8) =
            CSF_AT(uint8_t, self, 0x114 + depth * 8) != 0 ||
            inlineMode != 0;
    }

    if (CSF_AT(uint8_t, self, 0x11C + self->field_0004 * 8))
    {
        if (startLine)
        {
            sub_694DC0(self, nullptr, reinterpret_cast<char*>(self) + 0x14);

            uint32_t remaining = static_cast<uint32_t>(indentCount);
            if (remaining)
            {
                do
                {
                    const uint32_t length =
                        static_cast<uint32_t>(std::strlen(&g_FormatIndentUnit));
                    sub_694CD0(self, nullptr, static_cast<int>(length));
                    std::memcpy(
                        reinterpret_cast<void*>(self->field_0000 + self->field_000c),
                        &g_FormatIndentUnit, length);
                    self->field_000c += length;
                    --remaining;
                } while (remaining);
            }
        }
        else
        {
            sub_694DC0(self, nullptr,
                const_cast<char*>(CSF_STR(0x0083F284, " ")));
            sub_694D50(self, nullptr, static_cast<int>(self->field_0004 - 1));
        }
    }
    else
    {
        sub_694DC0(self, nullptr, reinterpret_cast<char*>(self) + 0x14);
    }

    int result;
    if (CSF_AT(uint8_t, self, 0x11C + self->field_0004 * 8))
    {
        const uint32_t length = static_cast<uint32_t>(std::strlen(text));
        sub_694CD0(self, nullptr, static_cast<int>(length));
        std::memcpy(
            reinterpret_cast<void*>(self->field_0000 + self->field_000c),
            text, length);
        self->field_000c += length;
        result = static_cast<int>(length);
    }
    else
    {
        result = sub_694EA0(self, nullptr, const_cast<char*>(text));
    }

    const uint32_t paddingLength = self->field_0010 + self->field_0004;
    if (paddingLength)
    {
        result = 538976288;
        std::memset(reinterpret_cast<uint8_t*>(self) + 0x14, 0x20, paddingLength);
    }
    CSF_AT(uint8_t, self, 0x14 + paddingLength) = 0;
    return result;
}

HOOK(0x00695030, S_00694cd0_PushFormatScope, "auto");
