#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Releases an array-like block whose element count sits in the dword just before
// the data pointer. A nonzero count dispatches to the object's vtable slot 1 with
// flag 3; a zero count frees the raw block (data pointer minus the 4-byte header).
static int __cdecl ReleaseCountedArrayOrFreeBlock(int dispatchKind, int dataPointer)
{
    int result = dispatchKind;

    switch (dispatchKind)
    {
    case 0:
    case 2:
    case 4:
    case 1:
    case 3:
    case 5:
        if (dataPointer != 0)
        {
            uint8_t* dataBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(dataPointer));
            uint32_t elementCount = *reinterpret_cast<uint32_t*>(dataBase - 4);

            if (elementCount != 0)
            {
                // Virtual call: vtable slot 1 (offset 4), __stdcall, one argument (3).
                using VirtualReleaseFn = int(__stdcall*)(int);
                VirtualReleaseFn releaseFn =
                    reinterpret_cast<VirtualReleaseFn>(CSF_AT(void**, dataBase, 0)[1]);
                result = releaseFn(3);
            }
            else
            {
                result = CoreMemory_Free(reinterpret_cast<void*>(dataBase - 4));
            }
        }
        break;

    default:
        return result;
    }

    return result;
}

HOOK(0x006C9ED0, ReleaseCountedArrayOrFreeBlock, "auto");
