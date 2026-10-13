#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"

CSF_FUNC(int, __cdecl, CoreErrorLog_CaptureStackAddresses, 0x0069D350,
         (int a1, unsigned int a2, void** lp));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB,
         (char* buffer, const char* format, ...));

static char __cdecl CoreErrorLog_FormatStackTrace(int errorContextAddress,
                                                char* output)
{
    void* errorContext = reinterpret_cast<void*>(errorContextAddress);
    void* context = CSF_AT(void*, errorContext, 0x4);
    void** stackPointer = CSF_AT(void**, context, 0xB4);

    alignas(8) uint8_t addressBytes[0x100];
    auto* addresses = reinterpret_cast<uint32_t*>(addressBytes);
    addresses[0] = CSF_AT(uint32_t, context, 0xB8);
    addresses[1] = 0;

    CoreErrorLog_CaptureStackAddresses(
        reinterpret_cast<int>(addresses + 1), 0x3Fu, stackPointer);

    alignas(8) uint8_t lineBytes[0x404];
    alignas(8) uint8_t descriptionBytes[0x400];
    char* line = reinterpret_cast<char*>(lineBytes);
    char* description = reinterpret_cast<char*>(descriptionBytes);

    std::memset(line, 0, 0x400);
    std::memset(description, 0, 0x400);
    *output = 0;

    for (int index = 0; index < 64; ++index)
    {
        const void* address = reinterpret_cast<const void*>(addresses[index]);
        if (!address)
            break;

        _sprintf(line, CSF_STR(0x0085E9C0, "0x%p : %s\n"),
                 address, description);
        std::strcat(output, line);
    }

    return 1;
}

HOOK(0x0069D3D0, CoreErrorLog_FormatStackTrace, "auto");
