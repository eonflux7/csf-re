#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static bool __fastcall CoreSortedTable_NameEquals(void* self, void* /*edx*/, const char* name)
{
    alignas(8) uint8_t stringBytes[0x1C];
    C_0082d410& v6 = *reinterpret_cast<C_0082d410*>(stringBytes);

    CSF_AT(uint32_t, stringBytes, 0x18) = 15;
    CSF_AT(uint32_t, stringBytes, 0x14) = 0;
    CSF_AT(uint8_t, stringBytes, 0x04) = 0;

    AssignStringRange_429B60(&v6, nullptr, const_cast<char*>(name),
                             static_cast<unsigned int>(std::strlen(name)));

    uint32_t textPtr = v6.field_0004;
    if (CSF_AT(uint32_t, stringBytes, 0x18) < 0x10)
        textPtr = reinterpret_cast<uint32_t>(&v6.field_0004);

    const uint32_t storedCount = CSF_AT(uint32_t, self, 0x50);
    int compareResult = CompareStringSubstring(
        static_cast<uint8_t*>(self) + 0x3C, nullptr, 0, storedCount,
        reinterpret_cast<const char*>(textPtr),
        CSF_AT(uint32_t, stringBytes, 0x14));
    bool isEqual = (compareResult == 0);

    if (CSF_AT(uint32_t, stringBytes, 0x18) >= 0x10 && v6.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(v6.field_0004));

    return isEqual;
}

HOOK(0x00699490, CoreSortedTable_NameEquals, "auto");
