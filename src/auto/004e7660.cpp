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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsStateCompatible, 0x0048EF90, (void* self, void*, int requestedState));
CSF_FUNC(int32_t, __fastcall, IsField8NonNegative, 0x004AD7E0, (void* self, void*));
CSF_FUNC(int, __fastcall, HasValidId, 0x004AD7F0, (void* self, void*));
CSF_FUNC(int, __fastcall, HasAssignedId, 0x004AD800, (void* self, void*));
CSF_FUNC(int*, __fastcall, C_Object_SelectBuffer, 0x004ADAD0, (int* self, void*));
CSF_FUNC(int, __fastcall, TargetRecord_Activate, 0x004ADB50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_IsTargetRecordValid, 0x004B2290, (void* self, void* edx));
CSF_FUNC(char, __fastcall, TargetFilter_IsSameTarget, 0x004E73F0, (void* self, void* edx, int a2));

static inline void* ToPtr(uint32_t v)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(v));
}

static bool __fastcall C_0083eb70_CheckTargetHandle(C_0083eb70* self, void* /*edx*/, int handle)
{
    bool result = false;

    int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
    uint8_t* entity = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(resolved));
    if (!entity)
        return false;

    if ((CSF_AT(uint32_t, entity, 0x170) & 0x2000) == 0)
        return result;

    if (!static_cast<uint8_t>(IsStateCompatible(ToPtr(self->field_0008), nullptr,
                                                 CSF_AT(int, entity, 0x78))))
        return result;

    void** controller = reinterpret_cast<void**>(static_cast<uintptr_t>(CSF_AT(uint32_t, entity, 0x34)));
    if (!controller)
        return result;

    void** vtbl = CSF_AT(void**, controller, 0);
    int kind = ((int(__fastcall*)(void*, void*))vtbl[0x98 / 4])(controller, nullptr);
    if (kind != 1)
        return result;

    uint8_t* controllerObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, entity, 0x34)));
    uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, controllerObj, 0x30C))) + 0x5DC;

    if (!static_cast<uint8_t>(AiCore_IsTargetRecordValid(record, nullptr)))
        return true;

    uint8_t* selfId = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008)) + 0x1C;
    int* buf = C_Object_SelectBuffer(reinterpret_cast<int*>(record), nullptr);
    uint32_t selfValue = CSF_AT(uint32_t, selfId, 4);
    uint32_t bufValue = static_cast<uint32_t>(buf[1]);
    result = (bufValue == selfValue);
    if (result)
        return result;

    int* buf2 = C_Object_SelectBuffer(reinterpret_cast<int*>(record), nullptr);
    result = TargetFilter_IsSameTarget(self, nullptr, reinterpret_cast<int>(buf2)) != 0;
    if (result)
        return result;

    void* idField = record + 0x28;
    if (static_cast<uint8_t>(HasValidId(idField, nullptr)))
    {
        if (static_cast<uint8_t>(IsField8NonNegative(record + 0x4C, nullptr)) == 0)
        {
            uint32_t state = CSF_AT(uint32_t, record, 0x2C);
            if (state == 5 || state == 0xB)
                return true;
        }
        return static_cast<uint8_t>(TargetRecord_Activate(record, nullptr)) != 0;
    }

    if (static_cast<uint8_t>(HasAssignedId(idField, nullptr)))
        return static_cast<uint8_t>(TargetRecord_Activate(record, nullptr)) != 0;

    return result;
}

HOOK(0x004E7660, C_0083eb70_CheckTargetHandle, "auto");
