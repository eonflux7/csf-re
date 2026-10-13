#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(char, __fastcall, NetChatDDL_WriteMessageData, 0x00656280, (void* self, void* edx, int a2, unsigned int a3));
CSF_FUNC(void*, __fastcall, GameConfig_WriteWordField, 0x006563D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char __fastcall C_00858180_AddExtendedChatMessage(
    C_00858180* self, void* /*edx*/,
    char firstField, char secondField, int /*unused*/,
    void** block, int inlineWord1, int inlineWord2, int inlineWord3,
    std::int16_t messageLength, unsigned int capacity, char finalField)
{
    alignas(8) std::uint8_t temporaryStringBytes[4];
    CSF_AT(std::uint32_t, temporaryStringBytes, 0) =
        reinterpret_cast<std::uint32_t>(self);
    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x008689C4, "MensajeChatEx")));

    alignas(8) std::uint8_t keyBytes[4];
    CSF_AT(int, keyBytes, 0) =
        HashTemporaryString(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    void* entry = FindOrCreateConfigEntry(
        self, nullptr, reinterpret_cast<int>(keyBytes));

    GameConfig_WriteDwordField(entry, nullptr, firstField);
    GameConfig_WriteDwordField(entry, nullptr, secondField);

    const std::uint16_t length = static_cast<std::uint16_t>(messageLength);
    GameConfig_WriteWordField(entry, nullptr, static_cast<char>(messageLength));

    std::uint32_t inlineStorage[4] = {
        reinterpret_cast<std::uint32_t>(block),
        static_cast<std::uint32_t>(inlineWord1),
        static_cast<std::uint32_t>(inlineWord2),
        static_cast<std::uint32_t>(inlineWord3)
    };
    void* messageData = block;
    if (capacity < 8)
        messageData = inlineStorage;

    NetChatDDL_WriteMessageData(
        entry, nullptr, reinterpret_cast<int>(messageData), length);
    GameConfig_WriteWordField(entry, nullptr, finalField);

    const char result = AddConfigEntry(self, nullptr, 1, entry);
    if (capacity >= 8 && block)
        Mem_FreeDebug(block);

    return result;
}

HOOK(0x00751060, C_00858180_AddExtendedChatMessage, "auto");
