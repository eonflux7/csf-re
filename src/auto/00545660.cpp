#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(C_0082d410*, __fastcall, C_0082d410_ConstructFromText, 0x00434150, (C_0082d410* self, void* , uint32_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ShowSaveGameMessage, 0x0050F860, (void* self, void* , int text, int messageAction, int configuration));
CSF_FUNC(int, __fastcall, SaveGames_GetEntryCount, 0x00511FD0, (void* self, void*));
CSF_FUNC(char, __cdecl, ReadFirstStoredIndexedValue, 0x00540FF0, (int listBox, int* outValue));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int*, __cdecl, MenuSaveGame_ClearSlotState, 0x00544480, (int* a1));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ActivateWithTitle, 0x00544750, (void* self, void*));
CSF_FUNC(void*, __fastcall, MenuSaveGame_InitEntryFromString, 0x00544D30, (void* self, void* , void* a2, const char* Source));
CSF_FUNC(std::uint32_t*, __fastcall, CMenuSaveGame_AppendElement36, 0x005454A0, (void* self, void* , int element));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* , uint8_t* a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszMenu_P_EditG, 0x00899B04);

static int __fastcall CMenuSaveGame_HandleButtonEvent(
    CMenuSaveGame* self, void* /*edx*/, unsigned int event)
{
    void* screen = reinterpret_cast<void*>(self->field_0044);
    int result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x104 / 4])(screen, nullptr);

    if (!result || self->field_0050 == 6)
        return result;

    alignas(8) uint8_t entryBytes[0x24];
    alignas(8) uint8_t stringBytes[0x20];
    C_0082d410& text = *reinterpret_cast<C_0082d410*>(stringBytes);
    int& selectedIndex = CSF_AT(int, stringBytes, 0x1C);
    const unsigned int eventAddress = event;

    if (self->field_001c &&
        self->field_001c == CSF_AT(uint32_t, reinterpret_cast<void*>(eventAddress), 4))
    {
        if (ReadFirstStoredIndexedValue(static_cast<int>(self->field_001c), &selectedIndex))
        {
            unsigned int count = SaveGames_GetEntryCount(
                reinterpret_cast<uint8_t*>(self) + 0x20, nullptr);
            if (static_cast<unsigned int>(selectedIndex) < count)
            {
                self->field_0050 = 1;
                self->field_0054 = static_cast<uint32_t>(selectedIndex);
                CSF_AT(uint32_t, stringBytes, 0x18) = 7;
                CSF_AT(uint32_t, stringBytes, 0x14) = 0;
                *reinterpret_cast<uint16_t*>(&text.field_0004) = 0;
                InitStringFromLiteral(&text, CSF_STR(0x00843BE0, "GEN2"), 1);

                uint32_t message = text.field_0004;
                if (CSF_AT(uint32_t, stringBytes, 0x18) < 8)
                    message = reinterpret_cast<uint32_t>(&text.field_0004);
                void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                GameBddLoader_ShowSaveGameMessage(
                    state, nullptr, static_cast<int>(message), 12, 0);
                SaveGame_DestroyLocalizedString(&text, nullptr);
            }
        }
    }

    if (self->field_0010 &&
        self->field_0010 == CSF_AT(uint32_t, reinterpret_cast<void*>(eventAddress), 4))
    {
        void* menu = reinterpret_cast<void*>(self->field_0040);
        selectedIndex = reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x5C / 4])(menu, nullptr, g_pszMenu_P_EditG);
        if (selectedIndex)
        {
            void* slotState = reinterpret_cast<uint8_t*>(self) + 0x60;
            void* editContext = reinterpret_cast<uint8_t*>(self) + 0x5C;
            self->field_0050 = 4;
            CSF_AT(CMenuSaveGame*, self, 0x5C) = self;
            MenuSaveGame_ClearSlotState(reinterpret_cast<int*>(slotState));

            C_0082d410_ConstructFromText(
                &text, nullptr,
                reinterpret_cast<uint32_t*>(const_cast<char*>(
                    CSF_STR(0x008413F0, "EstaticoEdit"))));
            void* entry = MenuSaveGame_InitEntryFromString(
                entryBytes, nullptr, &text, CSF_STR(0x00843BD8, "GEN5"));
            CMenuSaveGame_AppendElement36(
                slotState, nullptr, reinterpret_cast<int>(entry));
            CMenuSaveGame_ResetStringStorage(entryBytes, nullptr);
            CMenuSaveGame_ResetStringStorage(&text, nullptr);

            C_0082d410_ConstructFromText(
                &text, nullptr,
                reinterpret_cast<uint32_t*>(const_cast<char*>(
                    CSF_STR(0x008413D4, "BotonOkQuestion"))));
            entry = MenuSaveGame_InitEntryFromString(
                entryBytes, nullptr, &text, CSF_STR(0x00843BD0, "GEN3"));
            CMenuSaveGame_AppendElement36(
                slotState, nullptr, reinterpret_cast<int>(entry));
            CMenuSaveGame_ResetStringStorage(entryBytes, nullptr);
            CMenuSaveGame_ResetStringStorage(&text, nullptr);

            C_0082d410_ConstructFromText(
                &text, nullptr,
                reinterpret_cast<uint32_t*>(const_cast<char*>(
                    CSF_STR(0x008413C0, "BotonCancelQuestion"))));
            entry = MenuSaveGame_InitEntryFromString(
                entryBytes, nullptr, &text, CSF_STR(0x00843BC8, "GEN4"));
            CMenuSaveGame_AppendElement36(
                slotState, nullptr, reinterpret_cast<int>(entry));
            CMenuSaveGame_ResetStringStorage(entryBytes, nullptr);
            CMenuSaveGame_ResetStringStorage(&text, nullptr);

            TextInputSubobject_Init(
                reinterpret_cast<S_004f21d0*>(reinterpret_cast<uint8_t*>(self) + 0x70),
                nullptr, &g_byte_82E65C);
            CSF_AT(uint8_t, self, 0x8C) = 1;
            CSF_AT(void*, reinterpret_cast<void*>(selectedIndex), 0x54) = editContext;

            void* status = reinterpret_cast<void*>(self->vSlot20());
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, status, 0)[0xC0 / 4])(status, nullptr, 0);

            menu = reinterpret_cast<void*>(self->field_0040);
            reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, menu, 0)[0x58 / 4])(menu, nullptr, g_pszMenu_P_EditG);
        }
    }

    if (self->field_0014 &&
        self->field_0014 == CSF_AT(uint32_t, reinterpret_cast<void*>(eventAddress), 4))
    {
        if (ReadFirstStoredIndexedValue(
                static_cast<int>(self->field_001c), reinterpret_cast<int*>(&event)))
        {
            unsigned int count = SaveGames_GetEntryCount(
                reinterpret_cast<uint8_t*>(self) + 0x20, nullptr);
            if (event < count)
            {
                self->field_0050 = 0;
                self->field_0054 = event;
                CSF_AT(uint32_t, stringBytes, 0x18) = 7;
                CSF_AT(uint32_t, stringBytes, 0x14) = 0;
                *reinterpret_cast<uint16_t*>(&text.field_0004) = 0;
                InitStringFromLiteral(&text, CSF_STR(0x00843BC0, "GEN1"), 1);

                uint32_t message = text.field_0004;
                if (CSF_AT(uint32_t, stringBytes, 0x18) < 8)
                    message = reinterpret_cast<uint32_t>(&text.field_0004);
                void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                GameBddLoader_ShowSaveGameMessage(
                    state, nullptr, static_cast<int>(message), 12, 0);
                SaveGame_DestroyLocalizedString(&text, nullptr);
            }
        }
    }

    result = static_cast<int>(self->field_0018);
    if (result &&
        static_cast<uint32_t>(result) ==
            CSF_AT(uint32_t, reinterpret_cast<void*>(eventAddress), 4))
        return CMenuSaveGame_ActivateWithTitle(self, nullptr);
    return result;
}

HOOK(0x00545660, CMenuSaveGame_HandleButtonEvent, "auto");
