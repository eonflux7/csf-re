#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AllocateTeamStorage_5587E0, 0x005587E0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __cdecl, MpRules_CopyVectorRange, 0x00698F80,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30,
         (void* block, int count));
CSF_FUNC(uint32_t*, __cdecl, sub_771FF0, 0x00771FF0,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, sub_772100, 0x00772100,
         (void* self, void* edx, uint32_t* a1, uint32_t* a2, uint32_t* a3));

static int __fastcall S_00772100_Assign(
    S_00772100* self, void* /*edx*/, int sourceAddress)
{
    // The packet classifies the call at 0x77263C as a reference to this class's vtable.
    (void)CSF_VTBL(0x00772100);

    auto* source = reinterpret_cast<S_00772100*>(sourceAddress);
    if (self == source)
        return reinterpret_cast<int>(self);

    const uint32_t sourceBegin = source->field_0004;
    const uint32_t sourceCount = sourceBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(source->field_0008 - sourceBegin) >> 3)
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

    const uint32_t destBegin = self->field_0004;
    const uint32_t destCount = destBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0008 - destBegin) >> 3)
        : 0;

    if (sourceCount <= destCount)
    {
        MpRules_CopyVectorRange(
            reinterpret_cast<uint32_t*>(sourceBegin),
            reinterpret_cast<uint32_t*>(source->field_0008),
            reinterpret_cast<uint32_t*>(destBegin));

        const uint32_t currentSourceBegin = source->field_0004;
        const uint32_t currentSourceCount = currentSourceBegin
            ? static_cast<uint32_t>(
                  static_cast<int32_t>(
                      source->field_0008 - currentSourceBegin) >> 3)
            : 0;
        self->field_0008 = self->field_0004 + 8 * currentSourceCount;
        return reinterpret_cast<int>(self);
    }

    const uint32_t destCapacity = destBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_000c - destBegin) >> 3)
        : 0;

    if (sourceCount > destCapacity)
    {
        if (destBegin)
        {
            ReleaseVectorStorage(
                reinterpret_cast<void*>(destBegin),
                static_cast<int32_t>(self->field_000c - destBegin) >> 3);
        }

        const uint32_t allocSourceBegin = source->field_0004;
        const int allocCount = allocSourceBegin
            ? static_cast<int32_t>(source->field_0008 - allocSourceBegin) >> 3
            : 0;

        if (AllocateTeamStorage_5587E0(self, nullptr, allocCount))
        {
            self->field_0008 = reinterpret_cast<uint32_t>(sub_772100(
                self, nullptr,
                reinterpret_cast<uint32_t*>(source->field_0004),
                reinterpret_cast<uint32_t*>(source->field_0008),
                reinterpret_cast<uint32_t*>(self->field_0004)));
        }
        return reinterpret_cast<int>(self);
    }

    const int existingCount = destBegin
        ? static_cast<int32_t>(self->field_0008 - destBegin) >> 3
        : 0;
    const uint32_t split =
        source->field_0004 + 8 * static_cast<uint32_t>(existingCount);

    MpRules_CopyVectorRange(
        reinterpret_cast<uint32_t*>(source->field_0004),
        reinterpret_cast<uint32_t*>(split),
        reinterpret_cast<uint32_t*>(destBegin));

    self->field_0008 = reinterpret_cast<uint32_t>(sub_771FF0(
        reinterpret_cast<uint32_t*>(split),
        reinterpret_cast<uint32_t*>(source->field_0008),
        reinterpret_cast<uint32_t*>(self->field_0008),
        reinterpret_cast<int>(self),
        sourceAddress));

    return reinterpret_cast<int>(self);
}

HOOK(0x00772500, S_00772100_Assign, "auto");
