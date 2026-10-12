#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350, (const char* path, const char* mode, int flags));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static void __cdecl CoreFiles_WriteLoadingLog()
{
    alignas(8) std::uint8_t bufferBytes[0x200];
    char* buffer = reinterpret_cast<char*>(bufferBytes);
    alignas(8) std::uint8_t messageBytes[0x20];
    C_0082d410& message = *reinterpret_cast<C_0082d410*>(messageBytes);

    void* options = g_pOptions;
    const std::uint8_t loggingEnabled = CSF_AT(std::uint8_t, options, 0x1);
    std::uint32_t entryIndex = 0;
    CSF_AT(std::uint8_t, options, 0x2) = 0;

    if (loggingEnabled)
    {
        CSF_AT(std::uint32_t, messageBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, messageBytes, 0x14) = 0;
        *reinterpret_cast<std::uint8_t*>(&message.field_0004) = 0;

        AssignStringRange_429B60(
            &message, nullptr,
            const_cast<char*>(CSF_STR(0x0085E6C0, "- ACCESS UNLOCKED\r\n")),
            19);
        MenuManager_AppendPageName(
            reinterpret_cast<S_00470ab0*>(
                static_cast<std::uint8_t*>(g_pOptions) + 0x30),
            nullptr, &message);

        if (CSF_AT(std::uint32_t, messageBytes, 0x18) >= 0x10 &&
            message.field_0004 != 0)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(message.field_0004));
        }

        options = g_pOptions;
        CSF_AT(std::uint32_t, messageBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, messageBytes, 0x14) = 0;
        *reinterpret_cast<std::uint8_t*>(&message.field_0004) = 0;
    }

    if (CSF_AT(std::uint8_t, options, 0x1))
    {
        void* file = OpenSceneFile(
            CSF_STR(0x0085E6F0, "LogCarga.txt"),
            CSF_STR(0x00840044, "w"), 0);
        if (file)
        {
            std::uint32_t entryOffset = 0;
            for (;;)
            {
                options = g_pOptions;
                const std::uint32_t begin = CSF_AT(std::uint32_t, options, 0x34);
                if (!begin)
                    break;

                const std::uint32_t end = CSF_AT(std::uint32_t, options, 0x38);
                const std::int32_t byteCount =
                    static_cast<std::int32_t>(end - begin);
                if (entryIndex >= static_cast<std::uint32_t>(byteCount / 28))
                    break;

                auto* entry = reinterpret_cast<std::uint8_t*>(
                    CSF_AT(std::uint32_t, options, 0x34) + entryOffset);
                const char* text;
                if (CSF_AT(std::uint32_t, entry, 0x18) < 0x10)
                    text = reinterpret_cast<const char*>(entry + 4);
                else
                    text = CSF_AT(const char*, entry, 0x4);

                _sprintf(buffer, CSF_STR(0x0085E6E8, "%s\r\n"), text);
                const unsigned int length =
                    static_cast<unsigned int>(std::strlen(buffer));
                reinterpret_cast<void (__fastcall*)(void*, void*, char*, unsigned int)>(
                    CSF_AT(void**, file, 0)[0xC / 4])(
                        file, nullptr, buffer, length);

                ++entryIndex;
                entryOffset += 28;
            }
            CloseSceneFile(file);
        }
    }
}

HOOK(0x006986E0, CoreFiles_WriteLoadingLog, "auto");
