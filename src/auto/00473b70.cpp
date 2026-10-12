#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CopyDwordIntoSlot, 0x00471C90,
         (int a1, int a2, int a3, int a4, int a5));

CSF_FUNC(unsigned int, __fastcall, InsertDwordElements, 0x00473230,
         (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));

static int __fastcall AppendDwordPointerElement(
    void* self, void* /*edx*/, uint32_t* element)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x04);
    uint32_t size = 0;

    if (begin != 0) {
        size = static_cast<uint32_t>(
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x08) - begin) >> 2);
    }

    if (begin == 0 ||
        size >= static_cast<uint32_t>(
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x0C) - begin) >> 2)) {
        return static_cast<int>(InsertDwordElements(
            self, nullptr,
            reinterpret_cast<char*>(CSF_AT(uint32_t, self, 0x08)),
            1, element));
    }

    const uint32_t end = CSF_AT(uint32_t, self, 0x08);
    const int result = CopyDwordIntoSlot(
        static_cast<int>(end), 1,
        static_cast<int>(reinterpret_cast<uintptr_t>(element)),
        static_cast<int>(reinterpret_cast<uintptr_t>(self)), 0);
    CSF_AT(uint32_t, self, 0x08) = end + 4;
    return result;
}

HOOK(0x00473B70, AppendDwordPointerElement, "auto");
