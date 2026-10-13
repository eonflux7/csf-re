#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(char, __fastcall, C_0082dd78_StoreByteAndSetFlag, 0x0040FD20, (C_0082dd78* self, void* , char a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, AiMountTasks_CopyCommandHandle, 0x00492AE0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, IsEntryFrameBlocked, 0x0049ED50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(uint32_t*, __fastcall, AiMountTasks_CopyEntryFrame, 0x0049EDC0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovement_4DB140, 0x004DB140, (C_0082f9f8* self, void* , int movement));

static int __fastcall C_0082f9f8_DispatchToCommandSubobject(
    C_0082f9f8* self, void* /*edx*/, int handle, int entryIndex)
{
    void* control = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));

    if (!control || IsEntryFrameBlocked(control, nullptr, entryIndex))
    {
        return C_00833908_GetTargetName(
            CSF_AT(C_00833908*, CSF_AT(void*, self, 0x8), 0x118),
            nullptr);
    }

    CSF_AT(uint32_t, self, 0x104) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(handle), 0x4);
    CSF_AT(uint32_t, self, 0x108) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(handle), 0x8);

    void* taskContext = CSF_AT(void*, self, 0x8);
    void* command = CSF_AT(void*, taskContext, 0x118);
    CSF_AT(uint32_t, taskContext, 0x114) =
        CSF_AT(uint32_t, command, 0x1E0);

    AiMountTasks_CopyEntryFrame(
        control, nullptr, entryIndex,
        reinterpret_cast<int>(
            static_cast<uint8_t*>(
                CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x118)) + 0x1C));

    AiMountTasks_CopyCommandHandle(
        CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x118),
        nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x100),
        entryIndex);

    alignas(8) uint8_t positionBytes[0xC];
    C_008342e8_CopyControlPosition(
        control, nullptr, entryIndex,
        reinterpret_cast<uint32_t*>(positionBytes));

    CSF_AT(uint32_t, CSF_AT(void*, self, 0xC), 0x10C) =
        CSF_AT(uint32_t, positionBytes, 0);
    CSF_AT(int, CSF_AT(void*, self, 0xC), 0x110) = entryIndex;

    C_0082dd78_StoreByteAndSetFlag(
        CSF_AT(C_0082dd78*,
            CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x118), 0x4C),
        nullptr, 0);

    const uint8_t* flags = reinterpret_cast<const uint8_t*>(
        C_008342e8_GetControlFlagsAddress(control, nullptr, entryIndex));
    if ((*flags & 1) != 0)
    {
        void* controller = CSF_AT(
            void*, CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x118), 0x44);

        if (((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, controller, 0)[216 / 4])(controller, nullptr))
        {
            void* activeObject = ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, controller, 0)[216 / 4])(controller, nullptr);

            ((void (__fastcall*)(void*, void*))
                CSF_AT(void**, activeObject, 0)[60 / 4])(activeObject, nullptr);
        }

        void* currentCommand =
            CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x118);
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, currentCommand, 0)[180 / 4])(
                currentCommand, nullptr, 0);
    }

    return C_0082f9f8_StartMovement_4DB140(
        self, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x9C));
}

HOOK(0x004A6F70, C_0082f9f8_DispatchToCommandSubobject, "auto");
