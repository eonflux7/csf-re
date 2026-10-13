#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));

// Walks the current texture dictionary through sub_707E50 and copies the 0x48-byte
// accumulator it fills into the caller-provided buffer. Returns that buffer.
static void* __stdcall CollectCurrentTexDictionaryInfo(void* outBuffer)
{
    alignas(8) uint8_t accumulator[0x48];
    std::memset(accumulator, 0, sizeof(accumulator));

    int currentDictionary = RwTexDictionaryGetCurrent();
    RwTexDictionaryForAllTextures(
        currentDictionary,
        reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x00707E50)),
        reinterpret_cast<int>(accumulator));

    std::memcpy(outBuffer, accumulator, 0x48);
    return outBuffer;
}

HOOK(0x708040, CollectCurrentTexDictionaryInfo, "auto");
