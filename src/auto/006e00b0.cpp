#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, C_00861988_AllocateElementArray, 0x006C9CE0, (int elementType, int count, std::uint32_t* elementSizeOut));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_ApplySkinRegionSelection, 0x006C57A0, (C_0085e7b8** self, void* , C_0086754c* a2));
CSF_FUNC(void*, __fastcall, RecordArray_Grow_6D9EB0, 0x006D9EB0, (void* self, void* edx, int a2));

// Allocates an array of `count` elements of `*elementSize` bytes, applies the skin
// region selection to each element, then appends a {array, elementSize, count} record.
static char* __fastcall C_00861988_AllocateSkinRegionArray(C_00861988* self, void* /*edx*/, int elementType, int count, std::uint32_t* elementSize)
{
    char* elementArray = reinterpret_cast<char*>(C_00861988_AllocateElementArray(elementType, count, elementSize));
    if (elementArray != nullptr)
    {
        std::uint32_t totalBytes = static_cast<std::uint32_t>(count) * *elementSize;
        for (std::uint32_t offset = 0; offset < totalBytes; offset += *elementSize)
        {
            int globalValue = GetGlobalValue_6C5760();
            C_0085e7b8_ApplySkinRegionSelection(
                reinterpret_cast<C_0085e7b8**>(static_cast<std::uintptr_t>(globalValue)),
                nullptr,
                reinterpret_cast<C_0086754c*>(elementArray + offset));
            ++self->field_0050;
        }

        std::uint32_t elementSizeValue = *elementSize;

        if (self->field_003c >= CSF_AT(std::uint32_t, self, 0x38))
        {
            RecordArray_Grow_6D9EB0(
                reinterpret_cast<void*>(reinterpret_cast<std::uint8_t*>(self) + 0x34),
                nullptr,
                static_cast<int>(CSF_AT(std::uint32_t, self, 0x40)));
        }

        std::uint32_t index = self->field_003c;
        char** record = reinterpret_cast<char**>(static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 0x34)));
        char** slot = record + 3 * index;
        self->field_003c = index + 1;
        slot[0] = elementArray;
        slot[1] = reinterpret_cast<char*>(static_cast<std::uintptr_t>(elementSizeValue));
        slot[2] = reinterpret_cast<char*>(static_cast<std::uintptr_t>(count));
    }
    return elementArray;
}

HOOK(0x006E00B0, C_00861988_AllocateSkinRegionArray, "auto");
