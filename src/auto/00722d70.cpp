#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall ReplaceOwnedObject(
    void* self, void* /*edx*/, void* replacement)
{
    int result;
    void* previousObject = CSF_AT(void*, self, 0x218);

    if (previousObject)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previousObject, 0)[0])(
                previousObject, nullptr, 1);
        CSF_AT(void*, self, 0x218) = nullptr;
    }

    CSF_AT(void*, self, 0x218) = replacement;
    if (replacement)
    {
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, replacement, 0)[0x6C / 4])(
                replacement, nullptr, 1);
    }

    return result;
}

HOOK(0x00722D70, ReplaceOwnedObject, "auto");
