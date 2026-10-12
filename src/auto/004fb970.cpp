#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

static int __fastcall DevConsole_ReleaseResource(void* self, void* /*edx*/)
{
    if (!CSF_AT(void*, self, 0x04))
        return 0;

    void* manager = reinterpret_cast<void*>(ResolveRwManager(GetGlobalInt(), nullptr));
    void* resource = CSF_AT(void*, self, 0x04);
    void** resourceVtable = CSF_AT(void**, resource, 0);
    void** managerVtable = CSF_AT(void**, manager, 0);
    int handle = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        resourceVtable[0x14 / 4])(resource, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        managerVtable[0x14 / 4])(manager, nullptr, handle);

    resource = CSF_AT(void*, self, 0x04);
    int count = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, resource, 0)[0x38 / 4])(resource, nullptr);
    if (count > 0)
    {
        manager = reinterpret_cast<void*>(ResolveRwManager(GetGlobalInt(), nullptr));
        resource = CSF_AT(void*, self, 0x04);
        resourceVtable = CSF_AT(void**, resource, 0);
        managerVtable = CSF_AT(void**, manager, 0);
        int item = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            resourceVtable[0x34 / 4])(resource, nullptr, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            managerVtable[0x24 / 4])(manager, nullptr, item);
    }

    manager = reinterpret_cast<void*>(ResolveRwManager(GetGlobalInt(), nullptr));
    resource = CSF_AT(void*, self, 0x04);
    int result = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, manager, 0)[0x1C / 4])(manager, nullptr, resource);
    CSF_AT(void*, self, 0x04) = nullptr;
    return result;
}

HOOK(0x004FB970, DevConsole_ReleaseResource, "auto");
