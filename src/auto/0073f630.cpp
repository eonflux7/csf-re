#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, sub_73EEA0, 0x0073EEA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_73F080, 0x0073F080, (void* self, void* edx));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

using GetLocalTimeFunction = void (__stdcall*)(void*);
using GetComputerNameFunction = int (__stdcall*)(char*, std::uint32_t*);

CSF_GLOBAL(GetLocalTimeFunction, g_GetLocalTime, 0x0082D214);
CSF_GLOBAL(GetComputerNameFunction, g_GetComputerNameA, 0x0082D204);
CSF_GLOBAL(std::uint32_t, g_Session, 0x008BC098);

static bool __cdecl NetSession_RecreateSession()
{
    void* oldSession = reinterpret_cast<void*>(g_Session);
    if (oldSession)
    {
        sub_73EEA0(oldSession, nullptr);
        Mem_Free(oldSession);
        g_Session = 0;
    }

    alignas(8) std::uint8_t systemTimeBytes[0x10];
    g_GetLocalTime(systemTimeBytes);

    alignas(8) char computerName[0x10] = {};
    std::uint32_t nameSize = 15;
    g_GetComputerNameA(computerName, &nameSize);

    _sprintf(
        reinterpret_cast<char*>(CSF_ADDR_DATA(0x008BC0A0)),
        CSF_STR(0x0086774C, "_%s_%04d%02d%02d_%02d%02d"),
        computerName,
        static_cast<int>(CSF_AT(std::uint16_t, systemTimeBytes, 0x00)),
        static_cast<int>(CSF_AT(std::uint16_t, systemTimeBytes, 0x02)),
        static_cast<int>(CSF_AT(std::uint16_t, systemTimeBytes, 0x06)),
        static_cast<int>(CSF_AT(std::uint16_t, systemTimeBytes, 0x08)),
        static_cast<int>(CSF_AT(std::uint16_t, systemTimeBytes, 0x0A)));

    void* storage = Mem_Alloc(356);
    if (storage)
    {
        const int session = sub_73F080(storage, nullptr);
        g_Session = static_cast<std::uint32_t>(session);
        return session != 0;
    }

    g_Session = 0;
    return false;
}

HOOK(0x0073F630, NetSession_RecreateSession, "auto");
