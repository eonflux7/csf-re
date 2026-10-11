#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Stdcall ReadFile-compatible pointer stored in the game's import slot.
// BOOL is returned as a 32-bit int; the callee pops its five arguments.
using ReadFileProc = int(__stdcall*)(void* hFile, void* lpBuffer, uint32_t nNumberOfBytesToRead,
                                     uint32_t* lpNumberOfBytesRead, void* lpOverlapped);

CSF_GLOBAL(ReadFileProc, g_pfnReadFile, 0x82D238);

// Reads elementCount elements of elementSize bytes from file into buffer.
// Returns the number of whole elements read (bytes read / elementSize),
// or 0 when the read call fails.
static uint32_t __cdecl Audio_ReadElementsFromFile(void* buffer, uint32_t elementSize,
                                                   uint32_t elementCount, void* file) {
    uint32_t bytesRead = 0;
    uint32_t bytesToRead = elementSize * elementCount;

    if (g_pfnReadFile(file, buffer, bytesToRead, &bytesRead, nullptr)) {
        return bytesRead / elementSize;
    }
    return 0;
}

HOOK(0x70DF60, Audio_ReadElementsFromFile, "auto");
