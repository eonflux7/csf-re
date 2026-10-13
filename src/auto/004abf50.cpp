#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static int __stdcall AiCoverTasks_ApplyCoverAttack(int alertHandle, const uint32_t* attackData)
{
    void* alert = reinterpret_cast<void*>(alertHandle);

    int linkedValue = GetLinkedObjectValue_4DB5C0(alert, nullptr);
    int objectAddress = linkedValue ? linkedValue - 4 : 0;
    void* object = reinterpret_cast<void*>(objectAddress);
    void* controller = CSF_AT(void*, object, 0x2C);

    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, controller, 0)[0x8C / 4])(
            controller, nullptr, 0, 0, -1, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(alert, nullptr);
    objectAddress = linkedValue ? linkedValue - 4 : 0;
    object = reinterpret_cast<void*>(objectAddress);
    void* coverTask = reinterpret_cast<uint8_t*>(object) + 0x258;

    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, coverTask, 0)[0x28 / 4])(
            coverTask, nullptr, alertHandle);

    linkedValue = GetLinkedObjectValue_4DB5C0(alert, nullptr);
    objectAddress = linkedValue ? linkedValue - 4 : 0;
    object = reinterpret_cast<void*>(objectAddress);

    CSF_AT(uint32_t, object, 0x26C) = attackData[0];
    CSF_AT(uint32_t, object, 0x270) = attackData[1];
    CSF_AT(uint32_t, object, 0x274) = attackData[2];
    CSF_AT(uint32_t, object, 0x278) = attackData[3];

    linkedValue = GetLinkedObjectValue_4DB5C0(alert, nullptr);
    objectAddress = linkedValue ? linkedValue - 4 : 0;
    object = reinterpret_cast<void*>(objectAddress);

    CSF_AT(float, object, 0x264) = 0.3f;
    CSF_AT(float, object, 0x268) = 0.3f;
    return objectAddress;
}

HOOK(0x004ABF50, AiCoverTasks_ApplyCoverAttack, "auto");
