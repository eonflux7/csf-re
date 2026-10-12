#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee 0x6CE3A0: __thiscall (ecx = dst), one stack arg (src), ret 4.
CSF_FUNC(char*, __fastcall, sub_6CE3A0, 0x006CE3A0, (void* self, void* edx, int a2));

// Copies each 100-byte element in [srcBegin, srcEnd) to the destination run:
// runs the element copier, then copies the dword at +0x60. Returns the
// destination pointer one past the last element written (or dstBegin if empty).
static char* __cdecl CollisionShapes_CopyElementRange(void* srcBegin, void* srcEnd, void* dstBegin, int /*unused*/)
{
    if (srcBegin == srcEnd)
        return static_cast<char*>(dstBegin);

    char* src = static_cast<char*>(srcBegin);
    char* dst = static_cast<char*>(dstBegin);
    do
    {
        sub_6CE3A0(dst, nullptr, reinterpret_cast<int>(src));
        CSF_AT(uint32_t, dst, 0x60) = CSF_AT(uint32_t, src, 0x60);
        src += 0x64;
        dst += 0x64;
    } while (src != srcEnd);

    return dst;
}

HOOK(0x006CE480, CollisionShapes_CopyElementRange, "auto");
