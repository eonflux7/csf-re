#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00482080_SerializeBaseState, 0x00482080, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharActionFsm_ResetForLoad, 0x00486B40, (void* self, void* edx));
CSF_FUNC(int, __stdcall, CharActionFsm_SerializeEmbeddedState, 0x00489060, (void* a1, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

// The packet labels the direct base-serializer call as a vtable reference.
static const auto kReportedBaseSerializerVtable = CSF_VTBL(0x00482080);

static bool CharActionFsm_BeginArchiveField(void* archive)
{
    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    return ready;
}

static void CharActionFsm_ClearArchiveFieldFlags(void* archive)
{
    CSF_AT(uint8_t, archive, 0x15) = 0;
    CSF_AT(uint8_t, archive, 0x16) = 0;
}

static int __fastcall S_00482080_SerializeActionState(
    S_00482080* self, void* /*edx*/, int archiveAddress)
{
    auto* archive = reinterpret_cast<uint8_t*>(archiveAddress);
    auto* state = reinterpret_cast<uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, backend, 0x00)[0x80 / 4])(
                backend, nullptr, 0, archiveAddress);
    }

    const int savedContext = CSF_AT(int, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    S_00482080_SerializeBaseState(self, nullptr, archiveAddress);

    if (CSF_AT(uint8_t, archive, 0x14) == 0 &&
        CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        CharActionFsm_ResetForLoad(state - 4, nullptr);
    }

    if (CharActionFsm_BeginArchiveField(archive))
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CharActionFsm_SerializeEmbeddedState(state + 0x70, archiveAddress);
    }

    if (CharActionFsm_BeginArchiveField(archive))
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        Archive_SerializeObjectRef(state + 0x80, reinterpret_cast<bool*>(archive));
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        Archive_SerializeObjectRef(state + 0x84, reinterpret_cast<bool*>(archive));
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        Archive_SerializeObjectRef(state + 0x88, reinterpret_cast<bool*>(archive));
    }

    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x58 / 4])(
                    backend, nullptr, state + 0x8C, archiveAddress) == -1;
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x58 / 4])(
                    backend, nullptr, state + 0x90, archiveAddress) == -1;
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x58 / 4])(
                    backend, nullptr, state + 0x94, archiveAddress) == -1;
    }

    if (CharActionFsm_BeginArchiveField(archive))
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        Archive_SerializeObjectRef(state + 0x98, reinterpret_cast<bool*>(archive));
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        Archive_SerializeObjectRef(state + 0x9C, reinterpret_cast<bool*>(archive));
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        Archive_SerializeObjectRef(state + 0xA0, reinterpret_cast<bool*>(archive));
    }

    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x44 / 4])(
                    backend, nullptr, state + 0xA4, archiveAddress) == -1;
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x54 / 4])(
                    backend, nullptr, state + 0xA8, archiveAddress) == -1;
    }

    if (CharActionFsm_BeginArchiveField(archive))
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        Archive_SerializeString(reinterpret_cast<int>(state + 0xAC), archive);
    }

    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x44 / 4])(
                    backend, nullptr, state + 0xB8, archiveAddress) == -1;
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x44 / 4])(
                    backend, nullptr, state + 0xB9, archiveAddress) == -1;
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x58 / 4])(
                    backend, nullptr, state + 0xBC, archiveAddress) == -1;
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x44 / 4])(
                    backend, nullptr, state + 0xC0, archiveAddress) == -1;
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x54 / 4])(
                    backend, nullptr, state + 0xC8, archiveAddress) == -1;
    }
    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, backend, 0x00)[0x54 / 4])(
                    backend, nullptr, state + 0xCC, archiveAddress) == -1;
    }

    CSF_AT(int, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0x00) != 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    void* backend = CSF_AT(void*, archive, 0x04);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, backend, 0x00)[0x7C / 4])(
                backend, nullptr, archiveAddress);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0x00) = 0;
    return result;
}

HOOK(0x00489510, S_00482080_SerializeActionState, "auto");
