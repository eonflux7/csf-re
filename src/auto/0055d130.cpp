#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __fastcall, MenuMpAccounts_ValidateAccountEditFields, 0x0055C780, (void* self, void*));
CSF_FUNC(void, __fastcall, MenuMpAccounts_CreateAccount, 0x0055CE90, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, MenuMpAccounts_UpdateSelectedAccount, 0x0055CF60, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(void*, __fastcall, GetContainerDataPtr, 0x00721270, (void* self, void*));

CSF_GLOBAL(int, g_SelectedAccountIndex, 0x0089A5E8);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

static void __fastcall S_0055c780_ResetOnSelect(
    S_0055c780* self, void* /*edx*/)
{
    // The packet also lists this callable entry as a vtable reference.
    (void)CSF_VTBL(0x0055C780);

    void* firstControl = reinterpret_cast<void*>(self->field_0020);
    self->field_006e = 1;
    if (firstControl)
    {
        auto* text = static_cast<std::uint16_t*>(
            GetContainerDataPtr(firstControl, nullptr));
        ConvertWideTextToNarrow(
            text, reinterpret_cast<char*>(&self->field_003c), 33);
    }

    void* secondControl = reinterpret_cast<void*>(self->field_0024);
    if (secondControl)
    {
        auto* text = static_cast<std::uint16_t*>(
            GetContainerDataPtr(secondControl, nullptr));
        ConvertWideTextToNarrow(text, &CSF_AT(char, self, 0x5D), 17);
    }

    self->field_0034 = g_MpNetworkType == 2;

    int request;
    switch (MenuMpAccounts_ValidateAccountEditFields(self, nullptr))
    {
    case 0:
        request = 1095053362;
        break;
    case 1:
        request = 1095053365;
        break;
    case 2:
        request = 1095053366;
        break;
    case 3:
        request = 1095053367;
        break;
    case 4:
        request = 1095053368;
        break;
    case 5:
        request = 1095053369;
        break;
    case 6:
        if (g_SelectedAccountIndex == -1)
            MenuMpAccounts_CreateAccount(self, nullptr);
        else
            MenuMpAccounts_UpdateSelectedAccount(self, nullptr);
        return;
    default:
        return;
    }

    self->field_0280 = 3;
    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    GameBddLoader_DispatchMenuRequest(
        globalState, nullptr, request, 1, 1, 0);
}

HOOK(0x0055D130, S_0055c780_ResetOnSelect, "auto");
