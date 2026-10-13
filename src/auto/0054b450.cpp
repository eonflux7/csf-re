#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, TypeRegistry_AdvanceIterator, 0x0054B290, (int* self, void*));

// Clears the registry-backed contents of C_008442a0: walks the list at field_009c
// (advancing the iterator until it wraps back to the head), then calls the
// object at field_00a4's virtual slot 36 (+0x90) and returns its result.
// Returns field_00a0 unchanged when it is zero.
static int __fastcall C_008442a0_ClearContents(C_008442a0* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_00a0);
    if (result)
    {
        int* listHead = reinterpret_cast<int*>(self->field_009c);
        int* iter = reinterpret_cast<int*>(*listHead);
        while (iter != listHead)
        {
            // The callee takes the address of the iterator slot as its `this`.
            TypeRegistry_AdvanceIterator(reinterpret_cast<int*>(&iter), nullptr);
        }

        void* obj = reinterpret_cast<void*>(self->field_00a4);
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[36])(obj, nullptr);
    }
    return result;
}

HOOK(0x0054B450, C_008442a0_ClearContents, "auto");
