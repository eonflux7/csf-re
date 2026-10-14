#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int*, __fastcall, ProfileSavegames_GetScenarioCollection, 0x00515CE0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue_8B0574, 0x00518980, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, OnlineMenu_SetSharedText, 0x00521E30, (int a1));
CSF_FUNC(int, __fastcall, MenuScenarioSelect_AlignChildToReferencedPosition, 0x0052D850, (void* self, void*));
CSF_FUNC(void, __fastcall, CMenuScenarioSelect_RefreshState, 0x0052DF20, (CMenuScenarioSelect* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, CMenuScenarioSelect_UpdateScenarioList, 0x0052E620, (CMenuScenarioSelect* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreParser_FormatScenarioText, 0x00692660, (int outputString, int descriptionText, int count, int flag));
CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(int, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0, (void* self, void* edx, int text));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(uint32_t, g_dwAccountEditScreenState, 0x0089A5EC);

static uint32_t __fastcall CMenuScenarioSelect_InitializeScreen(
    CMenuScenarioSelect* self, void* /*edx*/,
    uint32_t selectionContext, int /*unused*/)
{
    alignas(8) uint8_t stringFrame[0x38];
    uint8_t* patternBytes = stringFrame;
    uint8_t* formattedBytes = stringFrame + 0x1C;

    if (!self->field_0018)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        ((void(__fastcall*)(void*, void*, CMenuScenarioSelect*))
            CSF_AT(void**, state, 0)[0x18 / 4])(state, nullptr, self);
        if (!self->field_0018)
            return 0;
    }

    self->vResetSelectionState();
    self->field_0014 = selectionContext;
    GetValue_8B0574();
    void* progress = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    MenuProgressInput_UpdateGlobalState(progress, nullptr);

    void* screen = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    screen = reinterpret_cast<void*>(self->field_0018);
    uint32_t child = ((uint32_t(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x0084174C, "Puntuacion"));
    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_0068 = child;

    child = ((uint32_t(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x0084173C, "NombreEscenario"));
    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_0064 = child;

    child = ((uint32_t(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x00841730, "Briefing"));
    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_006c = child;

    child = ((uint32_t(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x00841724, "BotonPlay"));
    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_0058 = child;

    child = ((uint32_t(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x00841718, "BotonLoad"));
    self->field_005c = child;
    if (child)
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        int enabled = CSF_AT(uint8_t, settings, 0xCC);
        void* button = reinterpret_cast<void*>(self->field_005c);
        ((void(__fastcall*)(void*, void*, int, int, int))
            CSF_AT(void**, button, 0)[0x68 / 4])(
                button, nullptr, enabled, 1, 1);
    }

    screen = reinterpret_cast<void*>(self->field_0018);
    child = ((uint32_t(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x0084170C, "BotonExtras"));
    self->field_0060 = child;
    if (child)
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        int enabled = CSF_AT(uint8_t, settings, 0xD1);
        void* button = reinterpret_cast<void*>(self->field_0060);
        ((void(__fastcall*)(void*, void*, int, int, int))
            CSF_AT(void**, button, 0)[0x68 / 4])(
                button, nullptr, enabled, 1, 1);
    }

    screen = reinterpret_cast<void*>(self->field_0018);
    void* profileLabel = ((void*(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x008416FC, "StaticProfile"));
    if (profileLabel)
    {
        void* manager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        void* profile = CSF_AT(void*, manager, 0x8);
        S_004f21d0& pattern =
            *reinterpret_cast<S_004f21d0*>(patternBytes);
        S_004f21d0_SetPattern(&pattern, nullptr, &g_byte_82E65C);

        CSF_AT(uint32_t, formattedBytes, 0x18) = 7;
        CSF_AT(int, formattedBytes, 0x14) = 0;
        CSF_AT(uint16_t, formattedBytes, 0x4) = 0;

        int* collection =
            ProfileSavegames_GetScenarioCollection(profile, nullptr);
        uint32_t begin = CSF_AT(uint32_t, collection, 0x4);
        const void* profileText;
        if (begin &&
            static_cast<int32_t>(
                CSF_AT(uint32_t, collection, 0x8) - begin) / 80)
        {
            void* name = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, profile, 0)[0x24 / 4])(profile, nullptr);
            if (CSF_AT(uint32_t, name, 0x18) < 8)
                profileText = static_cast<uint8_t*>(name) + 4;
            else
                profileText = CSF_AT(void*, name, 0x4);
        }
        else
        {
            uint32_t capacity = CSF_AT(uint32_t, patternBytes, 0x18);
            profileText = CSF_AT(void*, patternBytes, 0x4);
            if (capacity < 8)
                profileText = patternBytes + 4;
        }

        CoreParser_FormatScenarioText(
            reinterpret_cast<int>(formattedBytes),
            reinterpret_cast<int>(CSF_STR(0x008416F4, "pri7")),
            reinterpret_cast<int>(profileText), 1);

        uint32_t capacity = CSF_AT(uint32_t, formattedBytes, 0x18);
        const void* formattedText = CSF_AT(void*, formattedBytes, 0x4);
        if (capacity < 8)
            formattedText = formattedBytes + 4;
        C_00720DF0_ForwardCallToChildVtblSlot16(
            profileLabel, nullptr, reinterpret_cast<int>(formattedText));

        if (CSF_AT(uint32_t, formattedBytes, 0x18) >= 8)
        {
            void* allocation = CSF_AT(void*, formattedBytes, 0x4);
            if (allocation)
                Mem_FreeDebug(allocation);
        }

        uint32_t patternCapacity = CSF_AT(uint32_t, patternBytes, 0x18);
        CSF_AT(uint32_t, formattedBytes, 0x18) = 7;
        CSF_AT(int, formattedBytes, 0x14) = 0;
        CSF_AT(uint16_t, formattedBytes, 0x4) = 0;
        if (patternCapacity >= 8)
        {
            void* allocation = CSF_AT(void*, patternBytes, 0x4);
            if (allocation)
                Mem_FreeDebug(allocation);
        }
    }

    self->vSetBackButton(static_cast<int>(self->field_0018));
    MenuScenarioSelect_AlignChildToReferencedPosition(self, nullptr);
    CMenuScenarioSelect_UpdateScenarioList(self, nullptr);
    CMenuScenarioSelect_RefreshState(self, nullptr);

    void* onlineState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    uint32_t capacity = CSF_AT(uint32_t, onlineState, 0xF9C);
    uint8_t* sharedString = static_cast<uint8_t*>(onlineState) + 0xF84;
    void* sharedText;
    if (capacity < 0x10)
        sharedText = sharedString + 4;
    else
        sharedText = CSF_AT(void*, sharedString, 0x4);

    // The original supplies the getter result in ECX as well as the
    // single, callee-popped text argument on the stack.
    progress = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        OnlineMenu_SetSharedText)(
            progress, nullptr, reinterpret_cast<int>(sharedText));

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* stateChild = CSF_AT(void*, state, 0x10);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, stateChild, 0)[0x8 / 4])(stateChild, nullptr);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x60 / 4])(self, nullptr);

    if (g_dwAccountEditScreenState == 2)
    {
        screen = reinterpret_cast<void*>(self->field_0018);
        self->field_0038 = 1;
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, screen, 0)[0xA8 / 4])(screen, nullptr, 1);
    }

    screen = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, screen, 0)[0x260 / 4])(screen, nullptr, 1);
    return self->field_0018;
}

HOOK(0x0052EA40, CMenuScenarioSelect_InitializeScreen, "auto");
