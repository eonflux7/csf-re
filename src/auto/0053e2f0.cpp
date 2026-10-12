#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, CHud_GetLinkedWordIfCountAbove1, 0x0042EB80, (CHud* self, void*));
CSF_FUNC(int, __fastcall, CHud_GetIndexedValue, 0x0042EBA0,  (CHud* self, void* , int entry));
CSF_FUNC(int, __fastcall, MenuMissionInfo_AppendCharacterEntries, 0x00456F50, (void* self, void* edx, void* a2, char a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(unsigned int, __fastcall, SchedulerBase_GetNumberOfBoundContexts, 0x004FE490, (void* self, void*));
CSF_FUNC(uint32_t, __fastcall, GetPageCountForContexts, 0x00681F30, (uint32_t contextCount, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(int, __cdecl, MenuMessageBox_TestEntryAgainstFlag, 0x00695500, (uint16_t* entry, char* flagByte));
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x00695590, (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0,
         (void* self, void* edx, const uint16_t* text));
CSF_FUNC(char, __cdecl, MenuMissionInfo_FormatRank, 0x007614B0, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(uint16_t, g_EmptyPanelText, 0x00842D6C);

static void* MenuMissionInfo_FindChild(void* object, const char* name)
{
    return reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, object, 0)[252 / 4])(object, nullptr, name);
}

static void MenuMissionInfo_SetHidden(void* object, int hidden)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, object, 0)[168 / 4])(object, nullptr, hidden);
}

static void MenuMissionInfo_SetImage(void* object, void* image)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, object, 0)[216 / 4])(object, nullptr, image);
}

static void* MenuMissionInfo_AcquireImage(void* object)
{
    return reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[128 / 4])(object, nullptr);
}

static int __fastcall CMenuMissionInfo_RefreshPanelAtIndex(
    CMenuMissionInfo* self, void* /*edx*/, int page)
{
    void* panel = MenuMissionInfo_FindChild(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842CD4, "PanelBichos"));
    if (!panel)
        return 0;

    // Keep the adjoining original buffers together: formatted control names
    // extend beyond the twelve bytes assigned to the name buffer.
    alignas(8) uint8_t frame[0x314];
    auto* rankText = reinterpret_cast<uint16_t*>(frame);
    auto* rowControls = reinterpret_cast<void**>(frame + 0x200);
    auto* pointsText = reinterpret_cast<uint16_t*>(frame + 0x26C);
    auto* pointsNumber = reinterpret_cast<char*>(frame + 0x2A8);
    auto* countText = reinterpret_cast<uint16_t*>(frame + 0x2C8);
    auto* countNumber = reinterpret_cast<char*>(frame + 0x2D4);
    auto* controlName = reinterpret_cast<char*>(frame + 0x2EC);
    void* entries = frame + 0x2F8;

    for (int number = 1; number <= 3; ++number)
    {
        void** controls = rowControls + (number - 1) * 9;

        _sprintf(controlName, CSF_STR(0x00842CCC, "Foto%d"), number);
        void* control = MenuMissionInfo_FindChild(panel, controlName);
        controls[0] = control;
        if (control)
            MenuMissionInfo_SetImage(control, nullptr);

        _sprintf(controlName, CSF_STR(0x00842CBC, "Foto%dTachada"), number);
        control = MenuMissionInfo_FindChild(panel, controlName);
        controls[1] = control;
        if (control)
            MenuMissionInfo_SetImage(control, nullptr);

        _sprintf(controlName, CSF_STR(0x00842D80, "Foto%dContador"), number);
        control = MenuMissionInfo_FindChild(panel, controlName);
        controls[2] = control;
        if (control)
            MenuMissionInfo_SetHidden(control, 1);

        _sprintf(controlName, CSF_STR(0x00842D70, "Foto%dNombre"), number);
        control = MenuMissionInfo_FindChild(panel, controlName);
        controls[3] = control;
        if (control)
            C_00720DF0_ForwardCallToChildVtblSlot16(
                control, nullptr, &g_EmptyPanelText);

        _sprintf(controlName, CSF_STR(0x00842D60, "Foto%dRango"), number);
        control = MenuMissionInfo_FindChild(panel, controlName);
        controls[4] = control;
        if (control)
            C_00720DF0_ForwardCallToChildVtblSlot16(
                control, nullptr, &g_EmptyPanelText);

        _sprintf(controlName, CSF_STR(0x00842D50, "Foto%dRangoIcon"), number);
        control = MenuMissionInfo_FindChild(panel, controlName);
        controls[5] = control;
        if (control)
            MenuMissionInfo_SetImage(control, nullptr);

        _sprintf(controlName, CSF_STR(0x00842D40, "Foto%dPuntos"), number);
        control = MenuMissionInfo_FindChild(panel, controlName);
        controls[6] = control;
        if (control)
            C_00720DF0_ForwardCallToChildVtblSlot16(
                control, nullptr, &g_EmptyPanelText);

        _sprintf(controlName, CSF_STR(0x00842D2C, "Foto%dPuntosIcon"), number);
        control = MenuMissionInfo_FindChild(panel, controlName);
        controls[7] = control;
        if (control)
            MenuMissionInfo_SetImage(control, nullptr);

        _sprintf(controlName, CSF_STR(0x00842D20, "Foto%dNotas"), number);
        control = MenuMissionInfo_FindChild(panel, controlName);
        controls[8] = control;
        if (control)
            C_00720DF0_ForwardCallToChildVtblSlot16(
                control, nullptr, &g_EmptyPanelText);
    }

    bool panelHidden = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
        CSF_AT(void**, panel, 0)[172 / 4])(panel, nullptr);
    if (panelHidden)
        return 1;

    CSF_AT(void*, entries, 0x0) = nullptr;
    CSF_AT(uint32_t, entries, 0x4) = 0;
    CSF_AT(uint32_t, entries, 0x8) = 0;
    CSF_AT(int, entries, 0xC) = 10;

    int global = GetGlobalValue_8B0568();
    unsigned int contexts = SchedulerBase_GetNumberOfBoundContexts(
        reinterpret_cast<void*>(global), nullptr);
    MenuMissionInfo_AppendCharacterEntries(
        reinterpret_cast<void*>(contexts), nullptr, entries, 0);

    global = GetGlobalValue_8B0568();
    contexts = SchedulerBase_GetNumberOfBoundContexts(
        reinterpret_cast<void*>(global), nullptr);
    MenuMissionInfo_AppendCharacterEntries(
        reinterpret_cast<void*>(contexts), nullptr, entries, 1);

    uint32_t entryIndex = 3 * static_cast<uint32_t>(page);
    uint32_t byteOffset = 12 * static_cast<uint32_t>(page);
    void* currentEntry = nullptr;

    for (int row = 0; row < 3; ++row, ++entryIndex, byteOffset += 4)
    {
        void** controls = rowControls + row * 9;
        void* portrait = nullptr;

        if (entryIndex < CSF_AT(uint32_t, entries, 0x8))
        {
            portrait = CSF_AT(void*,
                CSF_AT(void*, CSF_AT(void*, entries, 0x0), byteOffset), 0x24);

            if (portrait)
            {
                currentEntry = CSF_AT(
                    void*, CSF_AT(void*, entries, 0x0), byteOffset);
                rankText[0] = 0;

                if (controls[0])
                    MenuMissionInfo_SetImage(controls[0], portrait);

                if (controls[1])
                {
                    void* counter = controls[2];
                    if (counter)
                    {
                        MenuMissionInfo_SetImage(controls[1], nullptr);
                        if (CSF_AT(uint8_t,
                                   CSF_AT(void*, currentEntry, 0x2C), 0x4C))
                        {
                            MenuMissionInfo_SetHidden(counter, 0);
                            _sprintf(countNumber, CSF_STR(0x0083090C, "%d"),
                                     CSF_AT(int, currentEntry, 0x30));
                            CopyTrucoStringToWide(countNumber, countText, 10);
                            C_00720DF0_ForwardCallToChildVtblSlot16(
                                counter, nullptr, countText);
                        }
                        else
                        {
                            MenuMissionInfo_SetHidden(counter, 1);
                            if (CSF_AT(int, currentEntry, 0x30))
                                MenuMissionInfo_SetImage(
                                    controls[1], CSF_AT(void*, currentEntry, 0x28));
                        }
                    }
                }

                if (controls[3])
                {
                    int name = MenuMessageBox_ResolveEntryByKey(
                        CSF_AT(int, CSF_AT(void*, currentEntry, 0x2C), 0x40), 12);
                    if (name && MenuMessageBox_TestEntryAgainstFlag(
                                    reinterpret_cast<uint16_t*>(name),
                                    reinterpret_cast<char*>(&g_byte_82E65C)))
                        C_00720DF0_ForwardCallToChildVtblSlot16(
                            controls[3], nullptr, reinterpret_cast<uint16_t*>(name));
                }

                if (controls[4])
                {
                    MenuMissionInfo_FormatRank(
                        CSF_AT(int, currentEntry, 0x38),
                        reinterpret_cast<int>(rankText), 512);
                    C_00720DF0_ForwardCallToChildVtblSlot16(
                        controls[4], nullptr, rankText);
                }

                int rank = CSF_AT(int, currentEntry, 0x38);
                auto* hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
                int rankObject = CHud_GetIndexedValue(hud, nullptr, rank);
                if (rankObject)
                {
                    void* image = MenuMissionInfo_AcquireImage(
                        reinterpret_cast<void*>(rankObject));
                    if (image && controls[5])
                    {
                        MenuMissionInfo_SetImage(controls[5], image);
                        ReleaseObjectReference(
                            static_cast<uint8_t*>(image) + 4, nullptr);
                    }
                }

                hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
                int pointsObject = CHud_GetLinkedWordIfCountAbove1(hud, nullptr);
                if (pointsObject)
                {
                    void* image = MenuMissionInfo_AcquireImage(
                        reinterpret_cast<void*>(pointsObject));
                    if (image && controls[7])
                    {
                        MenuMissionInfo_SetImage(controls[7], image);
                        ReleaseObjectReference(
                            static_cast<uint8_t*>(image) + 4, nullptr);
                    }
                }

                if (controls[6])
                {
                    _sprintf(pointsNumber, CSF_STR(0x0083090C, "%d"),
                             CSF_AT(int, currentEntry, 0x34));
                    CopyTrucoStringToWide(pointsNumber, pointsText, 60);
                    C_00720DF0_ForwardCallToChildVtblSlot16(
                        controls[6], nullptr, pointsText);
                }

                if (controls[8])
                {
                    int notes = MenuMessageBox_ResolveEntryByKey(
                        CSF_AT(int, CSF_AT(void*, currentEntry, 0x2C), 0x44), 12);
                    if (notes && MenuMessageBox_TestEntryAgainstFlag(
                                     reinterpret_cast<uint16_t*>(notes),
                                     reinterpret_cast<char*>(&g_byte_82E65C)))
                        C_00720DF0_ForwardCallToChildVtblSlot16(
                            controls[8], nullptr, reinterpret_cast<uint16_t*>(notes));
                }
            }
        }

        int hidden = portrait == nullptr;
        if (controls[0])
            MenuMissionInfo_SetHidden(controls[0], hidden);
        if (controls[1])
            MenuMissionInfo_SetHidden(controls[1], hidden);
        if (controls[2])
        {
            int counterHidden = hidden ||
                !CSF_AT(uint8_t, CSF_AT(void*, currentEntry, 0x2C), 0x4C);
            MenuMissionInfo_SetHidden(controls[2], counterHidden);
        }
        for (int field = 3; field < 9; ++field)
        {
            if (controls[field])
                MenuMissionInfo_SetHidden(controls[field], hidden);
        }
    }

    void* previousButton = MenuMissionInfo_FindChild(
        panel, CSF_STR(0x00842D0C, "BotonPanelAnterior"));
    MenuMissionInfo_SetHidden(previousButton, page == 0);

    void* nextButton = MenuMissionInfo_FindChild(
        panel, CSF_STR(0x00842CF8, "BotonPanelSiguiente"));
    global = GetGlobalValue_8B0568();
    contexts = SchedulerBase_GetNumberOfBoundContexts(
        reinterpret_cast<void*>(global), nullptr);

    int nextHidden = 1;
    if (GetPageCountForContexts(contexts, nullptr))
    {
        global = GetGlobalValue_8B0568();
        contexts = SchedulerBase_GetNumberOfBoundContexts(
            reinterpret_cast<void*>(global), nullptr);
        uint32_t count = GetPageCountForContexts(contexts, nullptr);
        uint64_t scaledCount =
            uint64_t{0xAAAAAAAB} * static_cast<uint32_t>(count + 2);
        uint32_t lastPage = static_cast<uint32_t>(scaledCount >> 32);
        lastPage = (lastPage >> 1) - 1;
        nextHidden = static_cast<uint32_t>(page) == lastPage;
    }
    MenuMissionInfo_SetHidden(nextButton, nextHidden);

    void* allocation = CSF_AT(void*, entries, 0x0);
    if (allocation)
        return Mem_Free(allocation);
    return 0;
}

HOOK(0x0053E2F0, CMenuMissionInfo_RefreshPanelAtIndex, "auto");
