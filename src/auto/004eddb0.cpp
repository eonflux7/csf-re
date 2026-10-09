#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_InitializeObjectOptions, 0x006B2750, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneMissionSetup_RegisterObjectOptions, 0x006CEE30, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(const char*, g_psz_POBJ_PROYECTIL_GRANADA, 0x0089985C);
CSF_GLOBAL(const char*, g_psz_POBJ_PROYECTIL_DINAMITA_PEGAJOSA, 0x00899860);
CSF_GLOBAL(const char*, g_psz_POBJ_PROYECTIL_OBUS, 0x00899864);
CSF_GLOBAL(const char*, g_psz_POBJ_PROYECTIL_CUCHILLO, 0x00899868);
CSF_GLOBAL(const char*, g_psz_POBJ_PROYECTIL_BOTE_GAS, 0x0089986C);
CSF_GLOBAL(const char*, g_psz_POBJ_PROYECTIL_BOTE_HUMO, 0x00899870);
CSF_GLOBAL(const char*, g_psz_POBJ_PROYECTIL_COKTEL_MOLOTOV, 0x00899874);
CSF_GLOBAL(const char*, g_psz_POBJ_PROYECTIL_MINA_ANTITANQUE, 0x00899878);
CSF_GLOBAL(const char*, g_psz_POBJ_PAQUETE_CIGARRILLOS, 0x0089987C);
CSF_GLOBAL(const char*, g_psz_POBJ_MONEDA, 0x00899880);

static void SceneMissionSetup_LoadResource(int resourceId)
{
    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* manager = CSF_AT(void*, state, 0xE4);
    auto* loader = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
    void** managerVtable = CSF_AT(void**, manager, 0);
    int resource = SceneLoader_GetResourceById(loader + 0x22A4, nullptr, resourceId);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        managerVtable[0x1C / 4])(manager, nullptr, resource);
}

static int __fastcall SceneMissionSetup_RegisterProjectile(void* self, void* /*edx*/, int objectId)
{
    void* definitions = CSF_AT(void*, self, 0x1FC0);
    void* definition = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, definitions, 0)[8 / 4])(definitions, nullptr, objectId);

    int result = CSF_AT(int, definition, 0x74) - 6;
    const char* objectName;
    switch (result)
    {
    case 0:  objectName = g_psz_POBJ_PROYECTIL_GRANADA; break;
    case 1:  objectName = g_psz_POBJ_PROYECTIL_DINAMITA_PEGAJOSA; break;
    case 2:  objectName = g_psz_POBJ_PROYECTIL_OBUS; break;
    case 4:  objectName = g_psz_POBJ_PROYECTIL_CUCHILLO; break;
    case 10: objectName = g_psz_POBJ_PROYECTIL_BOTE_GAS; break;
    case 11: objectName = g_psz_POBJ_PROYECTIL_BOTE_HUMO; break;
    case 12: objectName = g_psz_POBJ_PROYECTIL_COKTEL_MOLOTOV; break;
    case 13: objectName = g_psz_POBJ_PROYECTIL_MINA_ANTITANQUE; break;
    case 18: objectName = g_psz_POBJ_PAQUETE_CIGARRILLOS; break;
    case 19: objectName = g_psz_POBJ_MONEDA; break;
    default: return result;
    }

    if (!objectName)
        return 0;

    void* objects = CSF_AT(void*, self, 0x1FC8);
    result = reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, objects, 0)[0xC / 4])(objects, nullptr, objectName);
    if (result == -1)
        return result;

    objects = CSF_AT(void*, self, 0x1FC8);
    auto* object = reinterpret_cast<std::uint8_t*>(
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, objects, 0)[8 / 4])(objects, nullptr, result));

    if (CSF_AT(std::uint32_t, object, 0x50))
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(global, nullptr));
        const void* resourceName;
        if (CSF_AT(std::uint32_t, object, 0x54) < 0x10u)
            resourceName = object + 0x40;
        else
            resourceName = CSF_AT(const void*, object, 0x40);

        reinterpret_cast<void(__fastcall*)(void*, void*, const void*, int)>(
            CSF_AT(void**, controller, 0)[0x10 / 4])(
                controller, nullptr, resourceName, 1);
    }

    // The constructor initializes the entire aggregate, including both strings.
    alignas(4) char options[0x60];
    SceneMissionSetup_InitializeObjectOptions(options, nullptr);
    CSF_AT(std::uint32_t, options, 0) = CSF_AT(std::uint32_t, object, 0x1F0);
    AssignStringSubstring(options + 0x4, nullptr, object + 0x1F4, 0, -1);
    AssignStringSubstring(options + 0x20, nullptr, object + 0x3C, 0, -1);
    CSF_AT(std::uint32_t, options, 0x3C) = CSF_AT(std::uint32_t, object, 0x130);
    CSF_AT(std::uint32_t, options, 0x40) = CSF_AT(std::uint32_t, object, 0x134);
    CSF_AT(std::uint32_t, options, 0x44) = CSF_AT(std::uint32_t, object, 0x138);
    CSF_AT(std::uint32_t, options, 0x48) = CSF_AT(std::uint32_t, object, 0x13C);
    CSF_AT(std::uint32_t, options, 0x4C) = CSF_AT(std::uint32_t, object, 0x140);
    CSF_AT(std::uint32_t, options, 0x50) = CSF_AT(std::uint32_t, object, 0x144);
    CSF_AT(std::uint32_t, options, 0x54) = CSF_AT(std::uint32_t, object, 0x220);
    CSF_AT(std::uint32_t, options, 0x58) = CSF_AT(std::uint32_t, object, 0x224);
    CSF_AT(std::uint32_t, options, 0x5C) = CSF_AT(std::uint32_t, object, 0x228);

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(global, nullptr));
    void* collection = reinterpret_cast<void*>(
        SceneLoader_GetWorldObjectCollection(accessor, nullptr));

    // Packet also identifies this direct-call address as a vtable binding.
    (void)CSF_VTBL(0x006CEE30);
    SceneMissionSetup_RegisterObjectOptions(collection, nullptr, options);

    switch (CSF_AT(std::uint32_t, object, 0xB4))
    {
    case 8:
        SceneMissionSetup_LoadResource(20);
        SceneMissionSetup_LoadResource(17);
        SceneMissionSetup_LoadResource(18);
        break;
    case 9:
        SceneMissionSetup_LoadResource(26);
        SceneMissionSetup_LoadResource(17);
        SceneMissionSetup_LoadResource(18);
        break;
    case 0xA:
        SceneMissionSetup_LoadResource(17);
        SceneMissionSetup_LoadResource(18);
        break;
    case 0xB:
        SceneMissionSetup_LoadResource(23);
        break;
    case 0xE:
    case 0xF:
        SceneMissionSetup_LoadResource(19);
        SceneMissionSetup_LoadResource(27);
        break;
    case 0x10:
        SceneMissionSetup_LoadResource(24);
        SceneMissionSetup_LoadResource(28);
        break;
    case 0x11:
        SceneMissionSetup_LoadResource(25);
        SceneMissionSetup_LoadResource(17);
        break;
    case 0x14:
        SceneMissionSetup_LoadResource(22);
        break;
    case 0x15:
        SceneMissionSetup_LoadResource(21);
        break;
    default:
        break;
    }

    return static_cast<int>(DestroySceneObjectOptionsStorage(options, nullptr));
}

HOOK(0x004EDDB0, SceneMissionSetup_RegisterProjectile, "auto");
