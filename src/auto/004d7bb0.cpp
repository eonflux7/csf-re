#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00834fa8_RunStateHandlerAtOffset14, 0x004210C0, (C_00834fa8* self, void*));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Resets the task's helper objects in their original order, then starts movement
// using the embedded sub-object at this+0x54 as the movement argument.
static int __fastcall C_0083dbb0_ResetAndStartMovement(C_0083dbb0* self, void* /*edx*/)
{
    // Helper at +0x308: virtual slot +0xA0
    void* stateObj = reinterpret_cast<void*>(self->field_0308);
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, stateObj, 0)[0xA0 / 4])(stateObj, nullptr);

    // Helper at +0x30C: virtual slot +0x8C
    void* actorObj = reinterpret_cast<void*>(self->field_030c);
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, actorObj, 0)[0x8C / 4])(actorObj, nullptr);

    // Helper at +0x310: run state handler at offset 14
    C_00834fa8_RunStateHandlerAtOffset14(reinterpret_cast<C_00834fa8*>(self->field_0310), nullptr);

    // Helper at +0x30C again: virtual slot +0x90
    actorObj = reinterpret_cast<void*>(self->field_030c);
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, actorObj, 0)[0x90 / 4])(actorObj, nullptr);

    // Parent object at +0x14: child at +0x3C, virtual slot +0x94
    uint8_t* parentBytes = reinterpret_cast<uint8_t*>(self->field_0014);
    void* childA = reinterpret_cast<void*>(CSF_AT(uint32_t, parentBytes, 0x3C));
    if (childA != nullptr)
        ((void (__fastcall*)(void*, void*))CSF_AT(void**, childA, 0)[0x94 / 4])(childA, nullptr);

    // Parent object at +0x14 again: child at +0x40, virtual slot +0x80
    parentBytes = reinterpret_cast<uint8_t*>(self->field_0014);
    void* childB = reinterpret_cast<void*>(CSF_AT(uint32_t, parentBytes, 0x40));
    if (childB != nullptr)
        ((void (__fastcall*)(void*, void*))CSF_AT(void**, childB, 0)[0x80 / 4])(childB, nullptr);

    // Movement start with the address of the embedded sub-object this+0x54
    return CharTask_StartMovement(self, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x54));
}

HOOK(0x4D7BB0, C_0083dbb0_ResetAndStartMovement, "auto");
