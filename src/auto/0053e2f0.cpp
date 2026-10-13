#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, CHud_GetLinkedWordIfCountAbove1, 0x0042EB80, (CHud* self, void*));
CSF_FUNC(int, __fastcall, CHud_GetIndexedValue, 0x0042EBA0, (CHud* self, void* , int entry));
CSF_FUNC(int, __fastcall, MenuMissionInfo_AppendCharacterEntries, 0x00456F50, (void* self, void* , S_0041e5f0* a2, char a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(unsigned int, __fastcall, SchedulerBase_GetNumberOfBoundContexts, 0x004FE490, (void* self, void*));
CSF_FUNC(uint32_t, __fastcall, GetPageCountForContexts, 0x00681F30, (uint32_t contextCount, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreParser_ResolveEntryByKey, 0x00692440, (unsigned int key, int flags));
CSF_FUNC(int, __cdecl, StringFormat_CompareWideAgainstNarrow, 0x00695500, (uint16_t* a1, char* a2));
CSF_FUNC(unsigned int, __cdecl, StringFormat_CopyTrucoStringToWide, 0x00695590, (char* src, uint16_t* dst, int maxCount));
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0,
         (void* self, void* edx, const uint16_t* text));
CSF_FUNC(char, __cdecl, MenuMissionInfo_FormatRank, 0x007614B0, (int rankIndex, int destinationAddress, int capacity));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(uint16_t, g_EmptyPanelText, 0x00842D6C);

static void* MenuMissionInfo_FindWidget(void* panel, const char* name)
{
    return reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, panel, 0)[0xFC / 4])(panel, nullptr, name);
}

static void MenuMissionInfo_SetWidgetImage(void* widget, void* image)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, widget, 0)[0xD8 / 4])(widget, nullptr, image);
}

static void MenuMissionInfo_SetWidgetDisabled(void* widget, uint8_t disabled)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, uint8_t)>(
        CSF_AT(void**, widget, 0)[0xA8 / 4])(widget, nullptr, disabled);
}

static int MenuMissionInfo_IsPanelDisabled(void* panel)
{
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, panel, 0)[0xAC / 4])(panel, nullptr);
}

static void* MenuMissionInfo_AcquireImage(void* resource)
{
    return reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, resource, 0)[0x80 / 4])(resource, nullptr);
}

struct MissionInfoRowWidgets {
    void* photo;
    void* crossedPhoto;
    void* counter;
    void* name;
    void* rank;
    void* rankIcon;
    void* points;
    void* pointsIcon;
    void* notes;
};

static int __fastcall CMenuMissionInfo_RefreshPanelAtIndex(
    CMenuMissionInfo* self, void* /*edx*/, int page)
{
    void* panel = MenuMissionInfo_FindWidget(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842CD4, "PanelBichos"));
    if (!panel)
        return 0;

    // Preserve the adjacent buffers, including the widget-name overflow
    // into entry-list storage that is initialized afterwards.
    alignas(8) uint8_t workspace[0x314];
    auto* rankText = reinterpret_cast<uint16_t*>(workspace);
    auto* rows = reinterpret_cast<MissionInfoRowWidgets*>(workspace + 0x200);
    auto* pointsText = reinterpret_cast<uint16_t*>(workspace + 0x26C);
    auto* pointsNumber = reinterpret_cast<char*>(workspace + 0x2A8);
    auto* counterText = reinterpret_cast<uint16_t*>(workspace + 0x2C8);
    auto* counterNumber = reinterpret_cast<char*>(workspace + 0x2D4);
    auto* widgetName = reinterpret_cast<char*>(workspace + 0x2EC);
    void* entryList = workspace + 0x2F8;

    for (int rowNumber = 1; rowNumber <= 3; ++rowNumber) {
        MissionInfoRowWidgets& row = rows[rowNumber - 1];

        _sprintf(widgetName, CSF_STR(0x00842CCC, "Foto%d"), rowNumber);
        row.photo = MenuMissionInfo_FindWidget(panel, widgetName);
        if (row.photo)
            MenuMissionInfo_SetWidgetImage(row.photo, nullptr);

        _sprintf(widgetName, CSF_STR(0x00842CBC, "Foto%dTachada"), rowNumber);
        row.crossedPhoto = MenuMissionInfo_FindWidget(panel, widgetName);
        if (row.crossedPhoto)
            MenuMissionInfo_SetWidgetImage(row.crossedPhoto, nullptr);

        _sprintf(widgetName, CSF_STR(0x00842D80, "Foto%dContador"), rowNumber);
        row.counter = MenuMissionInfo_FindWidget(panel, widgetName);
        if (row.counter)
            MenuMissionInfo_SetWidgetDisabled(row.counter, 1);

        _sprintf(widgetName, CSF_STR(0x00842D70, "Foto%dNombre"), rowNumber);
        row.name = MenuMissionInfo_FindWidget(panel, widgetName);
        if (row.name)
            C_00720DF0_ForwardCallToChildVtblSlot16(row.name, nullptr, &g_EmptyPanelText);

        _sprintf(widgetName, CSF_STR(0x00842D60, "Foto%dRango"), rowNumber);
        row.rank = MenuMissionInfo_FindWidget(panel, widgetName);
        if (row.rank)
            C_00720DF0_ForwardCallToChildVtblSlot16(row.rank, nullptr, &g_EmptyPanelText);

        _sprintf(widgetName, CSF_STR(0x00842D50, "Foto%dRangoIcon"), rowNumber);
        row.rankIcon = MenuMissionInfo_FindWidget(panel, widgetName);
        if (row.rankIcon)
            MenuMissionInfo_SetWidgetImage(row.rankIcon, nullptr);

        _sprintf(widgetName, CSF_STR(0x00842D40, "Foto%dPuntos"), rowNumber);
        row.points = MenuMissionInfo_FindWidget(panel, widgetName);
        if (row.points)
            C_00720DF0_ForwardCallToChildVtblSlot16(row.points, nullptr, &g_EmptyPanelText);

        _sprintf(widgetName, CSF_STR(0x00842D2C, "Foto%dPuntosIcon"), rowNumber);
        row.pointsIcon = MenuMissionInfo_FindWidget(panel, widgetName);
        if (row.pointsIcon)
            MenuMissionInfo_SetWidgetImage(row.pointsIcon, nullptr);

        _sprintf(widgetName, CSF_STR(0x00842D20, "Foto%dNotas"), rowNumber);
        row.notes = MenuMissionInfo_FindWidget(panel, widgetName);
        if (row.notes)
            C_00720DF0_ForwardCallToChildVtblSlot16(row.notes, nullptr, &g_EmptyPanelText);
    }

    int panelState = MenuMissionInfo_IsPanelDisabled(panel);
    if (static_cast<uint8_t>(panelState))
        return panelState;

    CSF_AT(uint32_t, entryList, 0) = 0;
    CSF_AT(uint32_t, entryList, 4) = 0;
    CSF_AT(uint32_t, entryList, 8) = 0;
    CSF_AT(int, entryList, 0xC) = 10;

    auto contexts = SchedulerBase_GetNumberOfBoundContexts(
        reinterpret_cast<void*>(GetGlobalValue_8B0568()), nullptr);
    MenuMissionInfo_AppendCharacterEntries(
        reinterpret_cast<void*>(contexts), nullptr,
        reinterpret_cast<S_0041e5f0*>(entryList), 0);

    contexts = SchedulerBase_GetNumberOfBoundContexts(
        reinterpret_cast<void*>(GetGlobalValue_8B0568()), nullptr);
    MenuMissionInfo_AppendCharacterEntries(
        reinterpret_cast<void*>(contexts), nullptr,
        reinterpret_cast<S_0041e5f0*>(entryList), 1);

    uint32_t entryIndex = 3 * static_cast<uint32_t>(page);
    uint32_t entryOffset = 12 * static_cast<uint32_t>(page);
    void* entry = nullptr;

    for (int rowIndex = 0; rowIndex < 3; ++rowIndex, ++entryIndex, entryOffset += 4) {
        MissionInfoRowWidgets& row = rows[rowIndex];
        // The eight-byte decimal buffer may spill into the original photo
        // local; keep that local in the same contiguous workspace too.
        void*& photo = CSF_AT(void*, workspace, 0x2DC);
        photo = nullptr;

        if (entryIndex < CSF_AT(uint32_t, entryList, 8)) {
            void* candidate = CSF_AT(
                void*, CSF_AT(void*, entryList, 0), entryOffset);
            photo = CSF_AT(void*, candidate, 0x24);
            if (photo) {
                entry = CSF_AT(void*, CSF_AT(void*, entryList, 0), entryOffset);
                rankText[0] = 0;

                if (row.photo)
                    MenuMissionInfo_SetWidgetImage(row.photo, photo);

                if (row.crossedPhoto) {
                    void* counter = row.counter;
                    if (counter) {
                        MenuMissionInfo_SetWidgetImage(row.crossedPhoto, nullptr);
                        if (CSF_AT(uint8_t, CSF_AT(void*, entry, 0x2C), 0x4C)) {
                            MenuMissionInfo_SetWidgetDisabled(counter, 0);
                            _sprintf(counterNumber, CSF_STR(0x0083090C, "%d"),
                                     CSF_AT(int, entry, 0x30));
                            StringFormat_CopyTrucoStringToWide(counterNumber, counterText, 10);
                            C_00720DF0_ForwardCallToChildVtblSlot16(counter, nullptr, counterText);
                        } else {
                            MenuMissionInfo_SetWidgetDisabled(counter, 1);
                            if (CSF_AT(int, entry, 0x30))
                                MenuMissionInfo_SetWidgetImage(
                                    row.crossedPhoto, CSF_AT(void*, entry, 0x28));
                        }
                    }
                }

                if (row.name) {
                    auto* text = reinterpret_cast<uint16_t*>(CoreParser_ResolveEntryByKey(
                        CSF_AT(unsigned int, CSF_AT(void*, entry, 0x2C), 0x40), 12));
                    if (text && StringFormat_CompareWideAgainstNarrow(
                            text, reinterpret_cast<char*>(&g_byte_82E65C)))
                        C_00720DF0_ForwardCallToChildVtblSlot16(row.name, nullptr, text);
                }

                if (row.rank) {
                    MenuMissionInfo_FormatRank(
                        CSF_AT(int, entry, 0x38), reinterpret_cast<int>(rankText), 512);
                    C_00720DF0_ForwardCallToChildVtblSlot16(row.rank, nullptr, rankText);
                }

                int rankIndex = CSF_AT(int, entry, 0x38);
                void* rankResource = reinterpret_cast<void*>(CHud_GetIndexedValue(
                    reinterpret_cast<CHud*>(GetGlobal8afc78()), nullptr, rankIndex));
                if (rankResource) {
                    void* image = MenuMissionInfo_AcquireImage(rankResource);
                    if (image && row.rankIcon) {
                        MenuMissionInfo_SetWidgetImage(row.rankIcon, image);
                        ReleaseObjectReference(static_cast<uint8_t*>(image) + 4, nullptr);
                    }
                }

                void* pointsResource = reinterpret_cast<void*>(CHud_GetLinkedWordIfCountAbove1(
                    reinterpret_cast<CHud*>(GetGlobal8afc78()), nullptr));
                if (pointsResource) {
                    void* image = MenuMissionInfo_AcquireImage(pointsResource);
                    if (image && row.pointsIcon) {
                        MenuMissionInfo_SetWidgetImage(row.pointsIcon, image);
                        ReleaseObjectReference(static_cast<uint8_t*>(image) + 4, nullptr);
                    }
                }

                if (row.points) {
                    _sprintf(pointsNumber, CSF_STR(0x0083090C, "%d"),
                             CSF_AT(int, entry, 0x34));
                    StringFormat_CopyTrucoStringToWide(pointsNumber, pointsText, 60);
                    C_00720DF0_ForwardCallToChildVtblSlot16(row.points, nullptr, pointsText);
                }

                if (row.notes) {
                    auto* text = reinterpret_cast<uint16_t*>(CoreParser_ResolveEntryByKey(
                        CSF_AT(unsigned int, CSF_AT(void*, entry, 0x2C), 0x44), 12));
                    if (text && StringFormat_CompareWideAgainstNarrow(
                            text, reinterpret_cast<char*>(&g_byte_82E65C)))
                        C_00720DF0_ForwardCallToChildVtblSlot16(row.notes, nullptr, text);
                }
            }
        }

        int disabled = photo == nullptr;
        if (row.photo)
            MenuMissionInfo_SetWidgetDisabled(row.photo, disabled);
        if (row.crossedPhoto)
            MenuMissionInfo_SetWidgetDisabled(row.crossedPhoto, disabled);
        if (row.counter) {
            int counterDisabled = disabled ||
                !CSF_AT(uint8_t, CSF_AT(void*, entry, 0x2C), 0x4C);
            MenuMissionInfo_SetWidgetDisabled(row.counter, counterDisabled);
        }
        if (row.name)
            MenuMissionInfo_SetWidgetDisabled(row.name, disabled);
        if (row.rank)
            MenuMissionInfo_SetWidgetDisabled(row.rank, disabled);
        if (row.rankIcon)
            MenuMissionInfo_SetWidgetDisabled(row.rankIcon, disabled);
        if (row.points)
            MenuMissionInfo_SetWidgetDisabled(row.points, disabled);
        if (row.pointsIcon)
            MenuMissionInfo_SetWidgetDisabled(row.pointsIcon, disabled);
        if (row.notes)
            MenuMissionInfo_SetWidgetDisabled(row.notes, disabled);
    }

    void* previous = MenuMissionInfo_FindWidget(
        panel, CSF_STR(0x00842D0C, "BotonPanelAnterior"));
    MenuMissionInfo_SetWidgetDisabled(previous, page == 0);

    void* next = MenuMissionInfo_FindWidget(
        panel, CSF_STR(0x00842CF8, "BotonPanelSiguiente"));
    contexts = SchedulerBase_GetNumberOfBoundContexts(
        reinterpret_cast<void*>(GetGlobalValue_8B0568()), nullptr);
    int nextDisabled = 1;
    if (GetPageCountForContexts(contexts, nullptr)) {
        contexts = SchedulerBase_GetNumberOfBoundContexts(
            reinterpret_cast<void*>(GetGlobalValue_8B0568()), nullptr);
        uint64_t pageProduct = 2863311531LL *
            static_cast<uint32_t>(GetPageCountForContexts(contexts, nullptr) + 2);
        uint32_t lastPage = (static_cast<uint32_t>(pageProduct >> 32) >> 1) - 1;
        nextDisabled = static_cast<uint32_t>(page) == lastPage;
    }
    MenuMissionInfo_SetWidgetDisabled(next, nextDisabled);

    void* storage = CSF_AT(void*, entryList, 0);
    return storage ? CoreMemory_Free(storage) : 0;
}

HOOK(0x0053E2F0, CMenuMissionInfo_RefreshPanelAtIndex, "auto");
