#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Archive_SerializeFloatMember, 0x004076D0, (void* self, void* edx, int a2));
CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __stdcall, SceneLoader_SerializeRecordMember, 0x00445620, (void* a1, int a2));
CSF_FUNC(int, __fastcall, Archive_SerializeMemberCollection, 0x0044EA10, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeByteMember, 0x0044EB60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833b88_Serialize, 0x004933E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeParameterBlock, 0x00499130, (void* self, void* edx, void* a2));
CSF_FUNC(int, __stdcall, Archive_SerializeInitialParameters, 0x004991E0, (int a1, int a2));
CSF_FUNC(int, __fastcall, Archive_SerializePrimaryParameters, 0x00499AA0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeSecondaryParameters, 0x00499B80, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeAdditionalParameters, 0x00499C70, (void* self, void* edx, void* a2));
CSF_FUNC(unsigned int, __fastcall, CPlayerCharacter_SetResolvedObject, 0x0049B030, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __stdcall, Archive_SerializeRecordParameters, 0x0049C240, (void* a1, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static bool Archive_IsMemberReady(void* archive)
{
    const bool ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    return ready;
}

static void Archive_SetMemberFlags(void* archive, uint8_t reference)
{
    CSF_AT(uint8_t, archive, 0x15) = reference;
    CSF_AT(uint8_t, archive, 0x16) = 0;
}

static bool Archive_BeginMember(void* archive, uint8_t reference = 0)
{
    if (!Archive_IsMemberReady(archive))
        return false;
    Archive_SetMemberFlags(archive, reference);
    return true;
}

static void Archive_SerializeByte(void* archive, uint8_t* member)
{
    if (Archive_IsMemberReady(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        Archive_SetMemberFlags(archive, 0);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, member, archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }
}

static void Archive_SerializeInteger(void* archive, uint32_t* member)
{
    if (Archive_IsMemberReady(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        Archive_SetMemberFlags(archive, 0);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, stream, 0)[0x38 / 4])(
                    stream, nullptr, member, archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }
}

static void Archive_SerializeUnsignedInteger(void* archive, uint32_t* member)
{
    if (Archive_IsMemberReady(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        Archive_SetMemberFlags(archive, 0);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, stream, 0)[0x48 / 4])(
                    stream, nullptr, member, archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }
}

static void Archive_SerializeFloat(void* archive, void* member)
{
    if (Archive_IsMemberReady(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        Archive_SetMemberFlags(archive, 0);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[0x54 / 4])(
                    stream, nullptr, member, archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }
}

static void Archive_SerializeIdentifier(void* archive, uint32_t* member)
{
    if (Archive_IsMemberReady(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        Archive_SetMemberFlags(archive, 0);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, stream, 0)[0x58 / 4])(
                    stream, nullptr, member, archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }
}

static int __fastcall CPlayerCharacter_Serialize(CPlayerCharacter* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    bool error = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        error = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, stream, 0)[0x80 / 4])(
                stream, nullptr, 0, archiveAddress);
    }

    const int savedContext = CSF_AT(int, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = error;
    C_00833b88_Serialize(self, nullptr, archiveAddress);

    if (Archive_BeginMember(archive))
        Archive_SerializeInitialParameters(
            reinterpret_cast<int>(&self->field_020c), archiveAddress);

    Archive_SerializeByte(archive, &self->field_0228);
    Archive_SerializeByte(archive, &self->field_0229);
    Archive_SerializeInteger(archive, &self->field_0230);

    if (Archive_BeginMember(archive, 1))
        SceneLoader_SerializeReference(
            &self->field_022c, reinterpret_cast<bool*>(archive));

    Archive_SerializeByte(archive, &self->field_023c);

    if (Archive_BeginMember(archive))
        Archive_SerializePrimaryParameters(&self->field_0240, nullptr, archive);
    if (Archive_BeginMember(archive))
        Archive_SerializeSecondaryParameters(&self->field_0268, nullptr, archive);
    if (Archive_BeginMember(archive))
        Archive_SerializeParameterBlock(&self->field_028c, nullptr, archive);
    if (Archive_BeginMember(archive))
        Archive_SerializeAdditionalParameters(&self->field_02b0, nullptr, archive);
    if (Archive_BeginMember(archive))
        Archive_SerializeString(reinterpret_cast<int>(&self->field_02c8), archive);
    if (Archive_BeginMember(archive))
        Archive_SerializeString(reinterpret_cast<int>(&self->field_02d4), archive);

    Archive_SerializeUnsignedInteger(archive, &self->field_02e0);
    Archive_SerializeUnsignedInteger(archive, &self->field_02e4);
    Archive_SerializeUnsignedInteger(archive, &self->field_02e8);

    if (Archive_BeginMember(archive))
        Archive_SerializeFloatMember(&self->field_02ec, nullptr, archiveAddress);
    if (Archive_BeginMember(archive))
        Archive_SerializeRecordParameters(
            &CSF_AT(uint8_t, self, 0x2FC), archiveAddress);
    if (Archive_BeginMember(archive))
        Archive_SerializeRecordParameters(
            &CSF_AT(uint8_t, self, 0x30C), archiveAddress);
    if (Archive_BeginMember(archive))
        Archive_SerializeMemberCollection(&self->field_031c, nullptr, archive);
    if (Archive_BeginMember(archive))
        Archive_SerializeByteMember(&self->field_0340, nullptr, archiveAddress);
    if (Archive_BeginMember(archive))
        Archive_SerializeString(
            reinterpret_cast<int>(&CSF_AT(uint8_t, self, 0x344)), archive);

    Archive_SerializeFloat(archive, &self->field_0390);
    Archive_SerializeByte(archive, &self->field_0394);
    Archive_SerializeIdentifier(archive, &self->field_0398);
    Archive_SerializeByte(archive, &self->field_03a5);
    Archive_SerializeInteger(archive, &self->field_03a8);
    Archive_SerializeUnsignedInteger(archive, &self->field_03cc);
    Archive_SerializeByte(archive, &self->field_03d0);
    Archive_SerializeFloat(archive, &self->field_03d4);
    Archive_SerializeFloat(archive, &self->field_03d8);
    Archive_SerializeByte(archive, &self->field_03dc);
    Archive_SerializeByte(archive, &self->field_03dd);

    if (Archive_BeginMember(archive))
        Archive_SerializeString(reinterpret_cast<int>(&self->field_03b4), archive);
    if (Archive_BeginMember(archive))
        Archive_SerializeString(reinterpret_cast<int>(&self->field_03c0), archive);

    Archive_SerializeByte(archive, &self->field_03de);
    Archive_SerializeFloat(archive, &self->field_03e0);
    Archive_SerializeIdentifier(archive, &self->field_03ec);

    if (Archive_BeginMember(archive))
        SceneLoader_SerializeRecordMember(&self->field_03f0, archiveAddress);

    Archive_SerializeUnsignedInteger(archive, &self->field_0400);
    Archive_SerializeByte(archive, &self->field_0404);
    Archive_SerializeFloat(archive, &self->field_0408);
    Archive_SerializeFloat(archive, &self->field_040c);
    Archive_SerializeInteger(archive, &self->field_0410);

    const uint8_t previousError = CSF_AT(uint8_t, archive, 0);
    uint8_t hasResolvedObject = self->field_039c != 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (!previousError)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        Archive_SetMemberFlags(archive, 0);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, &hasResolvedObject, archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    Archive_SerializeUnsignedInteger(archive, &self->field_03a0);

    if (!CSF_AT(uint8_t, archive, 0x14) &&
        !CSF_AT(uint8_t, archive, 0) &&
        hasResolvedObject)
    {
        void* manager = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 8136);
        const int index =
            reinterpret_cast<int(__stdcall*)(uint32_t)>(
                CSF_AT(void**, manager, 0)[0x10 / 4])(self->field_03a0);
        if (index != -1)
        {
            const int object =
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, manager, 0)[0x08 / 4])(
                        manager, nullptr, index);
            CPlayerCharacter_SetResolvedObject(self, nullptr, object);
        }
    }

    if (Archive_BeginMember(archive, 1))
        SceneLoader_SerializeReference(
            &self->field_039c, reinterpret_cast<bool*>(archive));

    const bool ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(int, archive, 0x1C) = savedContext;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(
                    stream, nullptr, archiveAddress);
        if (static_cast<uint8_t>(result))
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x0049C850, CPlayerCharacter_Serialize, "auto");
