#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(char, __cdecl, GameBddLoader_PrepareMenuResources, 0x004A0B10, (int a1));
CSF_FUNC(char, __cdecl, GameBddLoader_PrepareScenarioSelection, 0x004FDAD0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void, __fastcall, GlobalMenu_SetSelectedScenarioName, 0x00501990, (S_004ff0f0* self, void* , char* String1, char a3));
CSF_FUNC(int, __fastcall, GameBddLoader_ConstructFader, 0x0050C580, (int a1, int a2));
CSF_FUNC(int, __fastcall, GameBddLoader_ConstructFadeController, 0x0050D0A0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructMenuController, 0x0051B480, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructMenuState, 0x0051B4C0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructMenuEffects, 0x0051B600, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructMenuListener, 0x0051B8F0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructMenuTransition, 0x0051BF20, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, RtRandomSeed, 0x00586860, (int a1));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint8_t*, __cdecl, SceneLoader_CreateLoadTimer, 0x0069CDC0, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructMenuEventSource, 0x00774120, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static uint32_t* __fastcall CMenuManager_Construct(
    CMenuManager* self, void* /*edx*/, int guiRoot, int /*unused*/, char* scenarioName)
{
    self->vtbl = CSF_VTBL(0x008405E0);
    self->field_00b8 = 0;
    self->field_00bc = 0;
    self->field_00c0 = 0;
    self->field_00c4 = 1;
    self->m_pScreenGame = 0;
    self->m_pScreenLoading = 0;
    self->m_pScreenGameMenu = 0;
    self->m_pScreenProfiles = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->m_pScreenMainMenu = 0;
    self->field_0034 = 0;
    self->m_pScreenTactic = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->m_pScreenGameOptionsInGame = 0;
    self->m_pScreenGameOptions = 0;
    self->field_0050 = 0;
    self->m_pScreenIntro = 0;
    self->field_0058 = 0;
    self->field_00ec = 0;
    self->m_pFader = 0;
    self->field_006c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_00d4 = 0;
    self->field_00d8 = 0;
    self->field_00dc = 0;
    self->field_00e0 = 0;
    self->field_00e4 = 0;
    self->field_00e8 = 0;
    self->field_007c = 0;
    self->m_pGuiRoot = guiRoot;
    self->field_0090 = reinterpret_cast<uint32_t>(SceneLoader_CreateLoadTimer());
    self->field_0014 = 0;
    self->field_0084 = 1;
    self->field_0088 = 0;
    self->field_0070 = 0;
    self->field_0089 = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_00a8 = 0;
    self->field_00a9 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00cc = -1;
    self->field_00d0 = 0;
    self->field_00f0 = 0;
    self->field_0100 = 0;
    self->field_00c8 = 0;
    self->field_0078 = 0;

    RtRandomSeed(123456);
    self->field_00c4 = 123456;

    void* display = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, display, 0)[0x18 / 4])(
            display, nullptr);
    int width = CSF_AT(int, dimensions, 0);

    display = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, display, 0)[0x18 / 4])(
            display, nullptr);
    int height = CSF_AT(int, dimensions, 0x4);
    void* root = reinterpret_cast<void*>(self->m_pGuiRoot);
    ((void (__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, root, 0)[0x2C / 4])(root, nullptr, 0, 0, width, height);

    void* timer = reinterpret_cast<void*>(self->field_0090);
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, timer, 0)[0x8 / 4])(
        timer, nullptr);

    auto* gameState = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    const char* controlName;
    if (CSF_AT(uint32_t, gameState, 0x1814) < 0x10)
        controlName = reinterpret_cast<const char*>(gameState + 6140 + 4);
    else
        controlName = CSF_AT(const char*, gameState, 0x1800);

    void* controls =
        MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    self->field_008c =
        ((int (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, controls, 0)[0x4C / 4])(controls, nullptr, controlName);

    GameBddLoader_PrepareMenuResources(0);

    void* eventSource = Mem_Alloc(24);
    if (eventSource)
    {
        GameBddLoader_ConstructMenuEventSource(eventSource, nullptr);
        CSF_AT(void**, eventSource, 0) = CSF_VTBL(0x00840704);
        CSF_AT(uint32_t, eventSource, 0xC) = 0;
        CSF_AT(uint32_t, eventSource, 0x10) = 0;
        CSF_AT(uint32_t, eventSource, 0x14) = 0;
    }
    self->field_00d8 = reinterpret_cast<uint32_t>(eventSource);

    void* storage = Mem_Alloc(12);
    self->field_00dc = storage
        ? reinterpret_cast<uint32_t>(GameBddLoader_ConstructMenuListener(
              storage, nullptr, self->field_00d8))
        : 0;

    storage = Mem_Alloc(44);
    self->field_00d4 = storage
        ? reinterpret_cast<uint32_t>(GameBddLoader_ConstructMenuController(
              storage, nullptr, self->field_00d8))
        : 0;

    storage = Mem_Alloc(8);
    self->field_00e0 = storage
        ? reinterpret_cast<uint32_t>(GameBddLoader_ConstructMenuState(storage, nullptr))
        : 0;

    storage = Mem_Alloc(24);
    self->field_00e4 = storage
        ? reinterpret_cast<uint32_t>(GameBddLoader_ConstructMenuTransition(storage, nullptr))
        : 0;

    storage = Mem_Alloc(88);
    self->field_00e8 = storage
        ? reinterpret_cast<uint32_t>(GameBddLoader_ConstructMenuEffects(storage, nullptr))
        : 0;

    GameBddLoader_PrepareScenarioSelection();

    alignas(8) uint8_t scenarioStringBytes[0x1C];
    void* scenarioString = scenarioStringBytes;
    CSF_AT(uint32_t, scenarioString, 0x18) = 15;
    CSF_AT(uint32_t, scenarioString, 0x14) = 0;
    CSF_AT(uint8_t, scenarioString, 0x4) = 0;
    AssignStringRange_429B60(
        scenarioString, nullptr, scenarioName,
        static_cast<unsigned int>(std::strlen(scenarioName)));
    bool hasScenario = CompareStringSubstring(
        scenarioString, nullptr, 0, CSF_AT(uint32_t, scenarioString, 0x14),
        reinterpret_cast<const char*>(&g_byte_82E65C), 0) != 0;

    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8B0568()), 0xB4) =
        hasScenario;
    GlobalMenu_SetSelectedScenarioName(
        reinterpret_cast<S_004ff0f0*>(GetGlobalValue_8B0568()),
        nullptr, scenarioName, 1);

    self->field_00f8 = -1;
    self->field_00fc = 0;
    self->field_0100 = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x90C) = 0;

    storage = Mem_Alloc(76);
    self->m_pFader = storage
        ? GameBddLoader_ConstructFader(reinterpret_cast<int>(storage), 0)
        : 0;

    storage = Mem_Alloc(44);
    // The packet also identifies this constructor address as a vtable reference.
    (void)CSF_VTBL(0x0050D0A0);
    self->field_006c = storage
        ? GameBddLoader_ConstructFadeController(storage, nullptr)
        : 0;

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0108 = 0;
    self->field_0074 = 0;
    self->field_0104 = 0;
    self->field_0080 = 0;
    self->field_010c = 0;

    if (CSF_AT(uint32_t, scenarioString, 0x18) >= 0x10 &&
        CSF_AT(uint32_t, scenarioString, 0x4))
    {
        Mem_FreeDebug(CSF_AT(void*, scenarioString, 0x4));
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00510860, CMenuManager_Construct, "auto");
