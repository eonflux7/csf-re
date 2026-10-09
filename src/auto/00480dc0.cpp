#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// memcpy as bound by the original call site (cdecl, 3 stack args).
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Copies the dword-aligned block [src, endAddr) so that it ends at baseAddr,
// i.e. destination = baseAddr - byteCount, and returns memcpy's result (the destination).
static void* __cdecl CharActionFsm_CopyDwordBlockBelowBase(void* src, int endAddr, int baseAddr)
{
    // Signed arithmetic shift, as in the original (sar).
    const int32_t byteCount = ((static_cast<int32_t>(endAddr) - static_cast<int32_t>(reinterpret_cast<std::uintptr_t>(src))) >> 2) << 2;
    void* destination = reinterpret_cast<void*>(static_cast<std::uintptr_t>(static_cast<int32_t>(baseAddr) - byteCount));
    return _memcpy(destination, src, static_cast<std::size_t>(byteCount));
}

HOOK(0x480DC0, CharActionFsm_CopyDwordBlockBelowBase, "auto");
