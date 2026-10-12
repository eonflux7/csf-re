#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxClumpDict_ConstructElementAtEnd, 0x006D4DF0, (int a1, int a2, void* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, GfxClumpDict_GrowAndEmplaceElement, 0x006D5390, (void* self, void* edx, uint32_t* a2, void* a3, int a4));

// Appends one 60-byte element to the buffer (begin = +4, end = +8, capacity = +0C).
// If there is no storage or the buffer is full, it delegates to the grow/reallocate path (sub_6D5390).
// Otherwise it constructs the new element at end (sub_6D4DF0) and advances end by 60.
static uint32_t* __fastcall GfxClumpDict_EmplaceBack(S_006d5390* self, void* /*edx*/, void* elementArg)
{
    uint32_t begin = self->field_0004;
    unsigned int index = 0;
    if (begin != 0)
        index = static_cast<unsigned int>(static_cast<int>(self->field_0008 - begin) / 60);

    if (begin == 0 || index >= static_cast<unsigned int>(static_cast<int>(self->field_000c - begin) / 60))
    {
        (void)CSF_VTBL(0x006D5390);
        return GfxClumpDict_GrowAndEmplaceElement(
            self, nullptr,
            reinterpret_cast<uint32_t*>(&elementArg),
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(elementArg)));
    }

    uint32_t end = self->field_0008;
    int constructed = GfxClumpDict_ConstructElementAtEnd(
        static_cast<int>(end), 1, elementArg,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(elementArg)));
    uint32_t* result = reinterpret_cast<uint32_t*>(
        static_cast<std::uintptr_t>(static_cast<uint32_t>(constructed)));
    self->field_0008 = end + 60;
    return result;
}

HOOK(0x006D54A0, GfxClumpDict_EmplaceBack, "auto");
