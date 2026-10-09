#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void**, __fastcall, MissionScript_EraseValueRange, 0x0046F5E0,
         (void* self, void* edx, void** a2, void* a3, int a4));
CSF_FUNC(char*, __fastcall, MissionScript_InsertValueCopies, 0x00470010,
         (void* self, void* edx, void* a1, unsigned int a3, int arg8));
CSF_FUNC(char*, __fastcall, Object_PrepareForDestruction, 0x004734D0,
         (char** self, void* edx));

[[maybe_unused]] static void** const g_RangeEraseTable =
    CSF_VTBL(0x0046F5E0);

static char* __fastcall S_0046f5e0_Resize(
    S_0046f5e0* self, void* /*edx*/,
    unsigned int requestedSize, C_008313f8 fillValue)
{
    const uint32_t begin = self->field_0004;
    const unsigned int size = begin
        ? static_cast<unsigned int>(
              static_cast<int32_t>(self->field_0008 - begin) / 12)
        : 0;

    if (size >= requestedSize)
    {
        if (begin)
        {
            const uint32_t end = self->field_0008;
            const unsigned int currentSize =
                static_cast<unsigned int>(
                    static_cast<int32_t>(end - begin) / 12);

            if (requestedSize < currentSize)
            {
                MissionScript_EraseValueRange(
                    self, nullptr,
                    reinterpret_cast<void**>(&requestedSize),
                    reinterpret_cast<void*>(begin + 12 * requestedSize),
                    static_cast<int32_t>(end));
            }
        }
    }
    else
    {
        const unsigned int currentSize = begin
            ? static_cast<unsigned int>(
                  static_cast<int32_t>(self->field_0008 - begin) / 12)
            : 0;

        MissionScript_InsertValueCopies(
            self, nullptr,
            reinterpret_cast<void*>(self->field_0008),
            requestedSize - currentSize,
            static_cast<int>(reinterpret_cast<uintptr_t>(&fillValue)));
    }

    return Object_PrepareForDestruction(
        reinterpret_cast<char**>(&fillValue), nullptr);
}

HOOK(0x004704E0, S_0046f5e0_Resize, "auto");
