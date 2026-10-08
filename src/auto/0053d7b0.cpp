#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap free (cdecl), bound by address.
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* block));

// Releases the panel's owned sub-object (vtable slot 0 with arg 1, i.e. a
// deleting destructor) and the heap buffer at +0x94, then clears the panel state.
static void __fastcall CMenuMissionInfo_ReleasePanelResources(CMenuMissionInfo* self, void* /*edx*/)
{
    void* ownedObject = reinterpret_cast<void*>(self->field_001c);
    if (ownedObject == nullptr)
        return;

    // Virtual slot 0 (byte offset 0), __thiscall with one argument (1).
    using DeletingDtorFn = void(__fastcall*)(void*, void*, int);
    DeletingDtorFn slot0 = reinterpret_cast<DeletingDtorFn>(CSF_AT(void**, ownedObject, 0)[0]);
    slot0(ownedObject, nullptr, 1);

    self->field_001c = 0;

    void* heapBuffer = reinterpret_cast<void*>(self->field_0094);
    self->field_0098 = 0;
    self->field_009c = 0;

    if (heapBuffer != nullptr) {
        Mem_Free(heapBuffer);
        self->field_0094 = 0;
    }
}

HOOK(0x0053D7B0, CMenuMissionInfo_ReleasePanelResources, "auto");
