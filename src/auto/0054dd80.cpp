#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_InitGlobalRequest, 0x00405A00, (S_00526d40* self, void*));
CSF_FUNC(int, __cdecl, SerializeObjectRefWithFlags, 0x00405BC0, (int a1, S_00691860* a2, int a3, char a4, char a5));
CSF_FUNC(uint32_t*, __fastcall, C_0082daf4_ConstructEntry, 0x0040C330, (C_0082daf4* self, void* , int a2, int a3));
CSF_FUNC(int, __cdecl, RecordStateAndDispatchOnce, 0x00410700, (void* self, int a2, int a3, char a4, char a5));
CSF_FUNC(S_00691860*, __cdecl, SerializeStringWithFlags, 0x00413540, (S_00691860* a2, int a1, uint32_t a3, char a4, char a5));
CSF_FUNC(std::uint32_t*, __fastcall, C_00831414_Construct, 0x00467AD0, (C_00831414* self, void* , int context));
CSF_FUNC(int, __fastcall, SceneFallbackLookup, 0x004686A0, (S_004686a0* self, void* , int a2));
CSF_FUNC(uint32_t*, __cdecl, WeaponFirearms_CreateActionEntry, 0x00475A40, (int type, int a2, int a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(unsigned int, __fastcall, S_004e8e00_SetIndexedValue, 0x004EE630, (S_004e8e00* self, void* , int index, int stateCode));
CSF_FUNC(C_00833908*, __fastcall, Scene_ResolveObjectAndInit, 0x004EEEB0, (S_004e9090* self, void* , int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DestroyCharacterRecord, 0x00511040, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void*, __fastcall, SoundManager_CopyPlaybackParams, 0x00521FA0, (void* self, void* , void* src));
CSF_FUNC(int, __fastcall, S_00526d40_SerializeSoundProperties, 0x00527AF0, (S_00526d40* self, void* , S_00691860* archive));
CSF_FUNC(C_008411a0*, __fastcall, C_008411a0_InitDefaults, 0x00528020, (C_008411a0* self, void*));
CSF_FUNC(uint32_t*, __fastcall, AudioSoundManager_CreateSoundInstance, 0x00528430, (void* self, void* , uint32_t* definition));
CSF_FUNC(char*, __fastcall, TypeRegistry_CopyCharacterDefinition, 0x0054DA80, (char* self, void* , int sourceAddress));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static C_00833908* __stdcall TypeRegistry_ResolveSerializedObject(
    int type, S_00691860* archive)
{
    switch (type)
    {
    case 11:
    {
        const int scene = GetValue8B0554();
        return scene
            ? reinterpret_cast<C_00833908*>(scene + 8124)
            : nullptr;
    }

    case 12:
    {
        alignas(8) uint8_t referenceBytes[0xC];
        CSF_AT(int, referenceBytes, 0x4) = -1;
        CSF_AT(int, referenceBytes, 0x8) = -1;
        type = -1;
        CSF_AT(void**, referenceBytes, 0) = CSF_VTBL(0x0082D448);

        SerializeStringWithFlags(
            archive, reinterpret_cast<int>(referenceBytes), 0, 0, 0);
        RecordStateAndDispatchOnce(
            archive, reinterpret_cast<int>(&type), 0, 0, 0);
        if (archive->pad_0000[0])
            return nullptr;

        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        void* definitions = CSF_AT(void*, state, 0xDC);
        const int source =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, definitions, 0)[0x8 / 4])(
                    definitions, nullptr, type);

        alignas(8) uint8_t characterBytes[0x22C];
        TypeRegistry_CopyCharacterDefinition(
            reinterpret_cast<char*>(characterBytes), nullptr, source);

        const int variant = CSF_AT(int, referenceBytes, 0x4);
        const int character = CSF_AT(int, characterBytes, 0);
        auto* scene = reinterpret_cast<S_004e9090*>(GetValue8B0554());
        C_00833908* result =
            Scene_ResolveObjectAndInit(scene, nullptr, character, variant);
        GameBddLoader_DestroyCharacterRecord(characterBytes, nullptr);
        return result;
    }

    case 14:
        return g_pCurrentScene
            ? reinterpret_cast<C_00833908*>(
                static_cast<uint8_t*>(g_pCurrentScene) + 4)
            : nullptr;

    case 15:
    {
        auto* entry =
            reinterpret_cast<C_0082daf4*>(CoreMemory_AllocUntagged(44));
        if (!entry)
            return nullptr;
        return reinterpret_cast<C_00833908*>(
            C_0082daf4_ConstructEntry(entry, nullptr, 0, -1));
    }

    case 16:
    case 17:
    case 18:
    {
        int index = -1;
        int objectRef = 0;
        RecordStateAndDispatchOnce(
            archive, reinterpret_cast<int>(&index), 0, 0, 0);
        SerializeObjectRefWithFlags(
            reinterpret_cast<int>(archive),
            reinterpret_cast<S_00691860*>(&objectRef), 0, 0, 1);
        if (archive->pad_0000[0] || !objectRef)
            return nullptr;

        uint32_t* entry = nullptr;
        switch (type)
        {
        case 16:
            entry = WeaponFirearms_CreateActionEntry(2, index, objectRef);
            break;
        case 17:
            entry = WeaponFirearms_CreateActionEntry(1, index, objectRef);
            break;
        case 18:
            entry = WeaponFirearms_CreateActionEntry(3, index, objectRef);
            break;
        }

        const int resolvedIndex = index;
        auto* scene = reinterpret_cast<S_004e8e00*>(GetValue8B0554());
        S_004e8e00_SetIndexedValue(scene, nullptr, resolvedIndex, -1);
        return reinterpret_cast<C_00833908*>(entry);
    }

    case 19:
    {
        auto* result =
            reinterpret_cast<C_00833908*>(CoreMemory_AllocUntagged(12));
        if (!result)
            return nullptr;
        result->field_0004 = 0;
        result->vtbl = CSF_VTBL(0x008313F8);
        CSF_AT(int, result, 0x8) = -1;
        return result;
    }

    case 20:
    {
        type = -1;
        RecordStateAndDispatchOnce(
            archive, reinterpret_cast<int>(&type), 0, 0, 0);
        if (archive->pad_0000[0] || type == -1)
            return nullptr;

        void* scene = g_pCurrentScene;
        void* overrideContext = CSF_AT(void*, scene, 0xC0);
        S_004686a0* lookup = CSF_AT(uint8_t, overrideContext, 0x8)
            ? CSF_AT(S_004686a0*, overrideContext, 0x4)
            : CSF_AT(S_004686a0*, scene, 0xBC);
        const int context = SceneFallbackLookup(lookup, nullptr, type);
        SceneFallbackLookup(
            CSF_AT(S_004686a0*, g_pCurrentScene, 0xBC), nullptr, type);

        auto* object =
            reinterpret_cast<C_00831414*>(CoreMemory_AllocUntagged(132));
        if (!object)
            return nullptr;
        return reinterpret_cast<C_00833908*>(
            C_00831414_Construct(object, nullptr, context));
    }

    case 21:
    {
        alignas(8) uint8_t requestBytes[0x38];
        auto& request = *reinterpret_cast<S_00526d40*>(requestBytes);
        S_00526d40_InitGlobalRequest(&request, nullptr);

        const bool reading = archive->pad_0000[0] == 0;
        archive[1].field_0004 = 0;
        if (!reading)
            return nullptr;
        reinterpret_cast<uint8_t*>(&archive->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&archive->field_0014)[2] = 0;
        S_00526d40_SerializeSoundProperties(&request, nullptr, archive);
        if (archive->pad_0000[0])
            return nullptr;

        void* state = reinterpret_cast<void*>(GetGlobalValue_521BB0());
        void* manager = CSF_AT(void*, state, 0x40);
        return reinterpret_cast<C_00833908*>(
            AudioSoundManager_CreateSoundInstance(
                manager, nullptr, reinterpret_cast<uint32_t*>(requestBytes)));
    }

    case 22:
    {
        alignas(8) uint8_t requestBytes[0x38];
        auto& request = *reinterpret_cast<S_00526d40*>(requestBytes);
        S_00526d40_InitGlobalRequest(&request, nullptr);

        const bool reading = archive->pad_0000[0] == 0;
        archive[1].field_0004 = 0;
        if (!reading)
            return nullptr;
        reinterpret_cast<uint8_t*>(&archive->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&archive->field_0014)[2] = 0;
        S_00526d40_SerializeSoundProperties(&request, nullptr, archive);
        if (archive->pad_0000[0])
            return nullptr;

        auto* allocation =
            reinterpret_cast<C_008411a0*>(CoreMemory_AllocUntagged(100));
        C_008411a0* sound = allocation
            ? C_008411a0_InitDefaults(allocation, nullptr)
            : nullptr;
        SoundManager_CopyPlaybackParams(
            reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(sound) + 4),
            nullptr, &request);
        return reinterpret_cast<C_00833908*>(sound);
    }

    default:
        return nullptr;
    }
}

HOOK(0x0054DD80, TypeRegistry_ResolveSerializedObject, "auto");
