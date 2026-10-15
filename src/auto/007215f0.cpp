#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, ReserveSsoOrHeapCapacity, 0x004856E0, (void* self, void* edx, uint32_t count, uint32_t flag));
CSF_FUNC(int, __cdecl, StringFormat_GetWideTextLength, 0x00695560, (uint16_t* text));
CSF_FUNC(void*, __fastcall, InsertWideCharacters, 0x006959F0, (void* self, void* edx, unsigned int a2, unsigned int a3, int16_t a4));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (S_004f21d0* self, void* , int16_t a2));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* , uint8_t* a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall CGuiEdit_AdjustTextRangeLeft(
    CGuiEdit* self, void* /*edx*/, char forceLeft)
{
    alignas(8) uint8_t boundsBytes[0x8];
    alignas(8) uint8_t extentBytes[0x8];
    alignas(8) uint8_t rangeBytes[0xC];
    uint32_t& sourceLength = CSF_AT(uint32_t, rangeBytes, 4);
    uint32_t& workingIndex = CSF_AT(uint32_t, rangeBytes, 8);

    auto* visibleText = reinterpret_cast<S_004f21d0*>(&self->m_wtext1);
    TextInputSubobject_Init(visibleText, nullptr, &g_byte_82E65C);

    uint16_t* data = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
        ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
        : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
    void* textObject = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void (__fastcall*)(void*, void*, uint16_t*)>(
        CSF_AT(void**, textObject, 0)[0x40 / 4])(textObject, nullptr, data);

    workingIndex = self->field_01b0;
    while (static_cast<int32_t>(workingIndex) >= 0)
    {
        textObject = reinterpret_cast<void*>(self->m_pText);
        int32_t* extent =
            reinterpret_cast<int32_t* (__fastcall*)(void*, void*, void*, uint32_t)>(
                CSF_AT(void**, textObject, 0)[0x78 / 4])(
                    textObject, nullptr, rangeBytes, 0);

        textObject = reinterpret_cast<void*>(self->m_pText);
        int32_t* bounds =
            reinterpret_cast<int32_t* (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, textObject, 0)[0x5C / 4])(
                    textObject, nullptr, extentBytes);
        if (*bounds < *extent)
            break;

        uint16_t* source = CSF_AT(uint32_t, &self->m_wtext0, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext0) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext0, 4));
        uint16_t* cursor = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
        int16_t character = static_cast<int16_t>(source[workingIndex]);
        uint16_t* begin = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
        uint32_t position = cursor
            ? static_cast<uint32_t>(static_cast<int32_t>(
                reinterpret_cast<uint32_t>(cursor) -
                reinterpret_cast<uint32_t>(begin)) >> 1)
            : 0u;
        InsertWideCharacters(visibleText, nullptr, position, 1u, character);

        data = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
        textObject = reinterpret_cast<void*>(self->m_pText);
        reinterpret_cast<void (__fastcall*)(void*, void*, uint16_t*)>(
            CSF_AT(void**, textObject, 0)[0x40 / 4])(textObject, nullptr, data);
        --workingIndex;
    }

    textObject = reinterpret_cast<void*>(self->m_pText);
    int32_t* extent =
        reinterpret_cast<int32_t* (__fastcall*)(void*, void*, void*, uint32_t)>(
            CSF_AT(void**, textObject, 0)[0x78 / 4])(
                textObject, nullptr, extentBytes, 0);
    textObject = reinterpret_cast<void*>(self->m_pText);
    int32_t* bounds =
        reinterpret_cast<int32_t* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, textObject, 0)[0x5C / 4])(
                textObject, nullptr, rangeBytes);
    if (*bounds < *extent)
    {
        uint16_t* cursor = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
        uint16_t* begin = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
        uint32_t position = cursor
            ? static_cast<uint32_t>(static_cast<int32_t>(
                reinterpret_cast<uint32_t>(cursor) -
                reinterpret_cast<uint32_t>(begin)) >> 1)
            : 0u;
        ReserveSsoOrHeapCapacity(visibleText, nullptr, position, 1u);
    }

    data = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
        ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
        : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
    textObject = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void (__fastcall*)(void*, void*, uint16_t*)>(
        CSF_AT(void**, textObject, 0)[0x40 / 4])(textObject, nullptr, data);

    uint32_t rightIndex = self->field_01b0 + 1;
    sourceLength = CSF_AT(uint32_t, &self->m_wtext0, 0x14);
    while (rightIndex < sourceLength)
    {
        textObject = reinterpret_cast<void*>(self->m_pText);
        extent =
            reinterpret_cast<int32_t* (__fastcall*)(void*, void*, void*, uint32_t)>(
                CSF_AT(void**, textObject, 0)[0x78 / 4])(
                    textObject, nullptr, extentBytes, 0);
        workingIndex = reinterpret_cast<uint32_t>(extent);
        textObject = reinterpret_cast<void*>(self->m_pText);
        bounds =
            reinterpret_cast<int32_t* (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, textObject, 0)[0x5C / 4])(
                    textObject, nullptr, boundsBytes);
        if (*bounds < *reinterpret_cast<int32_t*>(workingIndex))
            break;

        uint16_t* source = CSF_AT(uint32_t, &self->m_wtext0, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext0) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext0, 4));
        SaveSlots_ApplyIndex(
            visibleText, nullptr, static_cast<int16_t>(source[rightIndex]));

        data = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
        textObject = reinterpret_cast<void*>(self->m_pText);
        reinterpret_cast<void (__fastcall*)(void*, void*, uint16_t*)>(
            CSF_AT(void**, textObject, 0)[0x40 / 4])(textObject, nullptr, data);

        uint32_t nextEnd = self->field_01b0 + 1;
        uint32_t limit = sourceLength;
        ++rightIndex;
        self->field_01b0 = nextEnd;
        if (rightIndex >= limit)
            break;
    }

    textObject = reinterpret_cast<void*>(self->m_pText);
    extent =
        reinterpret_cast<int32_t* (__fastcall*)(void*, void*, void*, uint32_t)>(
            CSF_AT(void**, textObject, 0)[0x78 / 4])(
                textObject, nullptr, boundsBytes, 0);
    textObject = reinterpret_cast<void*>(self->m_pText);
    bounds =
        reinterpret_cast<int32_t* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, textObject, 0)[0x5C / 4])(
                textObject, nullptr, extentBytes);
    if (*bounds < *extent)
    {
        data = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
        uint32_t cursor = reinterpret_cast<uint32_t>(data) +
            2 * CSF_AT(uint32_t, &self->m_wtext1, 0x14) - 2;
        uint16_t* begin = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
        uint32_t position = cursor
            ? static_cast<uint32_t>(static_cast<int32_t>(
                cursor - reinterpret_cast<uint32_t>(begin)) >> 1)
            : 0u;
        ReserveSsoOrHeapCapacity(visibleText, nullptr, position, 1u);
        --self->field_01b0;
    }

    data = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
        ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
        : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
    textObject = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void (__fastcall*)(void*, void*, uint16_t*)>(
        CSF_AT(void**, textObject, 0)[0x40 / 4])(textObject, nullptr, data);

    data = CSF_AT(uint32_t, &self->m_wtext1, 0x18) < 8u
        ? reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&self->m_wtext1) + 4)
        : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, &self->m_wtext1, 4));
    int displayedLength = StringFormat_GetWideTextLength(data);
    uint32_t firstIndex = self->field_01b0 -
        static_cast<uint32_t>(displayedLength) + 1;
    if (static_cast<int32_t>(firstIndex) <= 0)
        firstIndex = 0;
    else
        firstIndex = self->field_01b0 -
            static_cast<uint32_t>(displayedLength) + 1;

    bool caretBeforeRange = self->m_caretIndex < firstIndex;
    self->field_01ac = firstIndex;
    uint32_t leftMode = (caretBeforeRange || forceLeft)
        ? 1u : (firstIndex & 0xFFFFFF00u);

    textObject = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, textObject, 0)[0x8C / 4])(
            textObject, nullptr, leftMode);

    textObject = reinterpret_cast<void*>(self->m_pText);
    bool isLeftMode = reinterpret_cast<bool (__fastcall*)(void*, void*)>(
        CSF_AT(void**, textObject, 0)[0x88 / 4])(textObject, nullptr);
    uint32_t caretOffset = isLeftMode
        ? 0u : self->m_caretIndex - self->field_01ac;

    textObject = reinterpret_cast<void*>(self->m_pText);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, textObject, 0)[0x84 / 4])(
            textObject, nullptr, static_cast<int32_t>(caretOffset));
}

HOOK(0x007215F0, CGuiEdit_AdjustTextRangeLeft, "auto");
