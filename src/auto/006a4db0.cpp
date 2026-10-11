#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// MSVC CRT calloc, bound by address. Original pushes Size then Count,
// so the CRT sees (Count, Size).
CSF_FUNC(void*, __cdecl, _calloc, 0x007B2081, (uint32_t Count, uint32_t Size));

// Zero-initialized allocation wrapper used by the zlib/log module.
static void* __cdecl ZlibLog_AllocZeroed(int /*unused*/, size_t Count, size_t Size)
{
    return _calloc(static_cast<uint32_t>(Count), static_cast<uint32_t>(Size));
}

HOOK(0x006A4DB0, ZlibLog_AllocZeroed, "auto");
