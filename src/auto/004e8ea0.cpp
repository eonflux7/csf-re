#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00833908_Construct, 0x00490940, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_00833a48_Construct, 0x00491EC0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_00833b88_Construct, 0x00492F60, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_00833cf8_Construct, 0x00493740, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_00833ee8_Construct, 0x0049C3D0, (C_00833ee8* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, C_00834050_Construct, 0x0049D280, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_00834198_Construct, 0x0049E280, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_008342e8_Construct, 0x0049F410, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneMissionSetup_ConstructObjectAttachment, 0x004A1D30, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, IsStateCodeAllowed, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));
CSF_FUNC(int, __cdecl, SceneMissionSetup_IsSpecialObjectType, 0x00755940, (int a1));

static uint32_t* __stdcall SceneMissionSetup_CreateObject(int context, int objectType)
{
    if (!objectType)
    {
        auto* storage = static_cast<C_00833ee8*>(Mem_Alloc(1052));
        uint32_t* object = storage
            ? C_00833ee8_Construct(storage, nullptr, context, 0)
            : nullptr;

        void* attachment = Mem_Alloc(36);
        SceneMissionSetup_ConstructObjectAttachment(
            attachment, nullptr, reinterpret_cast<int>(object));
        CSF_AT(void*, object, 0x3E8) = attachment;
        return object;
    }

    if (static_cast<uint8_t>(IsStateCodeAllowed(objectType)))
    {
        void* storage = Mem_Alloc(524);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t*>(
            C_00833b88_Construct(storage, nullptr, context, objectType));
    }

    if (static_cast<uint8_t>(IsResolvedEntryActive(objectType)))
    {
        void* storage = Mem_Alloc(432);
        if (!storage)
            return nullptr;
        return C_008342e8_Construct(storage, nullptr, context, objectType);
    }

    if (static_cast<uint8_t>(Scene_IsObjectRemovable(objectType)))
    {
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            void* storage = Mem_Alloc(552);
            if (!storage)
                return nullptr;
            return C_00834198_Construct(storage, nullptr, context, objectType);
        }
        else
        {
            void* storage = Mem_Alloc(440);
            if (!storage)
                return nullptr;
            return reinterpret_cast<uint32_t*>(
                C_00834050_Construct(storage, nullptr, context, objectType));
        }
    }

    if (static_cast<uint8_t>(SceneMissionSetup_IsSpecialObjectType(objectType)))
    {
        void* storage = Mem_Alloc(404);
        if (!storage)
            return nullptr;
        return C_00833cf8_Construct(storage, nullptr, context, objectType);
    }

    switch (objectType)
    {
    case 12:
    case 13:
    case 26:
    case 27:
    case 29:
    case 32:
    case 34:
    {
        void* storage = Mem_Alloc(388);
        if (!storage)
            return nullptr;
        return C_00833908_Construct(storage, nullptr, context, objectType);
    }
    case 28:
    {
        void* storage = Mem_Alloc(388);
        if (!storage)
            return nullptr;
        return C_00833a48_Construct(storage, nullptr, context, objectType);
    }
    default:
        return nullptr;
    }
}

HOOK(0x004E8EA0, SceneMissionSetup_CreateObject, "auto");
