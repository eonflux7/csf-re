#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ProcessContextStageHandlers, 0x006CA6F0, (void* self, void* , int contextAddress));

static const auto g_ContextStageVtable = CSF_VTBL(0x006CA6F0);

static int __fastcall S_006ca6f0_ProcessContext(
    S_006ca6f0* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);

    bool blocked = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* handler = CSF_AT(void*, context, 4);
        blocked = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, handler, 0)[128 / 4])(
                handler, nullptr, 0, contextAddress);
    }

    const uint32_t savedStage = CSF_AT(uint32_t, context, 28);
    CSF_AT(uint8_t, context, 0) = blocked;
    ProcessContextStageHandlers(self, nullptr, contextAddress);

    bool canProcess = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 28) = 0;
    if (canProcess)
    {
        void* handler = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 21) = 0;
        CSF_AT(uint8_t, context, 22) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, handler, 0)[84 / 4])(
                    handler, nullptr, reinterpret_cast<uint8_t*>(self) + 28,
                    contextAddress) == -1;
    }

    canProcess = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 28) = 0;
    if (canProcess)
    {
        void* handler = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 21) = 0;
        CSF_AT(uint8_t, context, 22) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, handler, 0)[84 / 4])(
                    handler, nullptr, reinterpret_cast<uint8_t*>(self) + 32,
                    contextAddress) == -1;
    }

    canProcess = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 28) = 0;
    if (canProcess)
    {
        void* handler = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 21) = 0;
        CSF_AT(uint8_t, context, 22) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, handler, 0)[84 / 4])(
                    handler, nullptr, reinterpret_cast<uint8_t*>(self) + 36,
                    contextAddress) == -1;
    }

    canProcess = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 28) = 0;
    if (canProcess)
    {
        void* handler = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 21) = 0;
        CSF_AT(uint8_t, context, 22) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, handler, 0)[84 / 4])(
                    handler, nullptr, reinterpret_cast<uint8_t*>(self) + 40,
                    contextAddress) == -1;
    }

    if (CSF_AT(uint8_t, context, 20) == 0 &&
        CSF_AT(uint8_t, context, 0) == 0)
    {
        void* activeObject = reinterpret_cast<void*>(self->field_002c);
        if (activeObject)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, activeObject, 0)[28 / 4])(
                    activeObject, nullptr);

            activeObject = reinterpret_cast<void*>(self->field_002c);
            contextAddress = 0;
            reinterpret_cast<void(__fastcall*)(void*, void*, int*, int)>(
                CSF_AT(void**, activeObject, 0)[24 / 4])(
                    activeObject, nullptr, &contextAddress, 0);
            self->field_0030 = 0;
        }
    }

    CSF_AT(uint32_t, context, 28) = savedStage;
    if (CSF_AT(uint8_t, context, 0) != 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    void* handler = CSF_AT(void*, context, 4);
    // Only the low byte controls the branch; the full return is forwarded.
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, handler, 0)[124 / 4])(
                handler, nullptr, reinterpret_cast<int>(context));

    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0) = 0;
    return result;
}

HOOK(0x006F13E0, S_006ca6f0_ProcessContext, "auto");
