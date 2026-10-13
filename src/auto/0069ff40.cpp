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

// Sets the default file system: stores the mode (3) in the manager's state field,
// forwards the file-system argument to the object at +0x40 through vtable slot 5
// (index 0/1/2 by mode 1/2/3), then queries vtable slot 4 for the result.
static int RtFSManager_SetDefaultFileSystem(int managerAddr, int fileSystemArg, int fsType)
{
    uint8_t* self = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(managerAddr));
    int result;

    CSF_AT(int, self, 0x20) = 3;

    switch (fsType)
    {
    case 1:
    {
        void* obj = CSF_AT(void*, self, 0x40);
        ((void (__fastcall*)(void*, void*, int, int))CSF_AT(void**, obj, 0)[5])(obj, nullptr, 0, fileSystemArg);
        break;
    }
    case 2:
    {
        void* obj = CSF_AT(void*, self, 0x40);
        ((void (__fastcall*)(void*, void*, int, int))CSF_AT(void**, obj, 0)[5])(obj, nullptr, 1, fileSystemArg);
        break;
    }
    case 3:
    {
        void* obj = CSF_AT(void*, self, 0x40);
        ((void (__fastcall*)(void*, void*, int, int))CSF_AT(void**, obj, 0)[5])(obj, nullptr, 2, fileSystemArg);
        break;
    }
    default:
        break;
    }

    void* obj = CSF_AT(void*, self, 0x40);
    result = ((int (__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[4])(obj, nullptr);

    CSF_AT(int, self, 0x08) = result;
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x20) = 2;
    return result;
}

HOOK(0x0069FF40, RtFSManager_SetDefaultFileSystem, "auto");
