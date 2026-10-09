#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, DispatchSceneObjectEvent, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(uint32_t*, __fastcall, ConstructSceneObjectController, 0x004A4280, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, ApplySceneObjectOptions, 0x004E9090, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, CreateSceneObjectModel, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(void, __fastcall, SetSceneObjectController, 0x0055D860, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Local scene-object options block (the original reserves 0xB8 bytes of stack for it):
//   +0x00 flags, +0x04 internal storage, +0x68.. placement data,
//   +0x78 embedded string (buffer +0x7C, size +0x8C, capacity +0x90), +0x94.. extra params.
static uint32_t* __fastcall S_004e9090_SetupSceneObjectFromDefinition(S_004e9090* self, void* /*edx*/,
                                                                      C_00833908* sceneObject, int definitionId)
{
    void* definitionTable = reinterpret_cast<void*>(self->field_1fc8);
    uint8_t* definition = ((uint8_t*(__fastcall*)(void*, void*, int))CSF_AT(void**, definitionTable, 0)[2])(
        definitionTable, nullptr, definitionId);

    int model = CreateSceneObjectModel(self, nullptr, definitionId, 0);
    if (model)
    {
        C_00833908_SetDoorStateFlags(sceneObject, nullptr, (uint32_t)model, 1);

        alignas(4) uint8_t options[0xB8];
        InitializeSceneObjectOptions(options, nullptr);

        uint32_t flags = CSF_AT(uint32_t, options, 0x0);
        if (CSF_AT(uint8_t, definition, 0x159))
            flags |= 0x400;
        else
            flags &= 0xFFFFFBFF;
        flags |= 0x40000;
        if (CSF_AT(uint8_t, definition, 0x15A))
            flags &= 0xFFFFFFDF;
        else
            flags |= 0x20;
        if (CSF_AT(uint8_t, definition, 0x15B))
            flags |= 0x4000;
        else
            flags &= 0xFFFFBFFF;
        CSF_AT(uint32_t, options, 0x0) = flags;

        CSF_AT(uint32_t, options, 0x68) = CSF_AT(uint32_t, definition, 0xCC);
        CSF_AT(uint32_t, options, 0x6C) = CSF_AT(uint32_t, definition, 0x160);
        CSF_AT(uint32_t, options, 0x70) = CSF_AT(uint32_t, definition, 0x164);
        CSF_AT(uint32_t, options, 0x74) = CSF_AT(uint32_t, definition, 0x168);

        AssignStringSubstring(options + 0x78, nullptr, definition + 0x16C, 0, (std::uint32_t)-1);

        CSF_AT(int, options, 0x94) = CSF_AT(int, definition, 0x188) - 1;
        CSF_AT(uint32_t, options, 0x98) = CSF_AT(uint32_t, definition, 0x18C);
        CSF_AT(uint32_t, options, 0x9C) = CSF_AT(uint32_t, definition, 0x190);
        CSF_AT(uint32_t, options, 0xA0) = CSF_AT(uint32_t, definition, 0x194);
        CSF_AT(uint32_t, options, 0xA4) = CSF_AT(uint32_t, definition, 0xD0);

        ApplySceneObjectOptions(self, nullptr, CSF_AT(int, definition, 0x0), (int)options);

        ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, model, 0)[4])((void*)model, nullptr, options);

        void* controllerMemory = Mem_Alloc(0x7C);
        void* controller = nullptr;
        if (controllerMemory)
            controller = ConstructSceneObjectController(controllerMemory, nullptr, (int)sceneObject);
        SetSceneObjectController(sceneObject, nullptr, controller);

        // inline destructor of the embedded string
        if (CSF_AT(uint32_t, options, 0x90) >= 0x10 && CSF_AT(void*, options, 0x7C))
            Mem_FreeDebug(CSF_AT(void*, options, 0x7C));
        CSF_AT(uint32_t, options, 0x90) = 0xF;
        CSF_AT(uint32_t, options, 0x8C) = 0;
        CSF_AT(uint8_t, options, 0x7C) = 0;

        DestroySceneObjectOptionsStorage(options + 0x4, nullptr);
    }

    const void* eventDescriptor = (const void*)CSF_ADDR_DATA(0x0083ED08);
    return DispatchSceneObjectEvent(sceneObject, nullptr, 16, (int)&eventDescriptor);
}

HOOK(0x004ED030, S_004e9090_SetupSceneObjectFromDefinition, "auto");
