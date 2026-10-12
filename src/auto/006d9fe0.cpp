#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_6C4490, 0x006C4490, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6C35C0, 0x006C35C0, (void* self, void* edx, int a2, int a3));

// Allocates a 52-byte state object, initializes it through sub_6C4490 when the
// allocation succeeds, runs sub_6C35C0(1800, 0) on it, and marks its state field as 3.
static int __cdecl CreateStateObjectWithDefaults()
{
    void* allocated = Mem_Alloc(52);
    int objectValue;
    if (allocated)
        objectValue = sub_6C4490(allocated, nullptr);
    else
        objectValue = 0;

    void* objPtr = reinterpret_cast<void*>(static_cast<std::uintptr_t>(objectValue));
    sub_6C35C0(objPtr, nullptr, 1800, 0);
    CSF_AT(int, objPtr, 0x1c) = 3;
    return objectValue;
}

HOOK(0x6D9FE0, CreateStateObjectWithDefaults, "auto");
