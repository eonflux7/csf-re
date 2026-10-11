#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(int, __cdecl, GameBddLoader_CleanupMenuServices, 0x004A09A0, ());
CSF_FUNC(int, __cdecl, GameBddLoader_CleanupMenuAssets, 0x004FDBC0, ());
CSF_FUNC(int, __fastcall, FreeBufferAndClear, 0x0050CA20, (int* self, void*));
CSF_FUNC(int, __cdecl, GameBddLoader_CleanupMenuScreens, 0x00518990, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

static int* __fastcall CMenuManager_Destroy(CMenuManager* self, void* /*edx*/)
{
    int* buffer = reinterpret_cast<int*>(self->field_006c);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008405E0);

    if (buffer)
    {
        FreeBufferAndClear(buffer, nullptr);
        Mem_Free(buffer);
        self->field_006c = 0;
    }

    void* object = reinterpret_cast<void*>(self->m_pFader);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->m_pFader = 0;
    }

    GameBddLoader_CleanupMenuScreens();
    GameBddLoader_CleanupMenuAssets();

    object = reinterpret_cast<void*>(self->field_00e0);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_00e0 = 0;
    }

    object = reinterpret_cast<void*>(self->field_00d4);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_00d4 = 0;
    }

    object = reinterpret_cast<void*>(self->field_00dc);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_00dc = 0;
    }

    object = reinterpret_cast<void*>(self->field_00d8);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_00d8 = 0;
    }

    object = reinterpret_cast<void*>(self->field_00e4);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_00e4 = 0;
    }

    object = reinterpret_cast<void*>(self->field_00e8);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_00e8 = 0;
    }

    object = reinterpret_cast<void*>(self->field_0090);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0090 = 0;
    }

    object = reinterpret_cast<void*>(self->field_00b0);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_00b0 = 0;
    }

    object = reinterpret_cast<void*>(self->field_00ac);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_00ac = 0;
    }

    object = reinterpret_cast<void*>(self->field_007c);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_007c = 0;
    }

    C_00840cf8* guiRoot = reinterpret_cast<C_00840cf8*>(self->m_pGuiRoot);
    if (guiRoot)
    {
        guiRoot->vSlot0(1);
        self->m_pGuiRoot = 0;
    }

    GameBddLoader_CleanupMenuServices();

    object = reinterpret_cast<void*>(self->field_008c);
    if (object)
    {
        ReleaseObjectReference(object, nullptr);
        self->field_008c = 0;
    }

    int* result = reinterpret_cast<int*>(self->field_00b8);
    if (result)
    {
        MissionScript_DestroyElementRange(
            result, CSF_AT(int*, self, 0xBC));
        result = reinterpret_cast<int*>(self->field_00b8);
        if (result)
            result = reinterpret_cast<int*>(Mem_FreeDebug(result));
    }

    self->field_00b8 = 0;
    CSF_AT(uint32_t, self, 0xBC) = 0;
    CSF_AT(uint32_t, self, 0xC0) = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008315F8);
    return result;
}

HOOK(0x0050F540, CMenuManager_Destroy, "auto");
