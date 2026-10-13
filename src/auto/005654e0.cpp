#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ShowSaveGameMessage, 0x0050F860, (void* self, void* , int text, int messageAction, int configuration));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreParser_FormatScenarioText, 0x00692660, (int outputString, int descriptionText, int count, int flag));
CSF_FUNC(unsigned int, __cdecl, StringFormat_CopyTrucoStringToWide, 0x00695590, (char* src, uint16_t* dst, int maxCount));
CSF_FUNC(int, __fastcall, CGuiListBox_GetItemCount_714860, 0x00714860, (CGuiListBox* self, void*));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));

static char* __fastcall MenuAccounts_ShowSelectedAccountMessage(
    void* self, void* /*edx*/)
{
    int result = CGuiListBox_GetItemCount_714860(
        CSF_AT(CGuiListBox*, self, 0x14), nullptr);
    if (result <= 0)
        return reinterpret_cast<char*>(result);

    GetGlobalValue_8B056C();
    void* accountManager = CSF_AT(
        void*, reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x14);
    void** accountManagerVtable = CSF_AT(void**, accountManager, 0);

    int rowIndex = GetStoredValueAtIndex(
        CSF_AT(void*, self, 0x14), nullptr, 0);
    int accountIndex = GetIndexedTableValue_714C20(
        CSF_AT(int*, self, 0x14), nullptr, rowIndex, 0);

    char* account =
        reinterpret_cast<char* (__fastcall*)(void*, void*, int)>(
            accountManagerVtable[0x30 / 4])(
                accountManager, nullptr, accountIndex);
    if (!account)
        return nullptr;

    alignas(8) uint8_t wideNameBytes[0x44];
    // The four-byte string prefix and its adjacent 0x18-byte storage
    // must remain contiguous for the formatter.
    alignas(8) uint8_t formattedStringBytes[0x1C];
    uint8_t* stringStorage = formattedStringBytes + 4;

    CSF_AT(uint32_t, stringStorage, 0x14) = 7;
    CSF_AT(uint32_t, stringStorage, 0x10) = 0;
    CSF_AT(uint16_t, stringStorage, 0) = 0;

    StringFormat_CopyTrucoStringToWide(
        account + 8, reinterpret_cast<uint16_t*>(wideNameBytes), 33);
    CoreParser_FormatScenarioText(
        reinterpret_cast<int>(formattedStringBytes),
        reinterpret_cast<int>(CSF_STR(0x00845B90, "ae04")),
        reinterpret_cast<int>(wideNameBytes), 1);

    void* messageText = CSF_AT(void*, stringStorage, 0);
    if (CSF_AT(uint32_t, stringStorage, 0x14) < 8)
        messageText = stringStorage;

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    result = GameBddLoader_ShowSaveGameMessage(
        globalState, nullptr, reinterpret_cast<int>(messageText), 12, 0);

    bool usesInlineStorage = CSF_AT(uint32_t, stringStorage, 0x14) < 8;
    CSF_AT(int, self, 0x28) = 2;
    if (!usesInlineStorage)
    {
        void* block = CSF_AT(void*, stringStorage, 0);
        result = reinterpret_cast<int>(block);
        if (block)
            result = Mem_FreeDebug(block);
    }

    return reinterpret_cast<char*>(result);
}

HOOK(0x005654E0, MenuAccounts_ShowSelectedAccountMessage, "auto");
