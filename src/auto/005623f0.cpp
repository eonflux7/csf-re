#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00403880: __thiscall ret 8 -> erase(pos, count)
CSF_FUNC(void*, __fastcall, StringEraseRange, 0x403880,
         (void* self, void* edx, unsigned int pos, unsigned int count));

// 0x00512630: __thiscall ret 8 -> append(count, ch)
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x512630,
         (void* self, void* edx, unsigned int count, char ch));

// basic_string::resize(newSize):
//   if (newSize > size) append(newSize - size, '\0');
//   else                erase(newSize, npos);
static void* __fastcall StringResize(void* self, void* /*edx*/, unsigned int newSize)
{
    unsigned int currentSize = CSF_AT(uint32_t, self, 0x14);
    if (newSize > currentSize)
        return StringAppendFill(self, nullptr, newSize - currentSize, 0);
    return StringEraseRange(self, nullptr, newSize, 0xFFFFFFFFu);
}

HOOK(0x5623F0, StringResize, "auto");
