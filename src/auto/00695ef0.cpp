#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static void* __cdecl CoreVariantValues_FormatTypesAndReleaseStrings(
    void* typeCodes,
    int /*a2*/,
    void* firstBuffer,
    int /*a4*/,
    int /*a5*/,
    int /*a6*/,
    int /*a7*/,
    unsigned int firstCapacity,
    int /*a9*/,
    void* secondBuffer,
    int /*a11*/,
    int /*a12*/,
    int /*a13*/,
    int /*a14*/,
    unsigned int secondCapacity)
{
    const uint32_t begin = CSF_AT(uint32_t, typeCodes, 0x04);
    uint32_t count = 0;
    if (begin != 0) {
        const uint32_t byteCount = CSF_AT(uint32_t, typeCodes, 0x08) - begin;
        count = static_cast<uint32_t>(static_cast<int32_t>(byteCount) >> 2);
    }

    alignas(8) uint8_t textBytes[0x1C];
    C_0082d410& text = *reinterpret_cast<C_0082d410*>(textBytes);
    CSF_AT(uint32_t, textBytes, 0x18) = 15;
    CSF_AT(uint32_t, textBytes, 0x14) = 0;
    reinterpret_cast<uint8_t*>(&text.field_0004)[0] = 0;

    if (count != 0) {
        uint32_t index = 0;
        do {
            const auto entries = reinterpret_cast<const uint32_t*>(
                CSF_AT(uint32_t, typeCodes, 0x04));
            switch (entries[index]) {
            case 100: // 'd'
                Profiles_AppendStringRange(
                    &text, 0, const_cast<char*>(CSF_STR(0x0085E6A8, "Integer ")), 8);
                break;
            case 102: // 'f'
                Profiles_AppendStringRange(
                    &text, 0, const_cast<char*>(CSF_STR(0x0085E6A0, "Float ")), 6);
                break;
            case 115: // 's'
                Profiles_AppendStringRange(
                    &text, 0, const_cast<char*>(CSF_STR(0x0085E698, "String ")), 7);
                break;
            case 119: // 'w'
                Profiles_AppendStringRange(
                    &text, 0, const_cast<char*>(CSF_STR(0x0085E68C, "Wstring ")), 8);
                break;
            default:
                break;
            }
            ++index;
        } while (index < count);

        if (CSF_AT(uint32_t, textBytes, 0x18) >= 0x10 &&
            text.field_0004 != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(text.field_0004));
        }
    }

    CSF_AT(uint32_t, textBytes, 0x18) = 15;
    CSF_AT(uint32_t, textBytes, 0x14) = 0;
    reinterpret_cast<uint8_t*>(&text.field_0004)[0] = 0;

    if (firstCapacity >= 8 && firstBuffer != nullptr)
        Mem_FreeDebug(firstBuffer);

    if (secondCapacity < 8)
        return reinterpret_cast<void*>(static_cast<uintptr_t>(secondCapacity));

    if (secondBuffer != nullptr) {
        return reinterpret_cast<void*>(
            static_cast<uintptr_t>(Mem_FreeDebug(secondBuffer)));
    }
    return secondBuffer;
}

HOOK(0x00695EF0, CoreVariantValues_FormatTypesAndReleaseStrings, "auto");
