#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, DevConsole_DrawText, 0x004F9CC0,
         (void* self, void* edx, float a2, float a3, void* String, int* a5));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560, (std::uint16_t* text));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_LayoutInset, 0x0082FEBC);
CSF_GLOBAL(float, g_TextCharacterWidth, 0x0083F248);
CSF_GLOBAL(float, g_MenuWidthScale, 0x0083F24C);
CSF_GLOBAL(float, g_MenuHeightScale, 0x0083F250);

static int __fastcall S_004f21d0_DrawEntryAndAdvance(
    S_004f21d0* self, void* /*edx*/, float x, float* cursor,
    S_004f21d0* entry, int /*unused*/)
{
    auto* text = entry->field_0020 < 8u
        ? reinterpret_cast<std::uint16_t*>(
              reinterpret_cast<std::uint8_t*>(entry) + 0x0C)
        : CSF_AT(std::uint16_t*, entry, 0x0C);

    if (text &&
        *(entry->field_0020 < 8u
              ? reinterpret_cast<std::uint16_t*>(
                    reinterpret_cast<std::uint8_t*>(entry) + 0x0C)
              : CSF_AT(std::uint16_t*, entry, 0x0C)) != 0)
    {
        alignas(8) std::uint8_t colorBytes[0x1C];
        auto* colorComponents =
            reinterpret_cast<const std::int8_t*>(&entry->field_003c);
        CSF_AT(float, colorBytes, 0x00) = static_cast<float>(colorComponents[0]);
        CSF_AT(float, colorBytes, 0x04) = static_cast<float>(colorComponents[1]);
        CSF_AT(float, colorBytes, 0x08) = static_cast<float>(colorComponents[2]);
        CSF_AT(float, colorBytes, 0x0C) = 255.0f;

        void* widthObject = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* characterObject = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);

        text = entry->field_0020 < 8u
            ? reinterpret_cast<std::uint16_t*>(
                  reinterpret_cast<std::uint8_t*>(entry) + 0x0C)
            : CSF_AT(std::uint16_t*, entry, 0x0C);

        void* rightLayout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        void* leftLayout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

        void* widthDimensions =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, widthObject, 0)[0x18 / 4])(
                    widthObject, nullptr);
        void* characterDimensions =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, characterObject, 0)[0x18 / 4])(
                    characterObject, nullptr);

        int textLength = GetWideTextLength(text);
        std::int32_t availableWidth = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, rightLayout, 0x223C) -
            CSF_AT(std::uint32_t, leftLayout, 0x2234));
        int scaledWidth = static_cast<int>(
            availableWidth *
            (static_cast<float>(CSF_AT(std::int32_t, widthDimensions, 0)) *
             g_MenuWidthScale));
        int characterWidth = static_cast<int>(
            static_cast<float>(CSF_AT(std::int32_t, characterDimensions, 0)) *
            g_MenuWidthScale * g_TextCharacterWidth);
        int rows = 1 - textLength / (1 - scaledWidth / characterWidth);

        void* heightObject = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* heightDimensions =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, heightObject, 0)[0x18 / 4])(
                    heightObject, nullptr);
        int lineStep = static_cast<int>(
            static_cast<float>(CSF_AT(std::int32_t, heightDimensions, 4)) *
            g_MenuHeightScale * g_LayoutInset);

        std::uint32_t cursorBits;
        std::memcpy(&cursorBits, cursor, sizeof(cursorBits));
        cursorBits += static_cast<std::uint32_t>(lineStep) *
                      static_cast<std::uint32_t>(rows);
        std::memcpy(cursor, &cursorBits, sizeof(cursorBits));

        void* drawText = entry->field_0020 < 8u
            ? reinterpret_cast<std::uint8_t*>(entry) + 0x0C
            : CSF_AT(void*, entry, 0x0C);
        float drawY;
        std::memcpy(&drawY, &cursorBits, sizeof(drawY));
        DevConsole_DrawText(self, nullptr, x, drawY, drawText,
                            reinterpret_cast<int*>(colorBytes));
        return rows;
    }

    void* heightObject = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* heightDimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, heightObject, 0)[0x18 / 4])(
                heightObject, nullptr);
    int lineStep = static_cast<int>(
        static_cast<float>(CSF_AT(std::int32_t, heightDimensions, 4)) *
        g_MenuHeightScale * g_LayoutInset);

    std::uint32_t cursorBits;
    std::memcpy(&cursorBits, cursor, sizeof(cursorBits));
    cursorBits += static_cast<std::uint32_t>(lineStep);
    std::memcpy(cursor, &cursorBits, sizeof(cursorBits));
    return 1;
}

HOOK(0x004F9ED0, S_004f21d0_DrawEntryAndAdvance, "auto");
