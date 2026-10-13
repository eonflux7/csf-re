#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));
CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));
CSF_FUNC(uint32_t*, __cdecl, Rt2dFontGet, 0x007C6040, (int a1));
CSF_FUNC(int, __cdecl, Rtfs_RegisterFileSystem, 0x007C6350, (uint32_t* a1));
CSF_FUNC(int, __cdecl, Rtfs_InitializeFileSystem, 0x007C67D0, (int a1, int a2, int a3, int a4));

CSF_GLOBAL(const char*, g_psz_asyn, 0x008A6B94);
CSF_GLOBAL(const char*, g_psz_asyn_8A6B98, 0x008A6B98);
CSF_GLOBAL(int, g_008bb400, 0x008BB400);

static bool __stdcall CoreFilesystemRtfs_CreateAsyncFileSystem(
    int entryCount, int /*unused2*/, int /*unused3*/)
{
    if (Rt2dFontGet(reinterpret_cast<int>(g_psz_asyn)) != nullptr)
        return false;

    const int allocation = AllocateAlignedBuffer(0x5Cu, 64, 0, 0);
    if (!allocation)
        return false;

    void* fileSystem = reinterpret_cast<void*>(allocation);
    g_008bb400 = allocation;

    CSF_AT(void*, fileSystem, 0x14) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FB40));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x18) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FB80));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x1C) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x006A0000));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x28) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FA20));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x2C) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FA40));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x30) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FA50));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x34) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FA80));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x4C) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FA90));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x38) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FAA0));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x44) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FAE0));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x3C) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FAF0));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x40) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FB20));
    CSF_AT(void*, reinterpret_cast<void*>(g_008bb400), 0x48) =
        reinterpret_cast<void*>(CSF_ADDR_FN(0x0069FB30));

    const uint32_t bufferSize = static_cast<uint32_t>(entryCount) << 6;
    void* entries = reinterpret_cast<void*>(
        AllocateAlignedBuffer(bufferSize, 64, 0, 0));
    CSF_AT(void*, fileSystem, 0x58) = entries;
    if (!entries)
        return false;

    std::memset(entries, 0, bufferSize);
    uint32_t offset = 0;
    for (int remaining = entryCount; remaining > 0; --remaining)
    {
        auto* entry = CSF_AT(uint8_t*, fileSystem, 0x58) + offset;
        CSF_AT(int, entry, 0x38) = g_008bb400;
        offset += 64;
    }

    if (Rtfs_InitializeFileSystem(
            g_008bb400, entryCount,
            reinterpret_cast<int>(g_psz_asyn),
            reinterpret_cast<int>(g_psz_asyn_8A6B98)) != 1)
    {
        FreeBlock_690F70(CSF_AT(void*, fileSystem, 0x58));
        FreeBlock_690F70(fileSystem);
        return false;
    }

    const bool registered = Rtfs_RegisterFileSystem(
        reinterpret_cast<uint32_t*>(g_008bb400)) != 0;
    if (!registered)
    {
        FreeBlock_690F70(CSF_AT(void*, fileSystem, 0x58));
        FreeBlock_690F70(fileSystem);
    }
    return registered;
}

HOOK(0x0069FC00, CoreFilesystemRtfs_CreateAsyncFileSystem, "auto");
