#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeMemberCollection, 0x0044EA10, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeByteMember, 0x0044EB60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Archive_SerializeMemberPair, 0x0044EBF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Archive_SerializeOptionalMemberA, 0x0044EE30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Archive_SerializeOptionalMemberB, 0x0044EEC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Archive_SerializeOptionalMemberC, 0x0044EF50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ProcessContextStages, 0x0044EFE0, (void* self, void* , int contextAddress));
CSF_FUNC(int, __fastcall, Archive_SerializeOptionalCollectionA, 0x0044F0B0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeOptionalCollectionB, 0x0044F150, (void* self, void* edx, void* a2));
CSF_FUNC(int, __stdcall, Archive_SerializeEmbeddedState, 0x004912B0, (int a1, int a2));
CSF_FUNC(unsigned int, __stdcall, Archive_SerializeMemberFlags, 0x00491340, (uint32_t* a1, char* a2));
CSF_FUNC(int, __stdcall, Archive_SerializeOptionalStateA, 0x004914F0, (int* a1, int a2));
CSF_FUNC(int, __stdcall, Archive_SerializeOptionalStateB, 0x004916D0, (int* a1, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static bool ArchiveCanSerializeMember(void* context)
{
    const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    return ready;
}

static void SetArchiveMemberFlags(void* context, uint8_t reference)
{
    CSF_AT(uint8_t, context, 0x15) = reference;
    CSF_AT(uint8_t, context, 0x16) = 0;
}

static bool PrepareArchiveMember(void* context, uint8_t reference = 0)
{
    if (!ArchiveCanSerializeMember(context))
        return false;
    SetArchiveMemberFlags(context, reference);
    return true;
}

// The virtual slot is a compile-time constant, never a runtime dispatch index.
template<std::size_t Slot, typename T>
static void SerializeArchiveField(void* context, T* field)
{
    if (!ArchiveCanSerializeMember(context))
        return;

    void* serializer = CSF_AT(void*, context, 0x04);
    SetArchiveMemberFlags(context, 0);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, T*, int)>(
            CSF_AT(void**, serializer, 0x00)[Slot])(
                serializer, nullptr, field, reinterpret_cast<int>(context));
    CSF_AT(uint8_t, context, 0x00) = result == -1;
}

static int __fastcall C_00833908_Serialize(
    C_00833908* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);

    bool failedToBegin = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failedToBegin =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, serializer, 0x00)[0x80 / 4])(
                    serializer, nullptr, 0, contextAddress);
    }

    const uint32_t savedMemberContext = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failedToBegin;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failedToBegin)
    {
        SetArchiveMemberFlags(context, 0);
        Archive_SerializeString(reinterpret_cast<int>(&self->field_001c), context);
    }

    SerializeArchiveField<0x58 / 4>(context, &self->field_0050);
    SerializeArchiveField<0x58 / 4>(context, &self->field_0054);
    SerializeArchiveField<0x44 / 4>(context, &self->field_0174);
    SerializeArchiveField<0x20 / 4>(context, &self->field_0094);

    if (PrepareArchiveMember(context))
        Archive_SerializeEmbeddedState(
            reinterpret_cast<int>(&self->field_0058), contextAddress);

    SerializeArchiveField<0x58 / 4>(context, &self->field_0078);
    SerializeArchiveField<0x58 / 4>(context, &self->field_007c);
    SerializeArchiveField<0x58 / 4>(context, &self->field_0080);
    SerializeArchiveField<0x20 / 4>(context, &self->field_0094);

    if (PrepareArchiveMember(context))
        Archive_SerializeMemberFlags(
            &self->field_00ac, reinterpret_cast<char*>(context));

    SerializeArchiveField<0x44 / 4>(context, &self->field_0175);
    SerializeArchiveField<0x54 / 4>(context, &self->field_00bc);
    SerializeArchiveField<0x38 / 4>(context, &self->field_0084);
    SerializeArchiveField<0x48 / 4>(context, &self->field_0090);
    SerializeArchiveField<0x20 / 4>(context, &self->field_0098);
    SerializeArchiveField<0x20 / 4>(context, &self->field_0170);

    if (PrepareArchiveMember(context, 1))
        Archive_SerializeObjectRef(&self->field_003c, reinterpret_cast<bool*>(context));
    if (PrepareArchiveMember(context, 1))
        Archive_SerializeObjectRef(&self->field_0040, reinterpret_cast<bool*>(context));
    if (PrepareArchiveMember(context, 1))
        Archive_SerializeObjectRef(&self->field_0038, reinterpret_cast<bool*>(context));
    if (PrepareArchiveMember(context, 1))
        Archive_SerializeObjectRef(&self->field_0048, reinterpret_cast<bool*>(context));
    if (PrepareArchiveMember(context, 1))
        Archive_SerializeObjectRef(&self->field_0034, reinterpret_cast<bool*>(context));
    if (PrepareArchiveMember(context, 1))
        SceneLoader_SerializeReference(&self->field_016c, reinterpret_cast<bool*>(context));
    if (PrepareArchiveMember(context, 1))
        Archive_SerializeObjectRef(&self->field_0030, reinterpret_cast<bool*>(context));
    if (PrepareArchiveMember(context, 1))
        Archive_SerializeObjectRef(&self->field_004c, reinterpret_cast<bool*>(context));
    if (PrepareArchiveMember(context, 1))
        Archive_SerializeObjectRef(&self->field_0044, reinterpret_cast<bool*>(context));

    if (self->field_0175)
    {
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x00))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeOptionalStateA(
                    reinterpret_cast<int*>(&self->field_00c0), contextAddress);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x1D))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeOptionalStateB(
                    reinterpret_cast<int*>(&self->field_00d0), contextAddress);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x07))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeOptionalMemberA(
                    &self->field_00e0, nullptr, contextAddress);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x08))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeOptionalMemberB(
                    &self->field_00e4, nullptr, contextAddress);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x0C))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeOptionalMemberC(
                    &self->field_00e8, nullptr, contextAddress);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x0D))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeMemberCollection(
                    &self->field_00f4, nullptr, context);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x0E))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeByteMember(
                    &self->field_0118, nullptr, contextAddress);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x11))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeMemberPair(
                    &self->field_011c, nullptr, contextAddress);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x12))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeMemberPair(
                    &self->field_0124, nullptr, contextAddress);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x16))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeOptionalCollectionA(
                    &self->field_012c, nullptr, context);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x17))
        {
            if (PrepareArchiveMember(context))
                Archive_SerializeOptionalCollectionB(
                    &self->field_013c, nullptr, context);
        }
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x1B))
        {
            if (PrepareArchiveMember(context))
                ProcessContextStages(&self->field_0148, nullptr, contextAddress);
        }
    }

    CSF_AT(uint32_t, context, 0x1C) = savedMemberContext;
    if (CSF_AT(uint8_t, context, 0x00) != 0)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, context, 0x04);
    const bool completed =
        reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, serializer, 0x00)[0x7C / 4])(
                serializer, nullptr, contextAddress);
    if (!completed)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0x00) = 0;
    return completed;
}

HOOK(0x00491950, C_00833908_Serialize, "auto");
