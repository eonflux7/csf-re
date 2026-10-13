#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));
CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));
CSF_FUNC(uint32_t*, __cdecl, Rt2dFontGet, 0x007C6040, (int a1));
CSF_FUNC(int, __cdecl, sub_7C6350, 0x007C6350, (uint32_t* a1));
CSF_FUNC(int, __cdecl, sub_7C67D0, 0x007C67D0, (int a1, int a2, int a3, int a4));

CSF_GLOBAL(const char*, g_psz_vfs, 0x008A6B9C);
CSF_GLOBAL(const char*, g_psz_vfs_8A6BA0, 0x008A6BA0);

static int __fastcall C_0085ec18_RegisterVirtualFileSystem(
    C_0085ec18* self, void* /*edx*/, int fileCount, int /*unused*/, int /*unused*/)
{
    if (Rt2dFontGet(reinterpret_cast<int>(g_psz_vfs)) != nullptr)
        return 0;

    void* fileSystem = reinterpret_cast<void*>(
        AllocateAlignedBuffer(0x5Cu, 64, 0, 0));
    if (fileSystem == nullptr)
        return 0;

    self->field_0004 = reinterpret_cast<uint32_t>(fileSystem);
    CSF_AT(uint32_t, fileSystem, 20) = (uint32_t)CSF_ADDR_FN(0x0069FFE0);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 24) =
        (uint32_t)CSF_ADDR_FN(0x0069FB80);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 28) =
        (uint32_t)CSF_ADDR_FN(0x006A0000);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 40) =
        (uint32_t)CSF_ADDR_FN(0x0069FDD0);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 44) =
        (uint32_t)CSF_ADDR_FN(0x0069FE80);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 48) =
        (uint32_t)CSF_ADDR_FN(0x0069FEB0);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 52) =
        (uint32_t)CSF_ADDR_FN(0x0069FF00);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 76) =
        (uint32_t)CSF_ADDR_FN(0x004206B0);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 56) =
        (uint32_t)CSF_ADDR_FN(0x0069FF40);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 68) =
        (uint32_t)CSF_ADDR_FN(0x0069FFA0);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 60) =
        (uint32_t)CSF_ADDR_FN(0x0069FFD0);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 64) =
        (uint32_t)CSF_ADDR_FN(0x0069FFC0);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 72) =
        (uint32_t)CSF_ADDR_FN(0x0069FFD0);

    const uint32_t bufferSize = 72u * static_cast<uint32_t>(fileCount);
    void* files = reinterpret_cast<void*>(
        AllocateAlignedBuffer(bufferSize, 64, 0, 0));
    CSF_AT(void*, fileSystem, 88) = files;
    if (files == nullptr)
        return 0;

    std::memset(files, 0, bufferSize);
    uint32_t offset = 0;
    for (int remaining = fileCount; remaining > 0; --remaining)
    {
        void* file = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t>(CSF_AT(void*, fileSystem, 88)) + offset);
        offset += 72;
        CSF_AT(uint32_t, file, 56) = self->field_0004;
        CSF_AT(uint32_t, file, 32) = 1;
    }

    if (sub_7C67D0(static_cast<int>(self->field_0004), fileCount,
                  reinterpret_cast<int>(g_psz_vfs),
                  reinterpret_cast<int>(g_psz_vfs_8A6BA0)) != 1)
    {
        FreeBlock_690F70(CSF_AT(void*, fileSystem, 88));
        FreeBlock_690F70(fileSystem);
        return 0;
    }

    const int registered =
        sub_7C6350(reinterpret_cast<uint32_t*>(self->field_0004)) != 0;
    if (!registered)
    {
        FreeBlock_690F70(CSF_AT(void*, fileSystem, 88));
        FreeBlock_690F70(fileSystem);
    }
    return registered;
}

HOOK(0x006A0020, C_0085ec18_RegisterVirtualFileSystem, "auto");
