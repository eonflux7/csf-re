#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, CharActionFsm_SerializeStateList, 0x00481D00, (void* a1, int a2));
CSF_FUNC(int, __fastcall, CharActionFsm_SerializeStateData, 0x0048B5C0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static bool CharActionFsm_BeginArchiveField(void* archive)
{
    const bool ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    return ready;
}

static void CharActionFsm_ClearArchiveFieldFlags(void* archive)
{
    CSF_AT(uint8_t, archive, 0x15) = 0;
    CSF_AT(uint8_t, archive, 0x16) = 0;
}

static void CharActionFsm_SerializeWord(void* field, void* archive)
{
    void* serializer = CSF_AT(void*, archive, 4);
    CharActionFsm_ClearArchiveFieldFlags(archive);
    CSF_AT(uint8_t, archive, 0) =
        reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
            CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, field, archive) == -1;
}

static void CharActionFsm_SerializeByte(void* field, void* archive)
{
    void* serializer = CSF_AT(void*, archive, 4);
    CharActionFsm_ClearArchiveFieldFlags(archive);
    CSF_AT(uint8_t, archive, 0) =
        reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
            CSF_AT(void**, serializer, 0)[0x44 / 4])(
                serializer, nullptr, field, archive) == -1;
}

static int __fastcall S_00482080_Serialize(
    S_00482080* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    auto* fields = reinterpret_cast<uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        Archive_SerializeObjectRef(fields + 4, reinterpret_cast<bool*>(archive));
    }

    if (CharActionFsm_BeginArchiveField(archive))
        CharActionFsm_SerializeWord(fields + 8, archive);

    if (CharActionFsm_BeginArchiveField(archive))
        CharActionFsm_SerializeWord(fields + 12, archive);

    if (CSF_AT(uint8_t, archive, 0x14) == 0 &&
        CSF_AT(uint8_t, archive, 0) == 0)
    {
        for (uint32_t index = 0; ; ++index)
        {
            const uint32_t begin = self->field_0014;
            if (!begin)
                break;

            const uint32_t count = static_cast<uint32_t>(
                static_cast<int32_t>(self->field_0018 - begin) >> 2);
            if (index >= count)
                break;

            void* state = reinterpret_cast<void**>(begin)[index];
            if (state)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, state, 0)[0])(
                        state, nullptr, 1);
                reinterpret_cast<uint32_t*>(self->field_0014)[index] = 0;
            }
        }

        if (self->field_0014)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0014));

        self->field_0014 = 0;
        self->field_0018 = 0;
        self->field_001c = 0;
    }

    if (CharActionFsm_BeginArchiveField(archive))
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CharActionFsm_SerializeStateList(fields + 16, archiveAddress);
    }

    if (CharActionFsm_BeginArchiveField(archive))
        CharActionFsm_SerializeWord(fields + 32, archive);

    if (CharActionFsm_BeginArchiveField(archive))
        CharActionFsm_SerializeWord(fields + 36, archive);

    if (CharActionFsm_BeginArchiveField(archive))
        CharActionFsm_SerializeByte(fields + 40, archive);

    if (CharActionFsm_BeginArchiveField(archive))
    {
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CharActionFsm_SerializeStateData(fields + 44, nullptr, archive);
    }

    if (CharActionFsm_BeginArchiveField(archive))
        CharActionFsm_SerializeWord(fields + 96, archive);

    if (CharActionFsm_BeginArchiveField(archive))
        CharActionFsm_SerializeByte(fields + 100, archive);

    if (CharActionFsm_BeginArchiveField(archive))
        CharActionFsm_SerializeByte(fields + 101, archive);

    if (CharActionFsm_BeginArchiveField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CharActionFsm_ClearArchiveFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, fields + 104, archive) == -1;
    }

    if (CharActionFsm_BeginArchiveField(archive))
        CharActionFsm_SerializeByte(fields + 108, archive);

    if (CharActionFsm_BeginArchiveField(archive))
        CharActionFsm_SerializeByte(fields + 109, archive);

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0) != 0)
    {
        CSF_AT(uint8_t, archive, 0) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, archive, 4);
    const int result = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, serializer, 0)[0x7C / 4])(
            serializer, nullptr, archive);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, archive, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0) = 0;
    return result;
}

HOOK(0x00482080, S_00482080_Serialize, "auto");
