#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Import-address-table slots used by the original (indirect calls via [0x82d288] / [0x82d2ec]).
CSF_GLOBAL(void*, g_IsWindowImportSlot, 0x82D288);
CSF_GLOBAL(void*, g_SendMessageAImportSlot, 0x82D2EC);

// C runtime _vsnprintf (cdecl, 4 args).
CSF_FUNC(int, __cdecl, Crt_Vsnprintf, 0x7B0BEB, (char* buffer, size_t count, const char* format, char* varargs));

// Formats a log line into a 10000-byte buffer and sends it to the log window as WM_COPYDATA (0x4A).
// The window handle is cleared if it no longer refers to a live window.
static int32_t __cdecl CoreZlibLog_SendLogLine(void** window, const char* format, ...)
{
    char buffer[10000];
    uint32_t copyData[3];

    // Variadic arguments start right after the named 'format' parameter (cdecl stack layout).
    char* varargs = reinterpret_cast<char*>(&format) + sizeof(format);

    if (*window && !reinterpret_cast<int(__stdcall*)(void*)>(g_IsWindowImportSlot)(*window))
        *window = nullptr;

    int32_t result = Crt_Vsnprintf(buffer, 0x2710, format, varargs);

    void* targetWindow = *window;
    buffer[9999] = 0;

    if (targetWindow)
    {
        copyData[0] = 0;
        copyData[2] = reinterpret_cast<uint32_t>(buffer);
        copyData[1] = static_cast<uint32_t>(strlen(buffer) + 1);
        return reinterpret_cast<int(__stdcall*)(void*, uint32_t, uint32_t, uint32_t)>(g_SendMessageAImportSlot)(
            targetWindow, 0x4A, 0, reinterpret_cast<uint32_t>(copyData));
    }
    return result;
}

HOOK(0x6A1E30, CoreZlibLog_SendLogLine, "auto");
