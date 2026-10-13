#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Walks the entry array (stride 124 bytes) owned by this object. For each entry it
// resolves a menu object, asks it for a derived object (vtbl slot 21) using a field of
// the global state (+0x8C), stores that object at entry+0x78, then calls its vtbl slot 38
// with the address of this object's sub-object at +0x5C.
static int __fastcall C_0083f0ec_RebindEntryObjects(C_0083f0ec* self, void* /*edx*/, int /*a2*/)
{
    int remaining = 0;
    if (self->field_0008 != 0)
        remaining = static_cast<int32_t>(self->field_000c - self->field_0008) / 124;

    if (remaining != 0)
    {
        int32_t entryOffset = 0;
        do
        {
            // The entry base is re-read from the object on every iteration.
            uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008)) + entryOffset;

            int globalInt = GetGlobalInt();
            void* resolved = MenuControls_ResolveObject(
                reinterpret_cast<void*>(static_cast<uintptr_t>(globalInt)), nullptr);

            int globalState = GetGlobalState_50D190();
            uint32_t stateArg = CSF_AT(uint32_t,
                reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), 0x8c);

            using ComputeFn = int(__fastcall*)(void*, void*, uint32_t);
            auto computeFn = reinterpret_cast<ComputeFn>(CSF_AT(void**, resolved, 0)[84 / 4]);
            int computed = computeFn(resolved, nullptr, stateArg);

            CSF_AT(uint32_t, entry, 0x78) = static_cast<uint32_t>(computed);

            void* computedObj = reinterpret_cast<void*>(static_cast<uintptr_t>(computed));
            using InitFn = void(__fastcall*)(void*, void*, uint32_t*);
            auto initFn = reinterpret_cast<InitFn>(CSF_AT(void**, computedObj, 0)[152 / 4]);
            initFn(computedObj, nullptr,
                   reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x5c));

            entryOffset += 124;
        } while (--remaining != 0);
    }
    return remaining;
}

HOOK(0x004F2140, C_0083f0ec_RebindEntryObjects, "auto");
