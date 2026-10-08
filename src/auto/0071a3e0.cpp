#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// int memcpy(void* dst, const void* src, size_t size)  -- the CRT memcpy at 0x7AEE60
CSF_FUNC(void*, __cdecl, _memcpy, 0x7AEE60, (void* dst, const void* src, size_t size));

// Copies the 4-byte-aligned range [src, end) into dst and returns the byte just past
// the destination range (dst + copied bytes), mirroring the original memcpy helper.
static int __cdecl CopyWordsAndAdvance(void* src, int end, void* dst)
{
    const int wordCount = (end - reinterpret_cast<int>(src)) >> 2; // signed arithmetic shift
    const int byteCount = wordCount * 4;

    void* result = _memcpy(dst, src, static_cast<size_t>(byteCount));
    return reinterpret_cast<int>(result) + byteCount;
}

HOOK(0x71A3E0, CopyWordsAndAdvance, "auto");
