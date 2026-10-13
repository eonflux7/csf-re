#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, NetProfile_RefreshValueChoices, 0x0069AE10, (void* self, void* edx));

using GetDlgItemFunction = void* (__stdcall*)(void*, int);
using GetWindowTextFunction = int (__stdcall*)(void*, char*, int);
using SetWindowTextFunction = int (__stdcall*)(void*, const char*);
using SendMessageFunction = int32_t (__stdcall*)(void*, uint32_t, uint32_t, intptr_t);

CSF_GLOBAL(GetDlgItemFunction, g_GetDlgItemImportSlot, 0x0082D2F0);
CSF_GLOBAL(GetWindowTextFunction, g_GetWindowTextAImportSlot, 0x0082D2F8);
CSF_GLOBAL(SetWindowTextFunction, g_SetWindowTextAImportSlot, 0x0082D2F4);
CSF_GLOBAL(SendMessageFunction, g_SendMessageAImportSlot, 0x0082D2EC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_GLOBAL(uint32_t, g_DeletedSuffixMiddle, 0x00840BFC);
CSF_GLOBAL(uint32_t, g_DeletedSuffixEnd, 0x00840C00);
CSF_GLOBAL(uint16_t, g_DeletedSuffixTerminator, 0x00840C04);

static int __fastcall NetProfile_RefreshGlobalValueTargets(void* self, void* /*edx*/)
{
    alignas(8) uint8_t textBytes[0x420];
    char* text = reinterpret_cast<char*>(textBytes);

    int result = CSF_AT(int, self, 0);
    if (result == 0 || CSF_AT(uint32_t, self, 0x14) == 0)
        return result;

    uint32_t index = 0;
    uint32_t entryOffset = 0;
    do
    {
        uint8_t* entry = CSF_AT(uint8_t*, self, 0x0C) + entryOffset;
        if (CSF_AT(uint8_t, entry, 8))
        {
            if (CSF_AT(int, entry, 0x14) < 0 ||
                CHandleTable_Resolve(
                    g_pCurrentScene, nullptr,
                    static_cast<int>(reinterpret_cast<intptr_t>(entry + 0x0C))))
            {
                S_0069ad60* choices = CSF_AT(S_0069ad60*, entry, 0);
                NetProfile_RefreshValueChoices(choices, nullptr);

                if (choices->field_007c)
                {
                    void* listWindow = g_GetDlgItemImportSlot(
                        CSF_AT(void*, CSF_AT(uint8_t*, self, 0x0C) + entryOffset, 4),
                        1003);
                    uint32_t selection = static_cast<uint32_t>(
                        g_SendMessageAImportSlot(listWindow, 392, 0, 0));

                    std::memset(text, 0, 1024);
                    uint32_t choiceIndex = 0;
                    if (choices->field_007c)
                    {
                        uint8_t* choiceSlot = reinterpret_cast<uint8_t*>(choices) + 0x3C;
                        do
                        {
                            int messageResult = g_SendMessageAImportSlot(
                                listWindow, 393, choiceIndex,
                                reinterpret_cast<intptr_t>(text));
                            const char* choiceText = CSF_AT(const char*, choiceSlot, 0);
                            if (messageResult == -1)
                            {
                                g_SendMessageAImportSlot(
                                    listWindow, 384, 0,
                                    reinterpret_cast<intptr_t>(choiceText));
                            }
                            else if (std::strcmp(text, choiceText))
                            {
                                SendMessageFunction sendMessage = g_SendMessageAImportSlot;
                                sendMessage(listWindow, 386, choiceIndex, 0);
                                sendMessage(
                                    listWindow, 385, choiceIndex,
                                    reinterpret_cast<intptr_t>(choiceText));
                            }

                            uint32_t choiceCount = choices->field_007c;
                            ++choiceIndex;
                            choiceSlot += 4;
                            if (choiceIndex >= choiceCount)
                                break;
                        } while (true);
                    }

                    void* valueWindow = g_GetDlgItemImportSlot(
                        CSF_AT(void*, CSF_AT(uint8_t*, self, 0x0C) + entryOffset, 4),
                        1008);
                    if (selection >= choices->field_007c)
                        selection = 0;

                    const char* selectedValue =
                        CSF_AT(const char*, choices, 4 * selection);
                    if (selectedValue)
                    {
                        g_GetWindowTextAImportSlot(valueWindow, text, 1024);
                        if (std::strcmp(selectedValue, text))
                            g_SetWindowTextAImportSlot(valueWindow, selectedValue);
                    }
                    g_SendMessageAImportSlot(listWindow, 390, selection, 0);
                }
            }
            else
            {
                std::memset(text, 0, 1024);
                g_GetWindowTextAImportSlot(CSF_AT(void*, entry, 4), text, 256);

                char* suffix = text + std::strlen(text);
                std::memcpy(suffix, CSF_STR(0x00840BF8, " * DELETED * "), 4);
                std::memcpy(suffix + 4, &g_DeletedSuffixMiddle, 4);
                std::memcpy(suffix + 8, &g_DeletedSuffixEnd, 4);
                std::memcpy(suffix + 12, &g_DeletedSuffixTerminator, 2);

                g_SetWindowTextAImportSlot(CSF_AT(void*, entry, 4), text);
                CSF_AT(uint8_t, entry, 8) = 0;
            }
        }

        result = static_cast<int>(index + 1);
        ++index;
        entryOffset += 24;
    } while (index < CSF_AT(uint32_t, self, 0x14));

    return result;
}

HOOK(0x00518A60, NetProfile_RefreshGlobalValueTargets, "auto");
