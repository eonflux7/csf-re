#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __stdcall ResolveSubobjectAndNotify(int a1, int a2)
{
    void* owner = reinterpret_cast<void*>(a1);
    int resolved = ((int(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[10])(owner, nullptr);

    void* resolvedObj = reinterpret_cast<void*>(resolved);
    int result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, resolvedObj, 0)[32])(resolvedObj, nullptr, a2);

    if (result)
    {
        void* child = reinterpret_cast<void*>(result);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[8])(child, nullptr, 0);
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[12])(child, nullptr, 0);
    }
    return result;
}

HOOK(0x44D370, ResolveSubobjectAndNotify, "auto");
