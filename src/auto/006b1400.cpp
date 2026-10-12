#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t, __fastcall, IncrementSaturatingCounter, 0x0069ADB0, (void* self, void*));
CSF_FUNC(char*, __cdecl, sub_69AE30, 0x0069AE30, (int a1, char* Format, ...));

// Increments the saturating counter on the object, then formats the object
// handle with the "BASICA. " tag through sub_69AE30 and returns its result.
static int __stdcall IncrementCounterAndFormatBasicaPrefix(int a1)
{
    IncrementSaturatingCounter(reinterpret_cast<void*>(a1), nullptr);
    return reinterpret_cast<int>(sub_69AE30(a1, const_cast<char*>(CSF_STR(0x00860114, "BASICA. "))));
}

HOOK(0x6B1400, IncrementCounterAndFormatBasicaPrefix, "auto");
