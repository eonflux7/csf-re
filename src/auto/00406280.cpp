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

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// __thiscall(this, a2, a3), ret 8.
// Looks up a matrix by key through the owner's child chain. If found, the
// matched object is asked to write into outAddr. Otherwise a new RW matrix is
// created, stored at outAddr, and the flag byte at outAddr+4 is set to 1.
static std::uint32_t __fastcall LookupMatrixByKeyOrCreate(void* self, void* /*edx*/,
                                                          std::uint32_t outAddr,
                                                          std::uint32_t key)
{
    // self->field_0008 holds the owner object.
    void* owner = CSF_AT(void*, self, 0x08);

    // owner->vtbl[0x28/4]() -> first child
    void* firstChild = ((void* (__fastcall*)(void*, void*))
                        CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    // firstChild->vtbl[0x68/4]() -> second child
    void* secondChild = ((void* (__fastcall*)(void*, void*))
                         CSF_AT(void**, firstChild, 0)[0x68 / 4])(firstChild, nullptr);

    // secondChild->vtbl[0x6c/4](key) -> match or null
    void* match = ((void* (__fastcall*)(void*, void*, std::uint32_t))
                   CSF_AT(void**, secondChild, 0)[0x6c / 4])(secondChild, nullptr, key);

    if (match != nullptr) {
        // match->vtbl[0x1c/4](outAddr)
        ((void (__fastcall*)(void*, void*, std::uint32_t))
         CSF_AT(void**, match, 0)[0x1c / 4])(match, nullptr, outAddr);
        return outAddr;
    }

    *reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(outAddr)) =
        reinterpret_cast<std::uint32_t>(RwMatrixCreate());
    *reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(outAddr) + 4) = 1;
    return outAddr;
}

HOOK(0x00406280, LookupMatrixByKeyOrCreate, "auto");
