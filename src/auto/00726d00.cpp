#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __cdecl, CheckObjectState, 0x006E2F30, (int object));
CSF_FUNC(int, __cdecl, GetObjectValue, 0x006E2E90, (int object));
CSF_GLOBAL(std::uint32_t, g_ObjectValueTotal, 0x008BBF34);

static int __cdecl UpdateObjectValueTotal(int object)
{
    if (object != 0)
    {
        CheckObjectState(object);
        const int value = GetObjectValue(object);
        CheckObjectState(object);
        g_ObjectValueTotal += static_cast<std::uint32_t>(value);
    }
    return object;
}

HOOK(0x00726D00, UpdateObjectValueTotal, "auto");
