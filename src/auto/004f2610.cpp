#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __cdecl, IsGlobal8AFC78NonZero, 0x0042D6E0, ());
CSF_FUNC(uint32_t*, __fastcall, CHud_CopyBoundsFromSource, 0x0042E150, (CHud* self, void* , int* a2));
CSF_FUNC(int, __fastcall, CHud_AttachPanelsToTarget, 0x0042E1A0, (CHud* self, void* , int a2));
CSF_FUNC(int, __fastcall, CHud_LookupValueByKey, 0x00431F10, (CHud* self, void* , int key));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

static void* __fastcall S_004f2610_UpdateTextAndLinkedPanel(
    S_004f2610* self, void* /*edx*/, int entryAddress, int stringAddress)
{
    auto* entry = reinterpret_cast<std::uint8_t*>(entryAddress);
    auto* input = reinterpret_cast<std::uint8_t*>(stringAddress);

    const bool plainText = self->field_004c == 0;
    const std::uint32_t inputCapacity = CSF_AT(std::uint32_t, input, 0x18);

    alignas(8) std::uint8_t textBytes[0x1C]{};
    alignas(8) std::uint8_t temporaryBytes[0x1C]{};
    CSF_AT(std::uint32_t, textBytes, 0x18) = 7;
    CSF_AT(std::uint32_t, textBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, textBytes, 0x04) = 0;

    wchar_t* inputText = inputCapacity < 8
        ? reinterpret_cast<wchar_t*>(input + 4)
        : CSF_AT(wchar_t*, input, 0x04);

    void* source;
    if (plainText)
    {
        source = ConstructWideString(temporaryBytes, nullptr, inputText);
    }
    else
    {
        const int formattingMode = g_NumberFormattingMode;
        alignas(8) std::uint8_t argumentBytes[0x1C]{};
        CSF_AT(std::uint32_t, argumentBytes, 0x18) = 7;
        CSF_AT(std::uint32_t, argumentBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, argumentBytes, 0x04) = 0;
        AssignStringRange_488830(
            argumentBytes, nullptr,
            reinterpret_cast<std::uint8_t*>(self) + 0x38, 0, 0xFFFFFFFF);

        // The formatter receives a 28-byte string by value, followed by
        // the text and formatting mode, beyond its declared output argument.
        struct StringArgument
        {
            std::uint32_t words[7];
        };
        StringArgument argument;
        std::memcpy(&argument, argumentBytes, sizeof(argument));
        source = reinterpret_cast<void* (__cdecl*)(void*, ...)>(
            FormatCountTextBuffer)(textBytes, argument, inputText, formattingMode);
    }

    AssignStringRange_488830(textBytes, nullptr, source, 0, 0xFFFFFFFF);
    if (plainText &&
        CSF_AT(std::uint32_t, temporaryBytes, 0x18) >= 8 &&
        CSF_AT(void*, temporaryBytes, 0x04))
    {
        Mem_FreeDebug(CSF_AT(void*, temporaryBytes, 0x04));
    }

    void* textObject = CSF_AT(void*, entry, 0x78);
    void* alignment = entry + (self->field_0059 ? 92 : 100);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, textObject, 0)[0x50 / 4])(
            textObject, nullptr, alignment);

    textObject = CSF_AT(void*, entry, 0x78);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, textObject, 0)[0x58 / 4])(
            textObject, nullptr, entry + 76);

    void* displayedText = CSF_AT(void*, textBytes, 0x04);
    if (CSF_AT(std::uint32_t, textBytes, 0x18) < 8)
        displayedText = textBytes + 4;
    textObject = CSF_AT(void*, entry, 0x78);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, textObject, 0)[0x40 / 4])(
            textObject, nullptr, displayedText);

    textObject = CSF_AT(void*, entry, 0x78);
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t)>(
        CSF_AT(void**, textObject, 0)[0x68 / 4])(
            textObject, nullptr, self->field_0024);

    textObject = CSF_AT(void*, entry, 0x78);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, textObject, 0)[0x98 / 4])(
            textObject, nullptr, reinterpret_cast<std::uint8_t*>(self) + 92);

    int* linkedPanel = nullptr;
    if (static_cast<std::uint8_t>(IsGlobal8AFC78NonZero()))
    {
        const int key = CSF_AT(int, entry, 0x38);
        auto* hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
        linkedPanel = reinterpret_cast<int*>(
            CHud_LookupValueByKey(hud, nullptr, key));
        if (linkedPanel)
        {
            hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
            CHud_CopyBoundsFromSource(hud, nullptr, linkedPanel);
        }
    }

    // The last two color components share storage with the position output.
    alignas(8) std::uint8_t colorAndPositionBytes[0x10]{};
    if (CSF_AT(std::uint8_t, self, 0x58))
    {
        auto* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        const float fadeStart =
            CSF_AT(float, settings, 0x18F0) * CSF_AT(float, entry, 0x40);

        textObject = CSF_AT(void*, entry, 0x78);
        void* color = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, textObject, 0)[0x64 / 4])(textObject, nullptr);
        const float elapsed = CSF_AT(float, entry, 0x3C);
        std::memcpy(colorAndPositionBytes, color, 0x10);

        const float alpha = elapsed < fadeStart
            ? g_ColorChannelScale
            : g_ColorChannelScale -
                (CSF_AT(float, entry, 0x3C) - fadeStart) *
                g_ColorChannelScale /
                (CSF_AT(float, entry, 0x40) - fadeStart);
        CSF_AT(float, colorAndPositionBytes, 0x0C) = alpha;

        textObject = CSF_AT(void*, entry, 0x78);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, textObject, 0)[0x60 / 4])(
                textObject, nullptr, colorAndPositionBytes);

        if (linkedPanel)
        {
            alignas(8) std::uint8_t linkedColorBytes[0x10]{};
            textObject = CSF_AT(void*, entry, 0x78);
            color = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, textObject, 0)[0x64 / 4])(textObject, nullptr);
            std::memcpy(linkedColorBytes, color, 0x10);
            CSF_AT(float, linkedColorBytes, 0x0C) = alpha;
            reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, linkedPanel, 0)[0x68 / 4])(
                    linkedPanel, nullptr, linkedColorBytes);
        }
    }

    textObject = CSF_AT(void*, entry, 0x78);
    void* result = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, textObject, 0)[0x08 / 4])(textObject, nullptr);

    if (linkedPanel)
    {
        alignas(8) std::uint8_t matrixReferenceBytes[0x0C]{};
        auto*& matrix = CSF_AT(std::uint32_t*, matrixReferenceBytes, 0);
        matrix = RwMatrixCreate();
        CSF_AT(std::uint8_t, matrixReferenceBytes, 0x04) = 1;
        CSF_AT(float, matrix, 0x28) = 1.0f;
        CSF_AT(float, matrix, 0x14) = 1.0f;
        CSF_AT(float, matrix, 0x00) = 1.0f;
        matrix[4] = 0;
        matrix[2] = 0;
        matrix[1] = 0;
        matrix[9] = 0;
        matrix[8] = 0;
        matrix[6] = 0;
        matrix[14] = 0;
        matrix[13] = 0;
        matrix[12] = 0;
        matrix[3] |= 0x20003u;

        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, linkedPanel, 0)[0x60 / 4])(
                linkedPanel, nullptr, matrixReferenceBytes);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, linkedPanel, 0)[0x38 / 4])(
                linkedPanel, nullptr, reinterpret_cast<std::uint8_t*>(self) + 0x14);

        void* position = colorAndPositionBytes + 8;
        textObject = CSF_AT(void*, entry, 0x78);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, textObject, 0)[0x54 / 4])(
                textObject, nullptr, position);
        CSF_AT(std::uint32_t, position, 0) += CSF_AT(std::uint32_t, self, 0x1C);

        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, linkedPanel, 0)[0x3C / 4])(linkedPanel, nullptr);

        textObject = CSF_AT(void*, entry, 0x78);
        void* measuredObject = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, textObject, 0)[0x28 / 4])(textObject, nullptr);
        if (measuredObject)
        {
            const int extent = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, measuredObject, 0)[0x18 / 4])(
                    measuredObject, nullptr, 1);
            const std::int64_t halfExtent = static_cast<std::int64_t>(
                static_cast<double>(extent) * g_HalfDurationScale);
            CSF_AT(std::uint32_t, position, 0x04) +=
                static_cast<std::uint32_t>(halfExtent);
        }

        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, linkedPanel, 0)[0x28 / 4])(
                linkedPanel, nullptr, position);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, linkedPanel, 0)[0x44 / 4])(linkedPanel, nullptr, 4);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, linkedPanel, 0)[0x08 / 4])(linkedPanel, nullptr);

        auto* hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
        result = reinterpret_cast<void*>(
            CHud_AttachPanelsToTarget(hud, nullptr, reinterpret_cast<int>(linkedPanel)));
        if (CSF_AT(std::uint8_t, matrixReferenceBytes, 0x04))
            result = reinterpret_cast<void*>(RwMatrixDestroy(matrix));
    }

    if (CSF_AT(std::uint32_t, textBytes, 0x18) >= 8)
    {
        result = CSF_AT(void*, textBytes, 0x04);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x004F2610, S_004f2610_UpdateTextAndLinkedPanel, "auto");
