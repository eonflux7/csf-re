#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GfxTextureLoader_AllocateRecordStorage, 0x00410B80, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MissionScript_GetEntryCount_46BEA0, 0x0046BEA0, (S_0046bea0* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30, (void* block, int count));
CSF_FUNC(char*, __cdecl, GfxTextureLoader_CopyRecordRange, 0x006DE0D0, (char* a1, char* a2, char* a3));
CSF_FUNC(char*, __cdecl, GfxTextureLoader_ConstructRecordTail, 0x006DE1A0, (char* a1, char* a2, char* a3, int a4, int a5));
CSF_FUNC(char*, __fastcall, GfxTextureLoader_ConstructRecordRange, 0x006DE370, (void* self, void* edx, char* a1, char* a2, char* a3));

static int __fastcall S_0046bea0_AssignTextureRecords(
    S_0046bea0* self, void* /*edx*/, int sourceAddress)
{
    auto* source = reinterpret_cast<S_0046bea0*>(sourceAddress);
    if (self == source)
        return reinterpret_cast<int>(self);

    const uint32_t sourceBegin = source->field_0004;
    const uint32_t sourceCount = sourceBegin
        ? static_cast<uint32_t>(
            static_cast<int32_t>(source->field_0008 - sourceBegin) / 28)
        : 0;

    if (!sourceCount)
    {
        if (self->field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
        self->field_0008 = 0;
        self->field_000c = 0;
        return reinterpret_cast<int>(self);
    }

    const uint32_t destinationBegin = self->field_0004;
    const uint32_t destinationCount = destinationBegin
        ? static_cast<uint32_t>(
            static_cast<int32_t>(self->field_0008 - destinationBegin) / 28)
        : 0;

    if (sourceCount <= destinationCount)
    {
        GfxTextureLoader_CopyRecordRange(
            reinterpret_cast<char*>(source->field_0004),
            reinterpret_cast<char*>(source->field_0008),
            reinterpret_cast<char*>(destinationBegin));

        const uint32_t currentSourceBegin = source->field_0004;
        const int32_t currentSourceCount = currentSourceBegin
            ? static_cast<int32_t>(source->field_0008 - currentSourceBegin) / 28
            : 0;
        self->field_0008 = self->field_0004
            + 28 * static_cast<uint32_t>(currentSourceCount);
        return reinterpret_cast<int>(self);
    }

    const uint32_t destinationCapacity = destinationBegin
        ? static_cast<uint32_t>(
            static_cast<int32_t>(self->field_000c - destinationBegin) / 28)
        : 0;

    if (sourceCount <= destinationCapacity)
    {
        const int destinationEntryCount =
            MissionScript_GetEntryCount_46BEA0(self, nullptr);
        auto* sourceSplit = reinterpret_cast<char*>(
            source->field_0004
            + 28 * static_cast<uint32_t>(destinationEntryCount));

        GfxTextureLoader_CopyRecordRange(
            reinterpret_cast<char*>(source->field_0004),
            sourceSplit,
            reinterpret_cast<char*>(destinationBegin));

        self->field_0008 = reinterpret_cast<uint32_t>(
            GfxTextureLoader_ConstructRecordTail(
                sourceSplit,
                reinterpret_cast<char*>(source->field_0008),
                reinterpret_cast<char*>(self->field_0008),
                reinterpret_cast<int>(self),
                sourceAddress));
        return reinterpret_cast<int>(self);
    }

    if (destinationBegin)
    {
        ReleaseVectorStorage(
            reinterpret_cast<void*>(destinationBegin),
            static_cast<int32_t>(self->field_000c - destinationBegin) / 28);
    }

    const int entryCount = MissionScript_GetEntryCount_46BEA0(source, nullptr);
    if (GfxTextureLoader_AllocateRecordStorage(self, nullptr, entryCount))
    {
        self->field_0008 = reinterpret_cast<uint32_t>(
            GfxTextureLoader_ConstructRecordRange(
                self, nullptr,
                reinterpret_cast<char*>(source->field_0004),
                reinterpret_cast<char*>(source->field_0008),
                reinterpret_cast<char*>(self->field_0004)));
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x006DE9B0, S_0046bea0_AssignTextureRecords, "auto");
