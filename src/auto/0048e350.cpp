#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Virtual slot signatures used by this method (all __thiscall, written as __fastcall).
using VirtIntFromInt = int(__fastcall*)(void* self, void* edx, int arg);
using VirtPtrFromInt = void*(__fastcall*)(void* self, void* edx, int arg);
using VirtIntNoArg   = int(__fastcall*)(void* self, void* edx);

// Looks up a key through the manager at self+0x16C, resolves the key to a child
// object, and invokes the child's slot-22 method. Returns -1 if no child is found.
static int __fastcall Owner_ActivateChildByKey(void* self, void* /*edx*/, int key)
{
    void** manager = CSF_AT(void**, self, 0x16C);
    int keyValue = reinterpret_cast<VirtIntFromInt>(CSF_AT(void**, manager, 0)[34])(manager, nullptr, key);

    // The manager is re-read from self+0x16C after the first call, as the original does.
    manager = CSF_AT(void**, self, 0x16C);
    void* childObj = reinterpret_cast<VirtPtrFromInt>(CSF_AT(void**, manager, 0)[32])(manager, nullptr, keyValue);

    if (childObj != nullptr)
        return reinterpret_cast<VirtIntNoArg>(CSF_AT(void**, childObj, 0)[22])(childObj, nullptr);

    return -1;
}

HOOK(0x0048E350, Owner_ActivateChildByKey, "auto");
