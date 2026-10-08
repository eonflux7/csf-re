#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

CSF_FUNC(int, __stdcall, MenuMpAccounts_HasInvalidAccountNameCharacters, 0x0055C240, (const char* accountName, int flags));
CSF_FUNC(void*, __fastcall, GetContainerDataPtr, 0x00721270, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560, (uint16_t* text));
CSF_FUNC(int, __cdecl, WideString_Compare, 0x00695760, (uint16_t* first, int16_t* second));

// Returns the account-edit validation code:
//   6 if the network type is 2 (no validation) or all fields are valid,
//   1 if the account name is empty,
//   3 if the account name contains invalid characters,
//   4 if the password length is outside [4, 16],
//   5 if the password and its confirmation do not match.
static int __fastcall MenuMpAccounts_ValidateAccountEditFields(void* self, void* /*edx*/)
{
    if (g_MpNetworkType == 2)
        return 6;

    if (CSF_AT(uint8_t, self, 0x3C) == 0)
        return 1;

    const char* accountName = reinterpret_cast<const char*>(static_cast<uint8_t*>(self) + 0x3C);
    const int invalidCharactersResult = MenuMpAccounts_HasInvalidAccountNameCharacters(accountName, 0);
    // The original only tests AL (the low byte of the BOOL result), so only the low byte is checked.
    if (static_cast<uint8_t>(invalidCharactersResult) != 0)
        return 3;

    void* passwordString = GetContainerDataPtr(CSF_AT(void*, self, 0x24), nullptr);
    uint32_t passwordLength = static_cast<uint32_t>(GetWideTextLength(static_cast<uint16_t*>(passwordString)));
    if (passwordLength < 4 || passwordLength > 0x10)
        return 4;

    void* confirmString = GetContainerDataPtr(CSF_AT(void*, self, 0x28), nullptr);
    void* passwordStringAgain = GetContainerDataPtr(CSF_AT(void*, self, 0x24), nullptr);
    int comparison = WideString_Compare(static_cast<uint16_t*>(passwordStringAgain),
                                        static_cast<int16_t*>(confirmString));
    if (comparison == 0)
        return 6;

    return 5;
}

HOOK(0x0055C780, MenuMpAccounts_ValidateAccountEditFields, "auto");
