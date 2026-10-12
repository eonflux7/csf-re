#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4482C0, 0x004482C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_558840, 0x00558840, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, sub_558DA0, 0x00558DA0, (int a1, int a2));
CSF_FUNC(int*, __cdecl, sub_558E50, 0x00558E50, (int* a1, int* a2, int* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __cdecl, sub_558EE0, 0x00558EE0, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));
CSF_FUNC(uint32_t*, __cdecl, sub_559010, 0x00559010, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, sub_559030, 0x00559030, (void* self, void* edx, int a1, int a2));
CSF_FUNC(int*, __fastcall, sub_559050, 0x00559050, (void* self, void* edx, int* a1, int* a2, int* a3));
CSF_FUNC(int, __fastcall, sub_559070, 0x00559070, (void* self, void* edx));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30, (void* block, int count));

static int __fastcall S_004482c0_AssignRuleVector(
    S_004482c0* self, void* /*edx*/, S_004482c0* source)
{
    if (self == source)
        return reinterpret_cast<int>(self);

    auto* sourceBegin = reinterpret_cast<uint32_t*>(source->field_0004);
    uint32_t sourceCount = 0;
    if (sourceBegin)
        sourceCount = static_cast<int32_t>(
            source->field_0008 - reinterpret_cast<uint32_t>(sourceBegin)) / 20;

    if (!sourceBegin || sourceCount == 0)
    {
        sub_559070(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    auto* destinationBegin = reinterpret_cast<uint32_t*>(self->field_0004);
    uint32_t destinationCount = 0;
    if (destinationBegin)
        destinationCount = static_cast<int32_t>(
            self->field_0008 - reinterpret_cast<uint32_t>(destinationBegin)) / 20;

    if (sourceCount <= destinationCount)
    {
        auto* copiedEnd = sub_558EE0(
            sourceBegin,
            reinterpret_cast<uint32_t*>(source->field_0008),
            destinationBegin,
            reinterpret_cast<int>(source));

        sub_558DA0(reinterpret_cast<int>(copiedEnd),
                   static_cast<int>(self->field_0008));

        const uint32_t currentSourceBegin = source->field_0004;
        uint32_t currentSourceCount = 0;
        if (currentSourceBegin)
            currentSourceCount = static_cast<int32_t>(
                source->field_0008 - currentSourceBegin) / 20;

        self->field_0008 = self->field_0004 + 20 * currentSourceCount;
        return reinterpret_cast<int>(self);
    }

    uint32_t destinationCapacity = 0;
    if (destinationBegin)
        destinationCapacity = static_cast<int32_t>(
            self->field_000c - reinterpret_cast<uint32_t>(destinationBegin)) / 20;

    if (sourceCount <= destinationCapacity)
    {
        const int existingCount = sub_4482C0(self, nullptr);
        auto* sourceSplit = reinterpret_cast<int*>(
            reinterpret_cast<uint32_t>(sourceBegin) +
            20 * static_cast<uint32_t>(existingCount));

        sub_559010(sourceBegin,
                   reinterpret_cast<uint32_t*>(sourceSplit),
                   destinationBegin);

        self->field_0008 = reinterpret_cast<uint32_t>(sub_558E50(
            sourceSplit,
            reinterpret_cast<int*>(source->field_0008),
            reinterpret_cast<int*>(self->field_0008),
            reinterpret_cast<int>(self),
            reinterpret_cast<int>(source)));
        return reinterpret_cast<int>(self);
    }

    if (destinationBegin)
    {
        sub_559030(self, nullptr,
                   reinterpret_cast<int>(destinationBegin),
                   static_cast<int>(self->field_0008));

        const uint32_t storageBegin = self->field_0004;
        const int capacity = static_cast<int32_t>(
            self->field_000c - storageBegin) / 20;
        ReleaseVectorStorage(reinterpret_cast<void*>(storageBegin), capacity);
    }

    const int requiredCount = sub_4482C0(source, nullptr);
    if (sub_558840(self, nullptr, requiredCount))
    {
        self->field_0008 = reinterpret_cast<uint32_t>(sub_559050(
            self, nullptr,
            reinterpret_cast<int*>(source->field_0004),
            reinterpret_cast<int*>(source->field_0008),
            reinterpret_cast<int*>(self->field_0004)));
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x00559520, S_004482c0_AssignRuleVector, "auto");
