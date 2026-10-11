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

CSF_FUNC(int, __fastcall, GetSubObjectField18, 0x006B8280, (void* self, void*));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));

// Slot 0x28 on the field_0008 object: no stack args, returns a child object pointer.
using ObjectGetterFn = void*(__fastcall*)(void* self, void* edx);
// Slot 0x84 on that object: returns a nonzero flag when the object is active.
using FlagGetterFn = int(__fastcall*)(void* self, void* edx);
// Slot 0x80 on that object: takes one int argument and returns a sub-object pointer.
using SubObjectGetterFn = void*(__fastcall*)(void* self, void* edx, int index);

static int __fastcall C_0082d844_GetAttachedObjectField18(C_0082d844* self, void* /*edx*/)
{
    int returnValue = 0;

    if (!self->field_0008)
        return 0;

    void* component = reinterpret_cast<void*>(self->field_0008);
    auto getObject = reinterpret_cast<ObjectGetterFn>(CSF_AT(void**, component, 0)[0x28 / 4]);
    void* object = getObject(component, nullptr);
    if (!object)
        return 0;

    auto isActive = reinterpret_cast<FlagGetterFn>(CSF_AT(void**, object, 0)[0x84 / 4]);
    if (!isActive(object, nullptr))
        return returnValue;

    auto getSubObject = reinterpret_cast<SubObjectGetterFn>(CSF_AT(void**, object, 0)[0x80 / 4]);
    void* subObject = getSubObject(object, nullptr, 0);

    int sceneSourceId = CSF_AT(int, subObject, 0x0C);
    int objectAttachment = SceneLoader_GetObjectAttachment(sceneSourceId);
    if (objectAttachment)
        returnValue = GetSubObjectField18(reinterpret_cast<void*>(objectAttachment), nullptr);

    return returnValue;
}

HOOK(0x004092A0, C_0082d844_GetAttachedObjectField18, "auto");
