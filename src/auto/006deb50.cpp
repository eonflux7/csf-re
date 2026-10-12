#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_006de5b0_EraseRange, 0x006DE5B0, (S_006de5b0* self, void* , uint32_t* result, char* first, char* last));
CSF_FUNC(void, __fastcall, S_006de5b0_InsertCopies, 0x006DE600, (void* self, void* edx, char* a1, unsigned int a3, void* arg8));

[[maybe_unused]] static auto const EraseRangeVtableAnnotation =
    CSF_VTBL(0x006DE5B0);

static void __fastcall S_006de5b0_Resize(
    S_006de5b0* self,
    void* /*edx*/,
    uint32_t requestedCount,
    uint32_t firstValueWord,
    int valueWord1,
    int valueWord2,
    int valueWord3,
    int valueWord4,
    int valueWord5,
    int valueWord6)
{
    const uint32_t begin = self->field_0004;
    const uint32_t currentCount = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0008 - begin) / 28)
        : 0;

    if (currentCount >= requestedCount)
    {
        if (begin &&
            requestedCount < static_cast<uint32_t>(
                static_cast<int32_t>(self->field_0008 - begin) / 28))
        {
            S_006de5b0_EraseRange(
                self, nullptr, &requestedCount,
                reinterpret_cast<char*>(begin + 28 * requestedCount),
                reinterpret_cast<char*>(self->field_0008));
        }
    }
    else
    {
        const int32_t existingCount = begin
            ? static_cast<int32_t>(self->field_0008 - begin) / 28
            : 0;

        // The original forwards a contiguous 28-byte by-value record.
        uint32_t fillValue[7] = {
            firstValueWord,
            static_cast<uint32_t>(valueWord1),
            static_cast<uint32_t>(valueWord2),
            static_cast<uint32_t>(valueWord3),
            static_cast<uint32_t>(valueWord4),
            static_cast<uint32_t>(valueWord5),
            static_cast<uint32_t>(valueWord6)
        };

        S_006de5b0_InsertCopies(
            self, nullptr,
            reinterpret_cast<char*>(self->field_0008),
            requestedCount - static_cast<uint32_t>(existingCount),
            fillValue);
    }
}

HOOK(0x006DEB50, S_006de5b0_Resize, "auto");
