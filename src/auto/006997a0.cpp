#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AllocateTeamStorage_5587E0, 0x005587E0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __cdecl, MpRules_CopyVectorRange, 0x00698F80,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, VectorRange_CopyTail, 0x00698FB0,
         (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __fastcall, S_00699370_ConstructRange, 0x00699370,
         (void* self, void* edx, int a1, int a2, int a3));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30,
         (void* block, int count));

static int __fastcall S_00699370_AssignFrom(
    S_00699370* self, void* /*edx*/, int sourceAddress)
{
    // The packet classifies the 0x699370 call site as a vtable reference.
    (void)CSF_VTBL(0x00699370);

    auto* source = reinterpret_cast<S_00699370*>(sourceAddress);
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

    const uint32_t destinationBegin = self->field_0004;
    const uint32_t destinationCount = destinationBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0008 - destinationBegin) >> 3)
        : 0;

    if (sourceCount <= destinationCount)
    {
        MpRules_CopyVectorRange(
            reinterpret_cast<uint32_t*>(sourceBegin),
            reinterpret_cast<uint32_t*>(source->field_0008),
            reinterpret_cast<uint32_t*>(self->field_0004));

        const uint32_t currentSourceBegin = source->field_0004;
        const uint32_t currentSourceCount = currentSourceBegin
            ? static_cast<uint32_t>(
                  static_cast<int32_t>(
                      source->field_0008 - currentSourceBegin) >> 3)
            : 0;
        self->field_0008 = self->field_0004 + 8 * currentSourceCount;
        return reinterpret_cast<int>(self);
    }

    const uint32_t capacity = destinationBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_000c - destinationBegin) >> 3)
        : 0;

    if (sourceCount > capacity)
    {
        if (destinationBegin)
        {
            ReleaseVectorStorage(
                reinterpret_cast<void*>(destinationBegin),
                static_cast<int32_t>(self->field_000c - destinationBegin) >> 3);
        }

        const uint32_t currentSourceBegin = source->field_0004;
        const int count = currentSourceBegin
            ? static_cast<int32_t>(
                  source->field_0008 - currentSourceBegin) >> 3
            : 0;

        if (AllocateTeamStorage_5587E0(self, nullptr, count))
        {
            self->field_0008 = static_cast<uint32_t>(S_00699370_ConstructRange(
                self, nullptr,
                static_cast<int>(source->field_0004),
                static_cast<int>(source->field_0008),
                static_cast<int>(self->field_0004)));
        }
        return reinterpret_cast<int>(self);
    }

    const int existingCount = destinationBegin
        ? static_cast<int32_t>(self->field_0008 - destinationBegin) >> 3
        : 0;
    const uint32_t tailStart =
        sourceBegin + 8 * static_cast<uint32_t>(existingCount);

    MpRules_CopyVectorRange(
        reinterpret_cast<uint32_t*>(sourceBegin),
        reinterpret_cast<uint32_t*>(tailStart),
        reinterpret_cast<uint32_t*>(self->field_0004));

    self->field_0008 = static_cast<uint32_t>(VectorRange_CopyTail(
        static_cast<int>(tailStart),
        static_cast<int>(source->field_0008),
        static_cast<int>(self->field_0008),
        reinterpret_cast<int>(self),
        sourceAddress));

    return reinterpret_cast<int>(self);
}

HOOK(0x006997A0, S_00699370_AssignFrom, "auto");
