#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082dd78_StoreByteAndSetFlag, 0x0040FD20, (C_0082dd78* self, void* , char a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, AiMountTasks_CopyCommandHandle, 0x00492AE0, (void* self, void* , void* src, uint32_t thirdValue));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (C_008342e8* self, void* , int a2));
CSF_FUNC(bool, __fastcall, IsEntryFrameBlocked, 0x0049ED50, (C_008342e8* self, void* , int entryIndex));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (C_008342e8* self, void* , int index, uint32_t* out));
CSF_FUNC(uint32_t*, __fastcall, C_008342e8_PrepareEntryFrame, 0x0049EDC0, (C_008342e8* self, void* , int entryIndex, int sourceAddr));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovement_4DB140, 0x004DB140, (C_0082f9f8* self, void* , int movement));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Keep each dispatch independent: the first call may change the vtable.
static __declspec(noinline) int __fastcall AiMountTasks_GetControlledObject(
    void* controller, void* /*edx*/)
{
    return ((int (__fastcall*)(void*, void*))
        CSF_AT(void**, controller, 0)[0xD8 / 4])(controller, nullptr);
}

static int __fastcall C_0082f9f8_DispatchToCommandSubobject(
    C_0082f9f8* self, void* /*edx*/, int handle, int entryIndex)
{
    auto* entry = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));

    if (!entry || IsEntryFrameBlocked(entry, nullptr, entryIndex))
    {
        return C_00833908_GetTargetName(
            CSF_AT(C_00833908*, CSF_AT(void*, self, 0x8), 0x118),
            nullptr);
    }

    CSF_AT(uint32_t, self, 0x104) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(handle), 0x4);
    CSF_AT(uint32_t, self, 0x108) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(handle), 0x8);

    void* commandState = CSF_AT(void*, self, 0x8);
    CSF_AT(uint32_t, commandState, 0x114) =
        CSF_AT(uint32_t, CSF_AT(void*, commandState, 0x118), 0x1E0);

    C_008342e8_PrepareEntryFrame(
        entry, nullptr, entryIndex,
        reinterpret_cast<int>(
            CSF_AT(uint8_t*, CSF_AT(void*, self, 0x8), 0x118) + 0x1C));

    AiMountTasks_CopyCommandHandle(
        CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x118),
        nullptr, reinterpret_cast<uint8_t*>(self) + 0x100,
        static_cast<uint32_t>(entryIndex));

    alignas(8) uint8_t positionBytes[0xC];
    auto* position = reinterpret_cast<uint32_t*>(positionBytes);
    C_008342e8_CopyControlPosition(entry, nullptr, entryIndex, position);

    CSF_AT(uint32_t, CSF_AT(void*, self, 0xC), 0x10C) = position[0];
    CSF_AT(int, CSF_AT(void*, self, 0xC), 0x110) = entryIndex;

    auto* target = CSF_AT(
        C_00833908*, CSF_AT(void*, self, 0x8), 0x118);
    C_0082dd78_StoreByteAndSetFlag(
        reinterpret_cast<C_0082dd78*>(target->field_004c), nullptr, 0);

    auto* flags = reinterpret_cast<uint8_t*>(
        C_008342e8_GetControlFlagsAddress(entry, nullptr, entryIndex));
    if ((*flags & 1) != 0)
    {
        target = CSF_AT(C_00833908*, CSF_AT(void*, self, 0x8), 0x118);
        void* controller = reinterpret_cast<void*>(target->field_0044);

        if (AiMountTasks_GetControlledObject(controller, nullptr))
        {
            void* controlledObject = reinterpret_cast<void*>(
                AiMountTasks_GetControlledObject(controller, nullptr));

            ((void (__fastcall*)(void*, void*))
                CSF_AT(void**, controlledObject, 0)[0x3C / 4])(
                    controlledObject, nullptr);
        }

        target = CSF_AT(C_00833908*, CSF_AT(void*, self, 0x8), 0x118);
        ((void (__fastcall*)(void*, void*, int))
            target->vtbl[0xB4 / 4])(target, nullptr, 0);
    }

    return C_0082f9f8_StartMovement_4DB140(
        self, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x9C));
}

HOOK(0x004A6F70, C_0082f9f8_DispatchToCommandSubobject, "auto");
