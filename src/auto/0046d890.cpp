#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, MissionScript_EraseEntryRange, 0x0046CBA0,
         (void* self, void* edx, int* a4, int a1, uint32_t* arg8));
CSF_FUNC(unsigned int, __fastcall, MissionScript_InsertEntries, 0x0046CC00,
         (void* self, void* edx, int a1, unsigned int a3, int* arg8));
CSF_FUNC(int, __cdecl, MissionScript_ReleaseValueStorage, 0x004714E0,
         (void* Block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int* __fastcall S_0046cba0_ResizeAndReleaseTemporary(
    S_0046cba0* self, void* /*edx*/,
    unsigned int requestedCount, int fillValue, int temporaryAddress)
{
    // The packet also identifies the direct erase callee as a vtable reference.
    (void)CSF_VTBL(0x0046CBA0);

    const uint32_t begin = self->field_0004;
    uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0008 - begin) >> 3)
        : 0;
    int* result = reinterpret_cast<int*>(static_cast<uintptr_t>(count));

    if (count < requestedCount)
    {
        count = begin
            ? static_cast<uint32_t>(
                  static_cast<int32_t>(self->field_0008 - begin) >> 3)
            : 0;
        result = reinterpret_cast<int*>(static_cast<uintptr_t>(
            MissionScript_InsertEntries(
                self, nullptr, static_cast<int>(self->field_0008),
                requestedCount - count, &fillValue)));
    }
    else if (begin)
    {
        const uint32_t end = self->field_0008;
        count = static_cast<uint32_t>(
            static_cast<int32_t>(end - begin) >> 3);
        result = reinterpret_cast<int*>(static_cast<uintptr_t>(count));

        if (requestedCount < count)
        {
            result = MissionScript_EraseEntryRange(
                self, nullptr, reinterpret_cast<int*>(&requestedCount),
                static_cast<int>(begin + 8 * requestedCount),
                reinterpret_cast<uint32_t*>(end));
        }
    }

    if (temporaryAddress)
    {
        void* temporary = reinterpret_cast<void*>(temporaryAddress);
        S_0069b1c0* value = CSF_AT(S_0069b1c0*, temporary, 0x004);
        if (value)
        {
            ((void (__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, value, 0)[0])(value, nullptr, 0);
            MissionScript_ReleaseValueStorage(value);
        }
        CSF_AT(uint32_t, temporary, 0x004) = 0;
        return reinterpret_cast<int*>(Mem_Free(temporary));
    }

    return result;
}

HOOK(0x0046D890, S_0046cba0_ResizeAndReleaseTemporary, "auto");
