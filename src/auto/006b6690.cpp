#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __stdcall, GfxSkin_ReleaseBufferElements, 0x006B4DC0, (int a1, int a2));
CSF_FUNC(int, __stdcall, S_006b59f0_ProcessSecondaryFields, 0x006B5610, (void* a1, int a2));
CSF_FUNC(int, __stdcall, S_006b59f0_ProcessBufferFields, 0x006B5790, (int* a1, int a2));
CSF_FUNC(char, __cdecl, S_006b59f0_ReleasePrimaryElements, 0x006B5D30, (int* a1));
CSF_FUNC(int, __stdcall, S_006b59f0_ProcessPrimaryFields, 0x006B5F90, (void* a1, int a2));

static int __fastcall S_006b59f0_ProcessArchive(
    S_006b59f0* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    auto* bytes = reinterpret_cast<std::uint8_t*>(self);
    const std::uint32_t savedContext = CSF_AT(std::uint32_t, archive, 0x1C);

    bool stopped = false;
    if (CSF_AT(std::uint8_t, archive, 0) == 0)
    {
        void* handler = CSF_AT(void*, archive, 0x04);
        stopped = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, handler, 0)[0x80 / 4])(
                handler, nullptr, 0, archiveAddress);
    }

    CSF_AT(std::uint8_t, archive, 0) = stopped;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (!stopped)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        S_006b59f0_ProcessPrimaryFields(self, archiveAddress);
    }

    bool processNext = CSF_AT(std::uint8_t, archive, 0) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (processNext)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        S_006b59f0_ProcessSecondaryFields(bytes + 0x10, archiveAddress);
    }

    processNext = CSF_AT(std::uint8_t, archive, 0) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (processNext)
    {
        void* handler = CSF_AT(void*, archive, 0x04);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        CSF_AT(std::uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, handler, 0)[0x44 / 4])(
                    handler, nullptr, bytes + 0x20, archiveAddress) == -1;
    }

    processNext = CSF_AT(std::uint8_t, archive, 0) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (processNext)
    {
        void* handler = CSF_AT(void*, archive, 0x04);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        CSF_AT(std::uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, handler, 0)[0x38 / 4])(
                    handler, nullptr, bytes + 0x24, archiveAddress) == -1;
    }

    processNext = CSF_AT(std::uint8_t, archive, 0) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (processNext)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        S_006b59f0_ProcessBufferFields(
            reinterpret_cast<int*>(bytes + 0x30), archiveAddress);
    }

    if (CSF_AT(std::uint8_t, archive, 0x14) != 0)
    {
        S_006b59f0_ReleasePrimaryElements(reinterpret_cast<int*>(self));
        if (CSF_AT(void*, self, 0x14))
            Mem_FreeDebug(CSF_AT(void*, self, 0x14));

        CSF_AT(std::uint32_t, self, 0x14) = 0;
        CSF_AT(std::uint32_t, self, 0x18) = 0;
        CSF_AT(std::uint32_t, self, 0x1C) = 0;

        auto* buffer = reinterpret_cast<S_006b59f0*>(bytes + 0x30);
        if (buffer->field_0004)
        {
            GfxSkin_ReleaseBufferElements(
                static_cast<int>(buffer->field_0004),
                static_cast<int>(buffer->field_0008));
            if (buffer->field_0004)
                Mem_FreeDebug(reinterpret_cast<void*>(buffer->field_0004));
        }
        buffer->field_0004 = 0;
        buffer->field_0008 = 0;
        buffer->field_000c = 0;
    }

    CSF_AT(std::uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(std::uint8_t, archive, 0) == 0)
    {
        void* handler = CSF_AT(void*, archive, 0x04);
        const bool result = reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, handler, 0)[0x7C / 4])(
                handler, nullptr, archiveAddress);
        if (result)
        {
            CSF_AT(std::uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x006B6690, S_006b59f0_ProcessArchive, "auto");
