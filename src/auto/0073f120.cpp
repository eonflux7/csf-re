#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ReturnZero, 0x004B2A30, ());
CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(int, __fastcall, Sub_647AA0, 0x00647AA0, (void* self, void* edx, int a, int b));
CSF_FUNC(void*, __fastcall, NetSession_GetManager, 0x006487D0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_6488A0, 0x006488A0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_6488B0, 0x006488B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_648910, 0x00648910, (void* self, void* edx, char* String2));
CSF_FUNC(int, __cdecl, sub_668250, 0x00668250, (int a1));
CSF_FUNC(void, __fastcall, sub_68ABC0, 0x0068ABC0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_68B270, 0x0068B270, (void* self, void* edx, int a2, unsigned int a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint8_t*, __fastcall, sub_73D4D0, 0x0073D4D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuCharSelect_FinalizePreviewSlot, 0x0073E920, (int* self, void* edx));
CSF_FUNC(char, __cdecl, sub_743980, 0x00743980, ());
CSF_FUNC(int, __cdecl, RegisterCallback_743A00, 0x00743A00, ());
CSF_FUNC(char, __fastcall, sub_74A450, 0x0074A450, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_78D130, 0x0078D130, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, sub_78DE80, 0x0078DE80, (void* self, void* edx, int a1));
CSF_FUNC(void*, __fastcall, sub_78E030, 0x0078E030, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_7A2930, 0x007A2930, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_7A6700, 0x007A6700, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_AccountFlagA, 0x008A9EC0);
CSF_GLOBAL(uint8_t, g_AccountFlagB, 0x008A9EC1);
CSF_GLOBAL(void*, g_AccountService, 0x008B99B0);
CSF_GLOBAL(uint8_t, g_MenuMpCharacterSelectLookup, 0x008B9A90);
CSF_GLOBAL(uint8_t, g_AccountConnectionFlag, 0x008BBF98);

static char __fastcall C_0085e760_SetAccountScreen(
    C_0085e760* self, void* /*edx*/, C_0085e760* accountScreen)
{
    alignas(8) uint8_t keyBytes[0x38];
    alignas(8) uint8_t encodedKeyBytes[0x1C];
    alignas(8) uint8_t optionResultBytes[0x14];

    // The packet labels this constructor address as a vtable reference.
    // The constructor itself performs any associated object initialization.
    (void)CSF_VTBL(0x0073D4D0);

    MenuCharSelect_FinalizePreviewSlot(reinterpret_cast<int*>(self), nullptr);

    void* previewStorage = Mem_Alloc(1456);
    uint8_t* preview = previewStorage
        ? sub_73D4D0(previewStorage, nullptr)
        : nullptr;

    CSF_AT(uint8_t*, self, 0xC8) = preview;
    CSF_AT(char, optionResultBytes, 0x13) = preview != nullptr;
    if (preview)
        CSF_AT(char, optionResultBytes, 0x13) = sub_74A450(preview, nullptr);

    CSF_AT(int, self, 0xC4) = -1;
    g_AccountFlagA = 0;
    g_AccountFlagB = 0;

    if (!CSF_AT(char, optionResultBytes, 0x13))
    {
        MenuCharSelect_FinalizePreviewSlot(reinterpret_cast<int*>(self), nullptr);
        return CSF_AT(char, optionResultBytes, 0x13);
    }

    // The caller's extra cdecl stack word is unused by this no-argument callee.
    sub_743980();

    if (!CSF_AT(void*, self, 0x15C))
    {
        void* service = NetZ_OperatorNew(4);
        if (service)
            CSF_AT(void**, service, 0) = CSF_VTBL(0x008676AC);
        CSF_AT(void*, self, 0x15C) = service;
        g_AccountService = service;
    }

    void* interfaceStorage = NetZ_OperatorNew(4);
    uint32_t* interfaceObject = interfaceStorage
        ? sub_7A2930(interfaceStorage, nullptr)
        : nullptr;
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(interfaceObject);

    if (interfaceObject)
    {
        int zero = ReturnZero();
        int connection = Sub_647AA0(&g_MenuMpCharacterSelectLookup, nullptr, 6, zero);
        sub_78DE80(reinterpret_cast<void*>(connection), nullptr, 1);
        RegisterCallback_743A00();
    }
    else
    {
        CSF_AT(char, optionResultBytes, 0x13) = 0;
    }

    g_AccountConnectionFlag = 0;
    CSF_AT(int, self, 0x98) = 2;

    if (!CSF_AT(char, optionResultBytes, 0x13))
    {
        MenuCharSelect_FinalizePreviewSlot(reinterpret_cast<int*>(self), nullptr);
        return CSF_AT(char, optionResultBytes, 0x13);
    }

    void* manager = NetSession_GetManager(nullptr, nullptr);
    void* protocol = reinterpret_cast<void*>(sub_648910(
        manager, nullptr, const_cast<char*>(CSF_STR(0x0084468C, "prudp"))));

    if (!protocol)
    {
        CSF_AT(char, optionResultBytes, 0x13) = 0;
        MenuCharSelect_FinalizePreviewSlot(reinterpret_cast<int*>(self), nullptr);
        return CSF_AT(char, optionResultBytes, 0x13);
    }

    accountScreen = reinterpret_cast<C_0085e760*>(10000);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*, int, C_0085e760**)>(
        CSF_AT(void**, protocol, 0)[0x54 / 4])(
            protocol, nullptr, optionResultBytes, 259, &accountScreen);

    accountScreen = reinterpret_cast<C_0085e760*>(100);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*, int, C_0085e760**)>(
        CSF_AT(void**, protocol, 0)[0x54 / 4])(
            protocol, nullptr, optionResultBytes, 257, &accountScreen);

    CSF_AT(void*, optionResultBytes, 0xC) = NetSession_GetManager(nullptr, nullptr);

    void* sessionStorage = NetZ_OperatorNew(88248);
    self->field_0004 = sessionStorage
        ? sub_7A6700(sessionStorage, nullptr)
        : 0;

    void* accountStorage = NetZ_OperatorNew(564);
    accountScreen = accountStorage
        ? reinterpret_cast<C_0085e760*>(sub_78E030(accountStorage, nullptr))
        : nullptr;
    self->field_0008 = reinterpret_cast<uint32_t>(accountScreen);

    std::memcpy(keyBytes, CSF_STR(0x00867714, "?knihTuoYtnoDyeKeruceStsoMehTsaHecroFekirtSsodnammoC"), 53);
    void* encodedKey = sub_68B270(
        encodedKeyBytes, nullptr, reinterpret_cast<int>(keyBytes), 53);
    sub_78D130(accountScreen, nullptr, static_cast<uint32_t*>(encodedKey));
    sub_68ABC0(encodedKeyBytes, nullptr);

    int session = static_cast<int>(self->field_0004);
    void* savedManager = CSF_AT(void*, optionResultBytes, 0xC);
    sub_6488A0(savedManager, nullptr, session);
    sub_6488B0(savedManager, nullptr, static_cast<int>(self->field_0008));
    sub_668250(static_cast<int>(CSF_ADDR_FN(0x0073DB20)));

    return CSF_AT(char, optionResultBytes, 0x13);
}

HOOK(0x0073F120, C_0085e760_SetAccountScreen, "auto");
