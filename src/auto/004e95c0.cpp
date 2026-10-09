#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));

// Resolves the object definition for objectId, then notifies the target's handler (at +0x44)
// about each linked sub-object that the index table can resolve.
static int __fastcall S_004e9090_NotifyLinkedObjects(S_004e9090* self, void* /*edx*/, C_00833908* target, int objectId)
{
    void* definitionTable = reinterpret_cast<void*>(self->field_1fc8);
    uint8_t* definition = ((uint8_t*(__fastcall*)(void*, void*, int))CSF_AT(void**, definitionTable, 0)[2])(
        definitionTable, nullptr, objectId);

    if (target->field_0044 == 0)
    {
        int linkedCount = CSF_AT(int, definition, 0x108);
        if (linkedCount != 0)
            return HandleTargetUnavailable(target, nullptr);
        return linkedCount;
    }

    int removable = Scene_IsObjectRemovable(CSF_AT(int, definition, 0xB4));
    if ((uint8_t)removable != 0)
        return removable;

    uint32_t linkedCount = CSF_AT(uint32_t, definition, 0x108);
    for (uint32_t i = 0; i < linkedCount; ++i)
    {
        void* indexTable = CSF_AT(void*, self, 0x1FC0);
        int linkedId = CSF_AT(int*, definition, 0x100)[i];
        int index = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, indexTable, 0)[3])(
            indexTable, nullptr, linkedId);
        if (index != -1)
        {
            void* handler = reinterpret_cast<void*>(target->field_0044);
            ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, handler, 0)[42])(
                handler, nullptr, index, 0);
        }
    }
    return (int)linkedCount;
}

HOOK(0x004E95C0, S_004e9090_NotifyLinkedObjects, "auto");
