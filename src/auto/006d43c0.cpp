#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));
CSF_FUNC(int, __cdecl, GfxFrames_ForEachClumpAtomicWithPairB, 0x006C0320, (int clump, int pairA, int pairB));

// Data of the "Textures\\" suffix (9 chars + NUL, copied as dword, dword, word).
CSF_GLOBAL(uint32_t, g_TexturesSuffixPart0, 0x0086105C);
CSF_GLOBAL(uint32_t, g_TexturesSuffixPart1, 0x00861060);
CSF_GLOBAL(uint16_t, g_TexturesSuffixPart2, 0x00861064);

// Locals the original keeps at [ebp-0xC]: cursor pointer, result count, flag byte.
struct ClumpMetadataScanState {
    char* cursor;        // [ebp-0xC]
    int resultCount;     // [ebp-0x8]
    char flag;           // [ebp-0x4]
};

static int __stdcall ClumpLoader_BuildClumpMetadata(int a1, int a2, char a3)
{
    uint8_t* resource = reinterpret_cast<uint8_t*>(a1);

    // Short names live inline at +4; longer ones are a pointer stored at +4.
    char* resourcePath;
    if (CSF_AT(uint32_t, resource, 0x18) < 0x10u)
        resourcePath = reinterpret_cast<char*>(resource + 4);
    else
        resourcePath = reinterpret_cast<char*>(CSF_AT(uint32_t, resource, 4));

    alignas(8) uint8_t pathBuffer[0x200];
    Hud_ExtractResourcePath(resourcePath, pathBuffer, 512);

    // Find the terminator of the extracted path.
    char* writePos = reinterpret_cast<char*>(pathBuffer);
    while (*writePos)
        ++writePos;

    // Append "Textures\\" (10 bytes including NUL).
    *reinterpret_cast<uint32_t*>(writePos) = g_TexturesSuffixPart0;
    *reinterpret_cast<uint32_t*>(writePos + 4) = g_TexturesSuffixPart1;
    *reinterpret_cast<uint16_t*>(writePos + 8) = g_TexturesSuffixPart2;

    ClumpMetadataScanState scan;
    scan.cursor = reinterpret_cast<char*>(pathBuffer);
    scan.resultCount = 0;
    scan.flag = a3;

    GfxFrames_ForEachClumpAtomicWithPairB(
        a2,
        (int)(intptr_t)CSF_ADDR_FN(0x006D4020),
        (int)(intptr_t)&scan);

    return scan.resultCount;
}

HOOK(0x006D43C0, ClumpLoader_BuildClumpMetadata, "auto");
