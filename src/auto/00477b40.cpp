#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));

static bool WeaponFirearms_BeginArchiveField(S_00691860* archive)
{
    const bool active = archive->pad_0000[0] == 0;
    archive[1].field_0004 = 0;
    if (active)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
    }
    return active;
}

static int __fastcall WeaponFirearms_Serialize(void* self, void* /*edx*/,
                                              S_00691860* archive)
{
    const bool failed =
        !archive->pad_0000[0] &&
        !reinterpret_cast<bool (__fastcall*)(void*, void*, int, S_00691860*)>(
            CSF_AT(void**, reinterpret_cast<void*>(archive->field_0004), 0)[128 / 4])(
                reinterpret_cast<void*>(archive->field_0004), nullptr, 0, archive);

    const uint32_t savedContext = archive[1].field_0004;
    archive->pad_0000[0] = failed;
    auto* fields = static_cast<uint8_t*>(self);

    if (WeaponFirearms_BeginArchiveField(archive))
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        archive->pad_0000[0] =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[88 / 4])(
                    backend, nullptr, self, archive) == -1;
    }

    if (WeaponFirearms_BeginArchiveField(archive))
        Archive_SerializeString(reinterpret_cast<int>(fields + 4), archive);

    if (WeaponFirearms_BeginArchiveField(archive))
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        archive->pad_0000[0] =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[56 / 4])(
                    backend, nullptr, fields + 16, archive) == -1;
    }

    if (WeaponFirearms_BeginArchiveField(archive))
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        archive->pad_0000[0] =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[88 / 4])(
                    backend, nullptr, fields + 28, archive) == -1;
    }

    if (WeaponFirearms_BeginArchiveField(archive))
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        archive->pad_0000[0] =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[68 / 4])(
                    backend, nullptr, fields + 32, archive) == -1;
    }

    if (WeaponFirearms_BeginArchiveField(archive))
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        archive->pad_0000[0] =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[68 / 4])(
                    backend, nullptr, fields + 33, archive) == -1;
    }

    if (WeaponFirearms_BeginArchiveField(archive))
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        archive->pad_0000[0] =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[68 / 4])(
                    backend, nullptr, fields + 35, archive) == -1;
    }

    if (WeaponFirearms_BeginArchiveField(archive))
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        archive->pad_0000[0] =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[68 / 4])(
                    backend, nullptr, fields + 34, archive) == -1;
    }

    if (WeaponFirearms_BeginArchiveField(archive))
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        archive->pad_0000[0] =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[68 / 4])(
                    backend, nullptr, fields + 36, archive) == -1;
    }

    if (WeaponFirearms_BeginArchiveField(archive))
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        archive->pad_0000[0] =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, backend, 0)[68 / 4])(
                    backend, nullptr, fields + 37, archive) == -1;
    }

    archive[1].field_0004 = savedContext;
    if (archive->pad_0000[0])
    {
        archive->pad_0000[0] = 1;
        return 1;
    }

    void* backend = reinterpret_cast<void*>(archive->field_0004);
    const int result =
        reinterpret_cast<int (__fastcall*)(void*, void*, S_00691860*)>(
            CSF_AT(void**, backend, 0)[124 / 4])(backend, nullptr, archive);

    if (static_cast<uint8_t>(result) == 0)
    {
        archive->pad_0000[0] = 1;
        return 1;
    }

    archive->pad_0000[0] = 0;
    return result;
}

HOOK(0x00477B40, WeaponFirearms_Serialize, "auto");
