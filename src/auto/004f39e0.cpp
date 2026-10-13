#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, CHud_LookupValueByKey, 0x00431F10, (CHud* self, void* , int key));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, HudStringShow_InitializeEntry, 0x004F2570, (void* a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, S_004f36f0_StoreEntryPointerForItem, 0x004F36F0, (S_004f36f0* self, void* , uint32_t* output, C_0082d410* item, int context));
CSF_FUNC(uint32_t*, __fastcall, S_004f36f0_EmplaceBack, 0x004F3760, (S_004f36f0* self, void* , int value));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int text, int flag));
CSF_FUNC(int, __cdecl, MenuMessageBox_TestEntryAgainstFlag, 0x00695500, (uint16_t* entry, char* flagByte));
CSF_FUNC(int, __fastcall, MenuAccount_IsGateClear_696B60, 0x00696B60, (S_00696af0* self, void* , char* a2));
CSF_FUNC(std::uint32_t*, __fastcall, S_004f21d0_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(float, g_HudHorizontalInset, 0x0083F10C);

static void __fastcall HudStringShow_ShowWithStyle(
    void* self, void* /*edx*/, void* source, char* text,
    float duration, int styleKey)
{
    // The packet labels this call address as a vtable reference.
    (void)CSF_VTBL(0x004F36F0);

    if (styleKey != -1)
    {
        CHud_LookupValueByKey(
            reinterpret_cast<CHud*>(GetGlobal8afc78()), nullptr, styleKey);
    }

    if (!(duration > g_fZero))
        return;

    void* screen = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* screenSize =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, screen, 0)[0x18 / 4])(screen, nullptr);
    const int x = static_cast<int32_t>(static_cast<int64_t>(
        static_cast<float>(CSF_AT(int32_t, screenSize, 0)) *
        CSF_AT(float, self, 0x28)));

    screen = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    screenSize =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, screen, 0)[0x18 / 4])(screen, nullptr);
    const int y = static_cast<int32_t>(static_cast<int64_t>(
        static_cast<float>(CSF_AT(int32_t, screenSize, 4)) *
        CSF_AT(float, self, 0x2C)));

    auto* entries = reinterpret_cast<S_004f36f0*>(
        static_cast<uint8_t*>(self) + 4);
    const uint32_t begin = entries->field_0004;
    uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(entries->field_0008 - begin) / 124)
        : 0;

    uint32_t index = 0;
    if (count)
    {
        auto* coordinates = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 8)) + 108;
        while (index < count)
        {
            if (CSF_AT(int32_t, coordinates, 0) == x &&
                CSF_AT(int32_t, coordinates, 4) == y)
                break;
            ++index;
            coordinates += 124;
        }
    }

    // These three adjacent regions form the complete 124-byte entry.
    alignas(8) uint8_t entryBytes[0x7C];
    C_0082d410& entry = *reinterpret_cast<C_0082d410*>(entryBytes);
    void* entryDisplayString = entryBytes + 0x1C;
    void* bounds = entryBytes + 0x4C;

    HudStringShow_InitializeEntry(&entry, static_cast<int>(count));
    CSF_AT(int32_t, entryBytes, 0x38) = styleKey;
    AssignStringRange_488830(
        entryDisplayString, nullptr, source, 0, 0xFFFFFFFF);
    AssignStringRange_429B60(
        &entry, nullptr, text, static_cast<unsigned int>(std::strlen(text)));

    CSF_AT(float, entryBytes, 0x40) = duration;
    CSF_AT(int32_t, entryBytes, 0x3C) = 0;
    CSF_AT(int32_t, entryBytes, 0x44) = 0;
    CSF_AT(int32_t, entryBytes, 0x48) = 0;
    CSF_AT(int32_t, entryBytes, 0x54) = x;
    CSF_AT(int32_t, entryBytes, 0x58) = y;
    CSF_AT(int32_t, entryBytes, 0x5C) = x;
    CSF_AT(int32_t, entryBytes, 0x60) = y;
    CSF_AT(int32_t, entryBytes, 0x64) = x;
    CSF_AT(int32_t, entryBytes, 0x68) = y;
    CSF_AT(int32_t, entryBytes, 0x6C) = x;
    CSF_AT(int32_t, entryBytes, 0x70) = y;

    void* controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* label =
        reinterpret_cast<void*(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, controls, 0)[0x54 / 4])(
                controls, nullptr, CSF_AT(uint32_t, state, 0x8C));
    CSF_AT(void*, entryBytes, 0x78) = label;

    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, label, 0)[0x98 / 4])(
            label, nullptr, static_cast<uint8_t*>(self) + 0x5C);

    screen = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    screenSize =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, screen, 0)[0x18 / 4])(screen, nullptr);
    const float horizontalInset =
        static_cast<float>(CSF_AT(int32_t, screenSize, 0)) *
        g_HudHorizontalInset;

    screen = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    screenSize =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, screen, 0)[0x18 / 4])(screen, nullptr);
    CSF_AT(int32_t, bounds, 0) =
        CSF_AT(int32_t, screenSize, 0) -
        static_cast<int32_t>(static_cast<int64_t>(horizontalInset)) - x;

    screen = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    screenSize =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, screen, 0)[0x18 / 4])(screen, nullptr);
    CSF_AT(int32_t, bounds, 4) = CSF_AT(int32_t, screenSize, 4) - y;

    // Includes the leading string-base byte and the inline wide buffer.
    alignas(8) uint8_t displayBytes[0x2C];
    CSF_AT(uint32_t, displayBytes, 0x18) = 7;
    CSF_AT(uint32_t, displayBytes, 0x14) = 0;
    CSF_AT(uint16_t, displayBytes, 4) = 0;

    char* flag = reinterpret_cast<char*>(&g_byte_82E65C);
    if (static_cast<uint8_t>(MenuAccount_IsGateClear_696B60(
            reinterpret_cast<S_00696af0*>(source), nullptr, flag)))
    {
        auto* converted = reinterpret_cast<int16_t*>(
            Text_ConvertToDisplayEncoding(
                reinterpret_cast<int>(text), CSF_AT(int32_t, self, 0x20)));
        if (converted &&
            MenuMessageBox_TestEntryAgainstFlag(
                reinterpret_cast<uint16_t*>(converted), flag))
        {
            S_004f21d0_AssignText(
                reinterpret_cast<uint32_t*>(displayBytes), nullptr, converted);
        }
    }
    else
    {
        AssignStringRange_488830(
            displayBytes, nullptr, source, 0, 0xFFFFFFFF);
    }

    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, label, 0)[0x58 / 4])(label, nullptr, bounds);

    void* displayText = CSF_AT(uint32_t, displayBytes, 0x18) < 8
        ? static_cast<void*>(displayBytes + 4)
        : CSF_AT(void*, displayBytes, 4);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, label, 0)[0x40 / 4])(label, nullptr, displayText);

    CSF_AT(int32_t, bounds, 4) =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, label, 0)[0xA0 / 4])(label, nullptr);

    if (index < count)
    {
        uint32_t byteOffset = 124 * index;
        uint32_t storedItem;
        S_004f36f0_StoreEntryPointerForItem(
            entries, nullptr, &storedItem,
            reinterpret_cast<C_0082d410*>(
                CSF_AT(uint32_t, self, 8) + byteOffset),
            reinterpret_cast<int>(&entry));

        void* font =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, label, 0)[0x28 / 4])(label, nullptr);
        int fontMetric = 0;
        if (font)
        {
            fontMetric =
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, font, 0)[0x18 / 4])(font, nullptr, 1);
        }
        const float metric = fontMetric
            ? static_cast<float>(fontMetric) : 0.029999999f;
        const float height = static_cast<float>(CSF_AT(int32_t, bounds, 4));
        void* timing = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        const float shiftTime =
            std::fabs(height) * (CSF_AT(float, timing, 6380) / metric);

        const uint32_t currentBegin = entries->field_0004;
        count = currentBegin
            ? static_cast<uint32_t>(
                  static_cast<int32_t>(
                      entries->field_0008 - currentBegin) / 124)
            : 0;

        uint32_t accumulatedHeight = 0;
        for (uint32_t currentIndex = index; currentIndex < count; ++currentIndex)
        {
            auto* item = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, self, 8) + byteOffset);
            if (CSF_AT(float, item, 72) > 0.0f)
            {
                CSF_AT(uint32_t, item, 84) = CSF_AT(uint32_t, item, 92);
                CSF_AT(uint32_t, item, 88) = CSF_AT(uint32_t, item, 96);

                item = reinterpret_cast<uint8_t*>(
                    CSF_AT(uint32_t, self, 8) + byteOffset);
                void* itemLabel = CSF_AT(void*, item, 120);
                void* itemFont =
                    reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, itemLabel, 0)[0x28 / 4])(
                            itemLabel, nullptr);
                int itemMetric = 0;
                if (itemFont)
                {
                    itemMetric =
                        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, itemFont, 0)[0x18 / 4])(
                                itemFont, nullptr, 1);
                }
                const float itemScale = itemMetric
                    ? static_cast<float>(itemMetric) : 0.029999999f;
                const float remainingDistance = static_cast<float>(
                    static_cast<int32_t>(
                        CSF_AT(uint32_t, item, 104) -
                        CSF_AT(uint32_t, item, 88)));
                timing = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                CSF_AT(float, item, 72) =
                    std::fabs(remainingDistance) *
                    (CSF_AT(float, timing, 6380) / itemScale);

                CSF_AT(uint32_t,
                    reinterpret_cast<void*>(
                        CSF_AT(uint32_t, self, 8) + byteOffset), 68) = 0;
            }

            CSF_AT(int32_t,
                reinterpret_cast<void*>(
                    CSF_AT(uint32_t, self, 8) + byteOffset), 104) = y;

            CSF_AT(uint32_t,
                reinterpret_cast<void*>(
                    CSF_AT(uint32_t, self, 8) + byteOffset), 104) +=
                accumulatedHeight * CSF_AT(uint32_t, self, 52);

            item = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, self, 8) + byteOffset);
            CSF_AT(float, item, 72) = shiftTime + CSF_AT(float, item, 72);

            const int direction = CSF_AT(int32_t, self, 52);
            uint32_t precedingHeight = 0;
            if (direction == 1)
            {
                precedingHeight = CSF_AT(uint32_t, self, 48) +
                    CSF_AT(uint32_t,
                        reinterpret_cast<void*>(
                            CSF_AT(uint32_t, self, 8) + byteOffset), 80);
            }
            const uint32_t subtotal = precedingHeight + accumulatedHeight;

            uint32_t followingHeight = 0;
            if (direction == -1 && currentIndex + 1 < count)
            {
                followingHeight = CSF_AT(uint32_t, self, 48) +
                    CSF_AT(uint32_t,
                        reinterpret_cast<void*>(
                            CSF_AT(uint32_t, self, 8) + byteOffset), 204);
            }
            accumulatedHeight = followingHeight + subtotal;
            byteOffset += 124;
        }
    }
    else
    {
        S_004f36f0_EmplaceBack(
            entries, nullptr, reinterpret_cast<int>(&entry));
    }

    if (CSF_AT(uint32_t, displayBytes, 0x18) >= 8 &&
        CSF_AT(void*, displayBytes, 4))
        Mem_FreeDebug(CSF_AT(void*, displayBytes, 4));
    CSF_AT(uint32_t, displayBytes, 0x18) = 7;
    CSF_AT(uint32_t, displayBytes, 0x14) = 0;
    CSF_AT(uint16_t, displayBytes, 4) = 0;

    if (CSF_AT(uint32_t, entryDisplayString, 0x18) >= 8 &&
        CSF_AT(void*, entryDisplayString, 4))
        Mem_FreeDebug(CSF_AT(void*, entryDisplayString, 4));
    CSF_AT(uint32_t, entryDisplayString, 0x18) = 7;
    CSF_AT(uint32_t, entryDisplayString, 0x14) = 0;
    CSF_AT(uint16_t, entryDisplayString, 4) = 0;

    if (CSF_AT(uint32_t, entryBytes, 0x18) >= 0x10 && entry.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(entry.field_0004));
}

HOOK(0x004F39E0, HudStringShow_ShowWithStyle, "auto");
