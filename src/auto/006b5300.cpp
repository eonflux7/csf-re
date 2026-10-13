#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30,
         (void* block, int count));
CSF_FUNC(int, __cdecl, CopyWordsAndAdvance, 0x0071A3E0,
         (void* src, int end, void* dst));
CSF_FUNC(char, __fastcall, AllocateVectorStorage, 0x00730B40,
         (void* self, void* edx, int count));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));
CSF_FUNC(int, __fastcall, Forward3ArgsWithContextTo4022E0, 0x00403A70,
         (void* self, void* , int a1, int a2, int a3));

// Vtable of S_00403a70 referenced by the original's call sites.
[[maybe_unused]] static const auto kObjectVtable = CSF_VTBL(0x00403A70);

// Number of 4-byte words between begin and end (signed shift, as in the original).
static uint32_t GetWordCount(uint32_t end, uint32_t begin)
{
    return static_cast<uint32_t>(static_cast<int32_t>(end - begin) >> 2);
}

static int SelfAsInt(S_00403a70* self)
{
    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

// Assigns the word vector held in `source` to `self`
// (begin/end/capacity at +4/+8/+0xC).
static int __fastcall AssignWordVectorFromSource(S_00403a70* self, void* /*edx*/,
                                                 void* source)
{
    if (reinterpret_cast<void*>(self) == source)
        return SelfAsInt(self);

    uint32_t sourceCount = 0;
    if (CSF_AT(uint32_t, source, 0x04) != 0) {
        sourceCount = GetWordCount(CSF_AT(uint32_t, source, 0x08),
                                   CSF_AT(uint32_t, source, 0x04));
    }

    if (sourceCount == 0) {
        const uint32_t storage = self->field_0004;
        if (storage != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(storage));

        self->field_0004 = 0;
        self->field_0008 = 0;
        self->field_000c = 0;
        return SelfAsInt(self);
    }

    const uint32_t destinationBegin = self->field_0004;
    const uint32_t destinationCount = destinationBegin != 0
        ? GetWordCount(self->field_0008, destinationBegin)
        : 0;

    if (sourceCount <= destinationCount) {
        const uint32_t copyBegin = CSF_AT(uint32_t, source, 0x04);
        const uint32_t copyEnd = CSF_AT(uint32_t, source, 0x08);
        const uint32_t copyBytes = GetWordCount(copyEnd, copyBegin) << 2;
        _memcpy(reinterpret_cast<void*>(destinationBegin),
                reinterpret_cast<const void*>(copyBegin), copyBytes);

        const uint32_t updatedSourceBegin = CSF_AT(uint32_t, source, 0x04);
        uint32_t updatedCount = 0;
        if (updatedSourceBegin != 0) {
            updatedCount = GetWordCount(CSF_AT(uint32_t, source, 0x08),
                                        updatedSourceBegin);
        }
        self->field_0008 = self->field_0004 + (updatedCount << 2);
        return SelfAsInt(self);
    }

    const uint32_t capacity = destinationBegin != 0
        ? GetWordCount(self->field_000c, destinationBegin)
        : 0;

    if (sourceCount > capacity) {
        if (destinationBegin != 0) {
            ReleaseVectorStorage(reinterpret_cast<void*>(destinationBegin),
                                 static_cast<int>(GetWordCount(self->field_000c,
                                                               destinationBegin)));
        }

        const uint32_t allocSourceBegin = CSF_AT(uint32_t, source, 0x04);
        uint32_t allocCount = 0;
        if (allocSourceBegin != 0) {
            allocCount = GetWordCount(CSF_AT(uint32_t, source, 0x08),
                                      allocSourceBegin);
        }

        if (AllocateVectorStorage(self, nullptr, static_cast<int>(allocCount))) {
            const uint32_t sourceBegin = CSF_AT(uint32_t, source, 0x04);
            const uint32_t sourceEnd = CSF_AT(uint32_t, source, 0x08);
            self->field_0008 = static_cast<uint32_t>(
                Forward3ArgsWithContextTo4022E0(
                    self, nullptr, static_cast<int>(sourceBegin),
                    static_cast<int>(sourceEnd),
                    static_cast<int>(self->field_0004)));
        }
        return SelfAsInt(self);
    }

    const uint32_t existingCount = destinationBegin != 0
        ? GetWordCount(self->field_0008, destinationBegin)
        : 0;
    const uint32_t split = CSF_AT(uint32_t, source, 0x04) + (existingCount << 2);

    CopyWordsAndAdvance(reinterpret_cast<void*>(CSF_AT(uint32_t, source, 0x04)),
                        static_cast<int>(split),
                        reinterpret_cast<void*>(self->field_0004));

    self->field_0008 = static_cast<uint32_t>(
        Forward3ArgsWithContextTo4022E0(
            self, nullptr, static_cast<int>(split),
            static_cast<int>(CSF_AT(uint32_t, source, 0x08)),
            static_cast<int>(self->field_0008)));
    return SelfAsInt(self);
}

HOOK(0x006B5300, AssignWordVectorFromSource, "auto");
