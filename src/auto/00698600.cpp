#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

static void* __cdecl CoreFiles_SetFileOption(char locked)
{
    void* result = g_pOptions;
    const uint8_t loggingEnabled = CSF_AT(uint8_t, result, 0x01);
    CSF_AT(char, result, 0x02) = locked;

    if (loggingEnabled)
    {
        alignas(8) uint8_t messageBytes[0x1C];
        C_0082d410& message =
            *reinterpret_cast<C_0082d410*>(messageBytes);

        reinterpret_cast<uint8_t*>(&message.field_0004)[0] = 0;
        CSF_AT(uint32_t, messageBytes, 0x14) = 0;
        CSF_AT(uint32_t, messageBytes, 0x18) = 15;

        if (locked)
        {
            AssignStringRange_429B60(
                &message, nullptr,
                const_cast<char*>(CSF_STR(0x0085E6D4, "- ACCESS LOCKED\r\n")),
                17);
        }
        else
        {
            AssignStringRange_429B60(
                &message, nullptr,
                const_cast<char*>(CSF_STR(0x0085E6C0, "- ACCESS UNLOCKED\r\n")),
                19);
        }

        result = MenuManager_AppendPageName(
            reinterpret_cast<S_00470ab0*>(
                static_cast<uint8_t*>(g_pOptions) + 0x30),
            nullptr, &message);

        if (CSF_AT(uint32_t, messageBytes, 0x18) >= 0x10)
        {
            result = reinterpret_cast<void*>(message.field_0004);
            if (result)
            {
                result = reinterpret_cast<void*>(
                    static_cast<uint32_t>(Mem_FreeDebug(result)));
            }
        }
    }

    return result;
}

HOOK(0x00698600, CoreFiles_SetFileOption, "auto");
