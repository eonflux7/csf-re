#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ReserveSsoOrHeapCapacity, 0x004856E0, (void* self, void* edx, uint32_t count, uint32_t flag));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, ProfilesSavegames_FormatDateTime, 0x00514180, (const void* dateTime, void* outString));
CSF_FUNC(int, __cdecl, MenuMpStats_RenderPanelByIndex, 0x00540F90, (int panelIndex));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreParser_ResolveEntryByKey, 0x00692440, (unsigned int key, int flags));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(std::uint32_t*, __fastcall, S_004f21d0_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(void*, __fastcall, AppendNarrowToWideString, 0x00697150, (void* self, void* , uint8_t* a2));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660, (CGuiListBoxBase* self, void*));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_SetColumnWidth, 0x00716520, (CGuiListBoxBase* self, void* , std::uint32_t columnIndex, std::int32_t width));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (CGuiListBoxBase* self, void* , int index));

CSF_GLOBAL(float, g_SupportRangeScale, 0x0082E06C);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(float, g_MpStatsLastColumnScale, 0x008431A0);
CSF_GLOBAL(float, g_FilmBackwardRandomScale, 0x008431AC);

static void* MenuMpStats_GetStringText(void* string)
{
    if (CSF_AT(uint32_t, string, 0x18) < 8)
        return static_cast<uint8_t*>(string) + 4;
    return CSF_AT(void*, string, 4);
}

static void __cdecl MenuMpStats_PopulateList(void* records, CGuiListBoxBase* list)
{
    if (!list)
        return;

    alignas(8) uint8_t emptyStringBytes[0x1C];
    alignas(8) uint8_t statusStringBytes[0x1C];
    alignas(8) uint8_t dateStringBytes[0x1C];
    // The final string spans both the -0x1C and -0x18 stack regions.
    alignas(8) uint8_t descriptionStringBytes[0x1C];

    S_004f21d0_SetPattern(
        reinterpret_cast<S_004f21d0*>(emptyStringBytes), nullptr,
        &g_byte_82E65C);

    uint32_t rowCount = 0;
    uint32_t begin = CSF_AT(uint32_t, records, 4);
    if (begin)
    {
        rowCount = static_cast<uint32_t>(
            static_cast<int32_t>(CSF_AT(uint32_t, records, 8) - begin) / 200);
    }

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        list->vtbl[0x298 / 4])(list, nullptr);
    CGuiListBoxBase_ClearStorageAndSendEvent(list, nullptr);

    int32_t width = CSF_AT(int32_t, list->vGetSize(), 0);
    CGuiListBoxBase_SetColumnWidth(
        list, nullptr, 0,
        static_cast<int32_t>(static_cast<int64_t>(
            static_cast<float>(width) * g_SoundActivityThreshold)));

    width = CSF_AT(int32_t, list->vGetSize(), 0);
    CGuiListBoxBase_SetColumnWidth(
        list, nullptr, 1,
        static_cast<int32_t>(static_cast<int64_t>(
            static_cast<float>(width) * g_FilmBackwardRandomScale)));

    width = CSF_AT(int32_t, list->vGetSize(), 0);
    CGuiListBoxBase_SetColumnWidth(
        list, nullptr, 2,
        static_cast<int32_t>(static_cast<int64_t>(
            static_cast<float>(width) * g_SupportRangeScale)));

    width = CSF_AT(int32_t, list->vGetSize(), 0);
    CGuiListBoxBase_SetColumnWidth(
        list, nullptr, 3,
        static_cast<int32_t>(static_cast<int64_t>(
            static_cast<float>(width) * g_MpStatsLastColumnScale)));

    if (CSF_AT(int32_t, reinterpret_cast<void*>(GetGlobalValue_8B056C()), 4))
    {
        CSF_AT(uint32_t, descriptionStringBytes, 0x18) = 7;
        CSF_AT(uint32_t, descriptionStringBytes, 0x14) = 0;
        CSF_AT(uint16_t, descriptionStringBytes, 4) = 0;

        uint32_t recordOffset = 0;
        for (uint32_t row = 0; row < rowCount; ++row, recordOffset += 200)
        {
            uint8_t* record = CSF_AT(uint8_t*, records, 4) + recordOffset;
            if (CSF_AT(uint8_t, record, 192))
            {
                CSF_AT(uint32_t, statusStringBytes, 0x14) = 0;
                CSF_AT(uint16_t, statusStringBytes, 4) = 0;
                CSF_AT(uint32_t, statusStringBytes, 0x18) = 7;

                void* status = InitStringFromLiteral(
                    statusStringBytes, CSF_STR(0x008431A4, "C109"), 1);
                void* text = MenuMpStats_GetStringText(status);
                reinterpret_cast<void(__fastcall*)(
                    void*, void*, uint32_t, int, void*, uint32_t)>(
                    list->vtbl[0x294 / 4])(list, nullptr, row, 0, text, row);

                if (CSF_AT(uint32_t, statusStringBytes, 0x18) >= 8)
                {
                    void* block = CSF_AT(void*, statusStringBytes, 4);
                    if (block)
                        Mem_FreeDebug(block);
                }
                CSF_AT(uint32_t, statusStringBytes, 0x18) = 7;
                CSF_AT(uint32_t, statusStringBytes, 0x14) = 0;
                CSF_AT(uint16_t, statusStringBytes, 4) = 0;
            }
            else
            {
                TextInputSubobject_Init(
                    reinterpret_cast<S_004f21d0*>(descriptionStringBytes),
                    nullptr, &g_byte_82E65C);

                record = CSF_AT(uint8_t*, records, 4) + recordOffset;
                void* text = MenuMpStats_GetStringText(record);
                reinterpret_cast<void(__fastcall*)(
                    void*, void*, uint32_t, int, void*, uint32_t)>(
                    list->vtbl[0x294 / 4])(list, nullptr, row, 0, text, row);

                record = CSF_AT(uint8_t*, records, 4) + recordOffset;
                auto* description = reinterpret_cast<int16_t*>(
                    CoreParser_ResolveEntryByKey(
                        CSF_AT(uint32_t, record, 172), 12));
                S_004f21d0_AssignText(
                    reinterpret_cast<uint32_t*>(descriptionStringBytes),
                    nullptr, description);

                if (CSF_AT(uint32_t, descriptionStringBytes, 0x14) > 0xF)
                {
                    ReserveSsoOrHeapCapacity(
                        descriptionStringBytes, nullptr, 0xFu, 0xFFFFFFFF);
                    AppendNarrowToWideString(
                        descriptionStringBytes, nullptr,
                        reinterpret_cast<uint8_t*>(
                            const_cast<char*>(CSF_STR(0x0084319C, "..."))));
                }

                text = MenuMpStats_GetStringText(descriptionStringBytes);
                reinterpret_cast<void(__fastcall*)(
                    void*, void*, uint32_t, int, void*, uint32_t)>(
                    list->vtbl[0x294 / 4])(list, nullptr, row, 1, text, row);

                record = CSF_AT(uint8_t*, records, 4) + recordOffset;
                if (CSF_AT(int32_t, record, 188) == -1)
                    text = MenuMpStats_GetStringText(emptyStringBytes);
                else
                    text = reinterpret_cast<void*>(
                        MenuMpStats_RenderPanelByIndex(
                            CSF_AT(int32_t, record, 188)));

                reinterpret_cast<void(__fastcall*)(
                    void*, void*, uint32_t, int, void*, uint32_t)>(
                    list->vtbl[0x294 / 4])(list, nullptr, row, 2, text, row);

                S_004f21d0_SetPattern(
                    reinterpret_cast<S_004f21d0*>(dateStringBytes),
                    nullptr, &g_byte_82E65C);

                record = CSF_AT(uint8_t*, records, 4) + recordOffset;
                if (CSF_AT(int32_t, record, 28) == -1)
                {
                    text = MenuMpStats_GetStringText(dateStringBytes);
                }
                else
                {
                    void* formatted = reinterpret_cast<void*>(
                        ProfilesSavegames_FormatDateTime(
                            record + 28, dateStringBytes));
                    text = MenuMpStats_GetStringText(formatted);
                }

                reinterpret_cast<void(__fastcall*)(
                    void*, void*, uint32_t, int, void*, uint32_t)>(
                    list->vtbl[0x294 / 4])(list, nullptr, row, 3, text, row);

                if (CSF_AT(uint32_t, dateStringBytes, 0x18) >= 8)
                {
                    void* block = CSF_AT(void*, dateStringBytes, 4);
                    if (block)
                        Mem_FreeDebug(block);
                }
                CSF_AT(uint32_t, dateStringBytes, 0x18) = 7;
                CSF_AT(uint32_t, dateStringBytes, 0x14) = 0;
                CSF_AT(uint16_t, dateStringBytes, 4) = 0;
            }
        }

        CGuiListBoxBase_AddSelectedItem(list, nullptr, 0);
        if (CSF_AT(uint32_t, descriptionStringBytes, 0x18) >= 8)
        {
            void* block = CSF_AT(void*, descriptionStringBytes, 4);
            if (block)
                Mem_FreeDebug(block);
        }
    }

    if (CSF_AT(uint32_t, emptyStringBytes, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, emptyStringBytes, 4);
        if (block)
            Mem_FreeDebug(block);
    }
}

HOOK(0x00541230, MenuMpStats_PopulateList, "auto");
