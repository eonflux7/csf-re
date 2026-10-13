#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuProgressInput_GetEntryCount, 0x0052BFB0, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpOnline_ScrollViewToTarget, 0x00542CD0, (CMenuMpOnline* self, void*));
CSF_FUNC(int, __cdecl, CoreParser_ResolveEntryByKey, 0x00692440, (unsigned int key, int flags));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(int, __cdecl, CoreSortedTable_ParseDescriptionId, 0x00698D90, (const char* description));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (CGuiSlider* self, void*));
CSF_FUNC(int*, __fastcall, DetachAndForwardLinkedNode, 0x007223B0, (void* self, void* , int16_t* a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static uint32_t __fastcall CMenuTextInput_InitializeControls(
    CMenuTextInput* self, void* /*edx*/, int owner, int /*unused*/)
{
    self->field_0010 = 0;
    self->field_001c = 0;
    self->field_0050 = 0;
    TextInputSubobject_Init(
        reinterpret_cast<S_004f21d0*>(reinterpret_cast<uint8_t*>(self) + 0x34),
        nullptr, &g_byte_82E65C);

    self->field_0010 = owner;
    auto* configuration = reinterpret_cast<uint8_t*>(self->field_0054);
    self->field_0030 = 0;
    self->field_0058 = 0;

    uint32_t identifier = configuration ? CSF_AT(uint32_t, configuration, 0) : 0;
    void* screen = reinterpret_cast<void*>(self->field_0014);
    ((void(__fastcall*)(void*, void*, uint32_t))
        CSF_AT(void**, screen, 0)[0xEC / 4])(screen, nullptr, identifier);

    configuration = reinterpret_cast<uint8_t*>(self->field_0054);
    self->field_0050 = configuration ? CSF_AT(uint32_t, configuration, 0) : 0;
    MenuProgressInput_UpdateGlobalState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    screen = reinterpret_cast<void*>(self->field_0014);
    CSF_AT(uint32_t, self, 0x20) =
        ((uint32_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, screen, 0)[0xFC / 4])(
                screen, nullptr, CSF_STR(0x008413F0, "EstaticoEdit"));

    screen = reinterpret_cast<void*>(self->field_0014);
    self->field_0024 =
        ((uint32_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, screen, 0)[0xFC / 4])(
                screen, nullptr, CSF_STR(0x008413E4, "UserEditBox"));

    screen = reinterpret_cast<void*>(self->field_0014);
    self->field_0028 =
        ((uint32_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, screen, 0)[0xFC / 4])(
                screen, nullptr, CSF_STR(0x008413D4, "BotonOkQuestion"));

    screen = reinterpret_cast<void*>(self->field_0014);
    uint32_t cancelButton =
        ((uint32_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, screen, 0)[0xFC / 4])(
                screen, nullptr, CSF_STR(0x008413C0, "BotonCancelQuestion"));
    self->field_002c = cancelButton;

    if (CSF_AT(uint32_t, self, 0x20) && self->field_0028 && cancelButton)
    {
        configuration = reinterpret_cast<uint8_t*>(self->field_0054);
        if (configuration)
        {
            void* entries = configuration + 4;
            if (static_cast<uint32_t>(
                    MenuProgressInput_GetEntryCount(entries, nullptr)) > 0)
            {
                if (MenuProgressInput_GetEntryCount(entries, nullptr) == 3)
                {
                    uint32_t remaining = static_cast<uint32_t>(
                        MenuProgressInput_GetEntryCount(entries, nullptr));
                    uint32_t entryOffset = 0;
                    while (remaining != 0)
                    {
                        configuration = reinterpret_cast<uint8_t*>(self->field_0054);
                        auto* entry = CSF_AT(uint8_t*, configuration, 8) + entryOffset;
                        const char* name = CSF_AT(uint32_t, entry, 24) < 0x10u
                            ? reinterpret_cast<const char*>(entry + 4)
                            : CSF_AT(const char*, entry, 4);

                        int controlKind = 0;
                        if (std::strcmp(CSF_STR(0x008413F0, "EstaticoEdit"), name) == 0)
                        {
                            controlKind = 1;
                        }
                        else
                        {
                            name = CSF_AT(uint32_t, entry, 24) < 0x10u
                                ? reinterpret_cast<const char*>(entry + 4)
                                : CSF_AT(const char*, entry, 4);
                            if (std::strcmp(
                                    CSF_STR(0x008413D4, "BotonOkQuestion"), name) == 0)
                            {
                                controlKind = 2;
                            }
                            else
                            {
                                name = CSF_AT(uint32_t, entry, 24) < 0x10u
                                    ? reinterpret_cast<const char*>(entry + 4)
                                    : CSF_AT(const char*, entry, 4);
                                if (std::strcmp(
                                        CSF_STR(0x008413C0, "BotonCancelQuestion"),
                                        name) == 0)
                                {
                                    controlKind = 3;
                                }
                            }
                        }

                        if (controlKind != 0)
                        {
                            int descriptionId = CoreSortedTable_ParseDescriptionId(
                                reinterpret_cast<const char*>(entry + 28));
                            void* control;
                            if (controlKind == 1)
                                control = reinterpret_cast<void*>(
                                    CSF_AT(uint32_t, self, 0x20));
                            else if (controlKind == 2)
                                control = reinterpret_cast<void*>(self->field_0028);
                            else
                                control = reinterpret_cast<void*>(self->field_002c);

                            void* textObject =
                                ((void*(__fastcall*)(void*, void*))
                                    CSF_AT(void**, control, 0)[0x110 / 4])(
                                        control, nullptr);
                            void** textVtable = CSF_AT(void**, textObject, 0);
                            int resolvedEntry = CoreParser_ResolveEntryByKey(
                                descriptionId, 1);
                            ((void(__fastcall*)(void*, void*, int))
                                textVtable[0x40 / 4])(
                                    textObject, nullptr, resolvedEntry);
                        }

                        entryOffset += 36;
                        --remaining;
                    }
                }
                else
                {
                    MenuProgressInput_GetEntryCount(entries, nullptr);
                }
            }
        }
    }

    screen = reinterpret_cast<void*>(self->field_0014);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    void* edit = reinterpret_cast<void*>(self->field_0024);
    if (edit)
    {
        configuration = reinterpret_cast<uint8_t*>(self->field_0054);
        if (configuration)
        {
            int16_t* initialText = CSF_AT(uint32_t, configuration, 44) < 8u
                ? reinterpret_cast<int16_t*>(configuration + 24)
                : CSF_AT(int16_t*, configuration, 24);
            DetachAndForwardLinkedNode(edit, nullptr, initialText);
        }
        CGuiSlider_ForwardSelfToParent(
            reinterpret_cast<CGuiSlider*>(self->field_0024), nullptr);
    }

    CMenuMpOnline_ScrollViewToTarget(
        reinterpret_cast<CMenuMpOnline*>(self), nullptr);

    void* ownerObject = reinterpret_cast<void*>(self->field_0010);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, ownerObject, 0)[0x9C / 4])(ownerObject, nullptr, 0);

    ownerObject = reinterpret_cast<void*>(self->field_0010);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, ownerObject, 0)[0xA8 / 4])(ownerObject, nullptr);

    return self->field_0014;
}

HOOK(0x0052BFD0, CMenuTextInput_InitializeControls, "auto");
