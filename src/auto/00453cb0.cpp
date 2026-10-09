#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Scene_ResetGlobalObjects, 0x0041E6F0, ());
CSF_FUNC(int, __fastcall, Scene_ResetSubsystem130, 0x0041EA60, (void* self, void* edx));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __cdecl, IsGlobal8AFC78NonZero, 0x0042D6E0, ());
CSF_FUNC(int, __fastcall, Hud_ResetWidgets, 0x0042E580, (void* self, void*));
CSF_FUNC(int, __fastcall, Scene_ResetSubsystemB8, 0x00443CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Scene_ResetSubsystemC0, 0x00444880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Scene_ClearEmbeddedState, 0x00445F90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Scene_ResetSubsystemB0, 0x00446210, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Scene_ResetSubsystemC4, 0x0044A250, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_44ACE0, 0x0044ACE0, ());
CSF_FUNC(int, __cdecl, IsGlobalFlagSet, 0x0044ACF0, ());
CSF_FUNC(int, __fastcall, Scene_ResetOptionalGlobal, 0x0044B230, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Scene_ResetSubsystemB4, 0x0044D3F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_RemoveObjectReference, 0x00453560, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Scene_ReleaseObjectReferences, 0x00453950, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_457A80, 0x00457A80, ());
CSF_FUNC(char, __fastcall, ResetGlobalValueObject, 0x00457AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Scene_ResetSubsystemBC, 0x0046A7A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Scene_ResetSubsystem184, 0x00475530, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Scene_ResetSubsystemAC, 0x004A00C0, (void* self, void* edx));
CSF_FUNC(uint32_t, __cdecl, Scene_ResetSharedState, 0x004AEB50, ());
CSF_FUNC(void, __fastcall, Scene_ResetSubsystem12C, 0x004DE820, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, Scene_ResetGlobalManager, 0x004E8CB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void Scene_ClearOwnedArray(void*& storage, uint32_t& capacity, uint32_t& count)
{
    void* block = storage;
    capacity = 0;
    count = 0;
    if (block)
    {
        Mem_Free(block);
        storage = nullptr;
    }
}

static char __fastcall CScene_ClearCurrentScene(
    CScene* self, void* /*edx*/, char releasePending)
{
    char result = releasePending;
    if (releasePending)
    {
        void* pending = CSF_AT(void*, self, 0x188);
        if (pending)
        {
            result = static_cast<char>(
                reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, uint32_t*)>(
                    CSF_AT(void**, pending, 0)[0x14 / 4])(
                        pending, nullptr,
                        &CSF_AT(uint32_t, self, 0x18C),
                        &CSF_AT(uint32_t, self, 0x190)));

            pending = CSF_AT(void*, self, 0x188);
            if (pending)
            {
                result = static_cast<char>(
                    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, pending, 0)[0])(
                            pending, nullptr, 1));
                CSF_AT(void*, self, 0x188) = nullptr;
            }
        }
    }

    if (CSF_AT(uint8_t, self, 0x0C))
    {
        CSF_AT(uint8_t, self, 0x144) = 0;
        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x134),
            CSF_AT(uint32_t, self, 0x138),
            CSF_AT(uint32_t, self, 0x13C));

        CSF_AT(uint8_t, self, 0x158) = 0;
        Scene_ClearEmbeddedState(reinterpret_cast<uint8_t*>(self) + 0x148, nullptr);
        CSF_AT(uint8_t, self, 0x16C) = 0;
        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x15C),
            CSF_AT(uint32_t, self, 0x160),
            CSF_AT(uint32_t, self, 0x164));

        for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x90); ++i)
        {
            void* entry = CSF_AT(void**, self, 0x88)[i];
            void* object = CSF_AT(void*, entry, 0x4C);
            void* component =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr);
            reinterpret_cast<void (__fastcall*)(void*, void*)>(
                CSF_AT(void**, component, 0)[0x20 / 4])(component, nullptr);
        }

        Scene_ReleaseObjectReferences(self, nullptr);
        for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x18); ++i)
        {
            void* object = CSF_AT(void**, self, 0x10)[i];
            if (object)
            {
                SceneLoader_RemoveObjectReference(
                    self, nullptr,
                    reinterpret_cast<int>(reinterpret_cast<uint8_t*>(object) + 0x1C));
            }
        }

        // The packet also labels this direct callee address as a vtable reference.
        (void)CSF_VTBL(0x00457AF0);
        ResetGlobalValueObject(reinterpret_cast<void*>(GetGlobalValue_457A80()), nullptr);
        Scene_ResetSharedState();
        Scene_ResetGlobalObjects();

        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x10),
            CSF_AT(uint32_t, self, 0x14),
            CSF_AT(uint32_t, self, 0x18));
        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x20),
            CSF_AT(uint32_t, self, 0x24),
            CSF_AT(uint32_t, self, 0x28));
        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x30),
            CSF_AT(uint32_t, self, 0x34),
            CSF_AT(uint32_t, self, 0x38));
        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x40),
            CSF_AT(uint32_t, self, 0x44),
            CSF_AT(uint32_t, self, 0x48));
        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x60),
            CSF_AT(uint32_t, self, 0x64),
            CSF_AT(uint32_t, self, 0x68));
        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x98),
            CSF_AT(uint32_t, self, 0x9C),
            CSF_AT(uint32_t, self, 0xA0));
        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x88),
            CSF_AT(uint32_t, self, 0x8C),
            CSF_AT(uint32_t, self, 0x90));
        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x74),
            CSF_AT(uint32_t, self, 0x78),
            CSF_AT(uint32_t, self, 0x7C));
        Scene_ClearOwnedArray(
            CSF_AT(void*, self, 0x50),
            CSF_AT(uint32_t, self, 0x54),
            CSF_AT(uint32_t, self, 0x58));

        uint8_t* item = CSF_AT(uint8_t*, self, 0xCC);
        if (item)
        {
            uint8_t* end = CSF_AT(uint8_t*, self, 0xD0);
            for (; item != end; item += 0x30)
            {
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, item, 0)[0])(item, nullptr, 0);
            }
            if (CSF_AT(void*, self, 0xCC))
                Mem_FreeDebug(CSF_AT(void*, self, 0xCC));
        }
        CSF_AT(void*, self, 0xCC) = nullptr;
        CSF_AT(void*, self, 0xD0) = nullptr;
        CSF_AT(void*, self, 0xD4) = nullptr;

        if (CSF_AT(void*, self, 0x100))
            Mem_FreeDebug(CSF_AT(void*, self, 0x100));
        CSF_AT(void*, self, 0x100) = nullptr;
        CSF_AT(void*, self, 0x104) = nullptr;
        CSF_AT(void*, self, 0x108) = nullptr;

        item = CSF_AT(uint8_t*, self, 0x110);
        if (item)
        {
            uint8_t* end = CSF_AT(uint8_t*, self, 0x114);
            for (; item != end; item += 0x18)
            {
                CSF_AT(void**, item, 0x0C) = CSF_VTBL(0x0082D410);
                CSF_AT(void**, item, 0) = CSF_VTBL(0x0082D410);
            }
            if (CSF_AT(void*, self, 0x110))
                Mem_FreeDebug(CSF_AT(void*, self, 0x110));
        }
        CSF_AT(void*, self, 0x110) = nullptr;
        CSF_AT(void*, self, 0x114) = nullptr;
        CSF_AT(void*, self, 0x118) = nullptr;

        if (CSF_AT(void*, self, 0x12C))
            Scene_ResetSubsystem12C(CSF_AT(void*, self, 0x12C), nullptr);
        Scene_ResetGlobalManager(reinterpret_cast<void*>(GetValue8B0554()), nullptr);
        Scene_ResetSubsystemBC(CSF_AT(void*, self, 0xBC), nullptr);
        Scene_ResetSubsystemC0(CSF_AT(void*, self, 0xC0), nullptr);
        Scene_ResetSubsystemAC(CSF_AT(void*, self, 0xAC), nullptr);
        Scene_ResetSubsystemB0(CSF_AT(void*, self, 0xB0), nullptr);
        Scene_ResetSubsystemB4(CSF_AT(void*, self, 0xB4), nullptr);
        Scene_ResetSubsystemB8(CSF_AT(void*, self, 0xB8), nullptr);
        Scene_ResetSubsystemC4(CSF_AT(void*, self, 0xC4), nullptr);
        Scene_ResetSubsystem130(CSF_AT(void*, self, 0x130), nullptr);
        Scene_ResetSubsystem184(CSF_AT(void*, self, 0x184), nullptr);

        if (static_cast<uint8_t>(IsGlobalFlagSet()))
            Scene_ResetOptionalGlobal(reinterpret_cast<void*>(GetGlobalValue_44ACE0()), nullptr);
        if (static_cast<uint8_t>(IsGlobal8AFC78NonZero()))
            Hud_ResetWidgets(reinterpret_cast<void*>(GetGlobal8afc78()), nullptr);

        void* block = CSF_AT(void*, self, 0x170);
        result = static_cast<char>(reinterpret_cast<uintptr_t>(block));
        CSF_AT(uint32_t, self, 0x174) = 0;
        CSF_AT(uint32_t, self, 0x178) = 0;
        if (block)
        {
            result = static_cast<char>(Mem_Free(block));
            CSF_AT(void*, self, 0x170) = nullptr;
        }
        CSF_AT(uint8_t, self, 0x0C) = 0;
    }
    return result;
}

HOOK(0x00453CB0, CScene_ClearCurrentScene, "auto");
