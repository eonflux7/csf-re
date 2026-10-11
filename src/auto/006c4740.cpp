#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, sub_6C7B20, 0x006C7B20, (int a1));
CSF_FUNC(int, __stdcall, Hud_SetWidgetParam, 0x006C7B40, (int a1, int a2));
CSF_FUNC(int, __stdcall, sub_6C7B60, 0x006C7B60, (int a1, int a2));
CSF_FUNC(int*, __fastcall, sub_6C7BF0, 0x006C7BF0, (void* self, void* edx, int* a2, int a3));

static char __fastcall GfxSkin_ResolveReferences(void* self, void* /*edx*/)
{
    const char resolved = CSF_AT(char, self, 0x10);
    if (resolved)
        return resolved;

    if (CSF_AT(void*, self, 0x08))
    {
        void* globalState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
        void* references = CSF_AT(void*, self, 0x08);
        void* resolver = CSF_AT(void*, globalState, 0x04);
        std::uint32_t remaining = CSF_AT(std::uint32_t, references, 0x08);

        if (remaining)
        {
            alignas(8) std::uint8_t resultBytes[0x10];
            std::uint32_t offset = 0;

            do
            {
                auto* entry = static_cast<std::uint8_t*>(
                    CSF_AT(void*, references, 0x00)) + offset;
                const int identifier = CSF_AT(int, entry, 0x00);

                if (identifier >= 0)
                {
                    int* result = sub_6C7BF0(
                        resolver, nullptr,
                        reinterpret_cast<int*>(resultBytes), identifier);
                    CSF_AT(int, entry, 0x08) = *result;
                    sub_6C7B20(reinterpret_cast<int>(entry));
                }
                else
                {
                    sub_6C7B60(
                        identifier & 0x7FFFFFFF,
                        reinterpret_cast<int>(entry + 0x08));

                    const int parameter = CSF_AT(int, entry, 0x04);
                    const int currentIdentifier = CSF_AT(int, entry, 0x00);
                    Hud_SetWidgetParam(currentIdentifier & 0x7FFFFFFF, parameter);
                }

                offset += 12;
                --remaining;
            } while (remaining);
        }
    }

    CSF_AT(char, self, 0x10) = 1;
    return 0;
}

HOOK(0x006C4740, GfxSkin_ResolveReferences, "auto");
