#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Import slot for SetFilePointer (kernel32), stored at 0x82D230 in the original's IAT.
using SetFilePointerFn = uint32_t(__stdcall*)(void* hFile, int32_t lDistanceToMove, int32_t* lpDistanceToMoveHigh, uint32_t dwMoveMethod);
CSF_GLOBAL(SetFilePointerFn, g_SetFilePointerImport, 0x82D230);

// Returns 1 when SetFilePointer reports INVALID_SET_FILE_POINTER (0xFFFFFFFF), else 0.
// a4 selects the move method: 0 = FILE_BEGIN, 1 = FILE_CURRENT, 2 = FILE_END.
static int __cdecl AudioFile_SeekFileChecked(void* hFile, int32_t lDistanceToMove, int a3, int a4)
{
    (void)a3;
    int result = 0;
    switch (a4)
    {
    case 0:
    {
        uint32_t position = g_SetFilePointerImport(hFile, lDistanceToMove, nullptr, 0);
        return position == 0xFFFFFFFFu ? 1 : 0;
    }
    case 1:
    {
        uint32_t position = g_SetFilePointerImport(hFile, lDistanceToMove, nullptr, 1);
        return position == 0xFFFFFFFFu ? 1 : 0;
    }
    case 2:
    {
        uint32_t position = g_SetFilePointerImport(hFile, lDistanceToMove, nullptr, 2);
        return position == 0xFFFFFFFFu ? 1 : 0;
    }
    }
    return result;
}

HOOK(0x70DFB0, AudioFile_SeekFileChecked, "auto");
