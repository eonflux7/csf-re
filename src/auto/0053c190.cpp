#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ShowSaveGameMessage, 0x0050F860, (void* self, void* , int text, int messageAction, int configuration));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(int, __fastcall, MenuAccount_IsGateClear_696B60, 0x00696B60, (S_00696af0* self, void* , char* a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static char __fastcall CMenuProfilesPC_CheckEnteredProfileName(
    CMenuProfilesPC* self, void* /*edx*/, S_00696af0* enteredName)
{
    char rejected = static_cast<char>(
        MenuAccount_IsGateClear_696B60(
            enteredName, nullptr, reinterpret_cast<char*>(&g_byte_82E65C)));

    if (!rejected)
    {
        const uint32_t length = CSF_AT(uint32_t, enteredName, 0x14);
        uint32_t index = 0;
        rejected = 1;

        if (length != 0)
        {
            while (rejected)
            {
                const uint16_t* characters;
                if (enteredName->field_0018 < 8)
                    characters = reinterpret_cast<const uint16_t*>(
                        &enteredName->field_0004);
                else
                    characters = reinterpret_cast<const uint16_t*>(
                        enteredName->field_0004);

                if (characters[index] != 32)
                    rejected = 0;

                if (++index >= length)
                    break;
            }
        }
    }

    if (!rejected)
    {
        self->field_0038 = 0xFFFFFFFF;
        return rejected;
    }

    alignas(8) uint8_t messageBytes[0x1C];
    CSF_AT(uint32_t, messageBytes, 0x14) = 0;
    CSF_AT(uint16_t, messageBytes, 0x04) = 0;
    CSF_AT(uint32_t, messageBytes, 0x18) = 7;

    InitStringFromLiteral(messageBytes, CSF_STR(0x00842994, "pr14"), 1);

    void* messageText = CSF_AT(void*, messageBytes, 0x04);
    if (CSF_AT(uint32_t, messageBytes, 0x18) < 8)
        messageText = messageBytes + 0x04;

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    GameBddLoader_ShowSaveGameMessage(
        globalState, nullptr, reinterpret_cast<int>(messageText), 16, 0);

    const bool heapAllocated = CSF_AT(uint32_t, messageBytes, 0x18) >= 8;
    self->field_0038 = 6;
    if (heapAllocated)
    {
        void* block = CSF_AT(void*, messageBytes, 0x04);
        if (block)
            Mem_FreeDebug(block);
    }

    return rejected;
}

HOOK(0x0053C190, CMenuProfilesPC_CheckEnteredProfileName, "auto");
