#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Menu_ConstructPageEntry, 0x0046FC50, (int a1, int a2, void* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, Menu_GrowAndAppend, 0x00470AB0, (void* self, void* edx, uint32_t* a2, void* a3, void* a4));

// Appends one 28-byte page entry to the buffer (begin = +4, end = +8, capacity = +0C).
// If there is no storage or the buffer is full, it delegates to the grow path (Menu_GrowAndAppend).
// Otherwise it constructs the new entry at end (Menu_ConstructPageEntry) and advances end by 28.
// The return value is the callee's result pointer, passed through as void* (the original's std::_String_base* return).
static void* __fastcall MenuManager_AppendPageName(S_00470ab0* self, void* /*edx*/, void* a3)
{
    uint32_t begin = self->field_0004;
    unsigned int index = 0;
    if (begin != 0)
        index = static_cast<unsigned int>(static_cast<int>(self->field_0008 - begin) / 28);

    if (begin == 0 || index >= static_cast<unsigned int>(static_cast<int>(self->field_000c - begin) / 28))
    {
        (void)CSF_VTBL(0x00470AB0);
        return reinterpret_cast<void*>(
            Menu_GrowAndAppend(self, nullptr, reinterpret_cast<uint32_t*>(&a3),
                               reinterpret_cast<void*>(self->field_0008), a3));
    }

    uint32_t end = self->field_0008;
    void* result = Menu_ConstructPageEntry(static_cast<int>(end), 1,
                                           a3,
                                           static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
                                           static_cast<int>(reinterpret_cast<std::uintptr_t>(a3)));
    self->field_0008 = end + 28;
    return result;
}

HOOK(0x00470FD0, MenuManager_AppendPageName, "auto");
