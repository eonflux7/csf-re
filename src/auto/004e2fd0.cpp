#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void, __fastcall, SetInnerObjectMode, 0x00492550, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(void, __fastcall, CallInnerObjectVirtualMethod110, 0x00492BC0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ActivatePendingState, 0x004E2D70, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));

static char __fastcall C_0083e248_UpdatePendingState(
    C_0083e248* self, void* /*edx*/, int handle, int /*unused*/)
{
    void* inner = reinterpret_cast<void*>(self->field_000c);
    self->field_0008 = 0;
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, inner, 0)[0x58 / 4])(inner, nullptr, handle);

    SetInnerObjectMode(reinterpret_cast<void*>(self->field_000c), nullptr, 2);
    CallInnerObjectVirtualMethod110(
        reinterpret_cast<void*>(self->field_000c), nullptr);

    const int state = static_cast<int32_t>(self->field_0044);
    if (state != 0)
    {
        if (state > 0 && state <= 2)
        {
            const char result = static_cast<char>(IsField1BCNonNegative(
                reinterpret_cast<void*>(self->field_000c), nullptr));
            if (!result)
                self->field_002c = 1;
            return result;
        }
        return static_cast<char>(state);
    }

    const bool forceActivation = self->field_0010 == 4;
    bool eligible = false;
    if (CSF_AT(int32_t, reinterpret_cast<void*>(handle), 0x08) >= 0)
    {
        const int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
        if (resolved != 0)
            eligible = CSF_AT(uint32_t, reinterpret_cast<void*>(resolved), 0x50) == 0;
    }

    int globalObject = GetGlobalInt();
    void* renderObject = reinterpret_cast<void*>(
        RenderHandle_FromObject(reinterpret_cast<void*>(globalObject), nullptr));
    const int dimensions = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, renderObject, 0)[0x5C / 4])(renderObject, nullptr);
    const uint32_t count = CSF_AT(uint32_t, reinterpret_cast<void*>(dimensions), 0x08);

    if (!eligible && !forceActivation)
    {
        globalObject = GetGlobalInt();
        renderObject = reinterpret_cast<void*>(
            RenderHandle_FromObject(reinterpret_cast<void*>(globalObject), nullptr));
        const int limit = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, renderObject, 0)[0x58 / 4])(renderObject, nullptr);

        if (2 * count >= static_cast<uint32_t>(limit))
            return static_cast<char>(limit);
    }

    const char pending = static_cast<char>(self->field_002c);
    if (pending)
        return pending;
    return ActivatePendingState(self, nullptr);
}

HOOK(0x004E2FD0, C_0083e248_UpdatePendingState, "auto");
