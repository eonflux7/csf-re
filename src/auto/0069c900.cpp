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

CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

// Original: char* __thiscall sub_69C900(this, char* Buffer, unsigned a3); ret 8.
// When a3 >= 9, reads three values from the object's virtual getters (slots 0x20, 0x1C, 0x18),
// then formats them with "%02d:%02d:%02d". Otherwise returns Buffer untouched.
// The getters take no stack arguments; the values the original pushes before each call
// remain on the stack and are consumed as the sprintf varargs (add esp, 0x14).
static char* __fastcall C_0085e92c_FormatTimeHHMMSS(C_0085e92c* self, void* /*edx*/, char* Buffer, unsigned int a3)
{
    if (a3 >= 9)
    {
        int v4 = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x20 / 4])(self, nullptr);
        int v5 = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x1C / 4])(self, nullptr);
        int v6 = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x18 / 4])(self, nullptr);
        _sprintf(Buffer, CSF_STR(0x0085E918, "%02d:%02d:%02d"), v6, v5, v4);
    }
    return Buffer;
}

HOOK(0x0069C900, C_0085e92c_FormatTimeHHMMSS, "auto");
