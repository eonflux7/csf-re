#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "legacy/string.h"
#include "types/auto_classes.h"

// Original functions called by this routine, bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalStateRoot, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetGlobalValue, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, ApplyGlobalValueBinding, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalStateObject, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, GetObjectForGlobalInt, 0x00652160, (int globalIntValue, void* edx));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuAccounts_ResetConfiguration, 0x00565FA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpNetworkType_ScrollViewportToTarget, 0x00534F20, (void* self, void* edx));

CSF_GLOBAL(int, g_SelectedAccountIndex, 0x0089A5E8);

// Signatures of the virtual methods dispatched by this routine (__thiscall, object in ecx).
using VirtualVoidNoArgs = void(__fastcall*)(void* self, void* edx);
using VirtualVoidIntArg = void(__fastcall*)(void* self, void* edx, int arg);
using VirtualVoidPtrArg = void(__fastcall*)(void* self, void* edx, void* arg);
using VirtualIntNoArgs = int(__fastcall*)(void* self, void* edx);
using VirtualIntNameArg = int(__fastcall*)(void* self, void* edx, const char* name);

// Converts a 32-bit address stored in a field into an object pointer.
static void* ObjectPointerFromValue(uint32_t value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

static int __fastcall CMenuMpNetworkType_InitializeScreen(CMenuMpNetworkType* self, void* /*edx*/, int entryMode, int /*unusedArgument*/)
{
    // No GUI object attached yet: the global state root binds this screen (vtable slot 0x18).
    if (self->field_0038 == 0)
    {
        void* stateRoot = GetGlobalStateRoot();
        reinterpret_cast<VirtualVoidPtrArg>(CSF_AT(void**, stateRoot, 0)[6])(stateRoot, nullptr, self);
    }

    // Reset the per-entry state of the screen.
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0025 = 0;
    self->field_0026 = 0;
    self->field_0027 = 0;
    self->field_002c = 0;
    self->field_0034 = 0;
    self->field_0044 = 0;
    self->field_0030 = 0xFFFFFFFFu;
    self->field_0049 = 0;

    // Virtual slot 0x40 on this screen, no arguments.
    reinterpret_cast<VirtualVoidNoArgs>(CSF_AT(void**, self, 0)[16])(self, nullptr);

    self->field_0034 = static_cast<uint32_t>(entryMode);

    ApplyGlobalValueBinding(GetGlobalValue(), nullptr);

    // Virtual slot 0x16C on the GUI object, no arguments, result unused.
    {
        void* guiObject = ObjectPointerFromValue(self->field_0038);
        reinterpret_cast<VirtualVoidNoArgs>(CSF_AT(void**, guiObject, 0)[91])(guiObject, nullptr);
    }

    // If a global state object exists, query its state (slot 0x1C); when it is not 2, notify it once more.
    if (GetGlobalStateObject() != nullptr)
    {
        void* stateObject = GetGlobalStateObject();
        int stateValue = reinterpret_cast<VirtualIntNoArgs>(CSF_AT(void**, stateObject, 0)[7])(stateObject, nullptr);
        if (stateValue != 2)
        {
            void* stateObjectAgain = GetGlobalStateObject();
            reinterpret_cast<VirtualIntNoArgs>(CSF_AT(void**, stateObjectAgain, 0)[7])(stateObjectAgain, nullptr);
        }
    }

    // Second notification of the global state object (slot 0x1C), guarded the same way.
    if (GetGlobalStateObject() != nullptr)
    {
        void* stateObject = GetGlobalStateObject();
        reinterpret_cast<VirtualIntNoArgs>(CSF_AT(void**, stateObject, 0)[7])(stateObject, nullptr);
    }

    // Bind the BotonLAN control (slot 0xFC, looked up by name) and forward it to the slider helper.
    {
        void* guiObject = ObjectPointerFromValue(self->field_0038);
        uint32_t lanButtonControl = static_cast<uint32_t>(
            reinterpret_cast<VirtualIntNameArg>(CSF_AT(void**, guiObject, 0)[63])(guiObject, nullptr, CSF_STR(0x00845C4C, "BotonLAN")));
        self->field_0014 = lanButtonControl;
        if (lanButtonControl != 0)
        {
            CGuiSlider_ForwardSelfToParent(ObjectPointerFromValue(lanButtonControl), nullptr);
        }
    }

    // Bind the BotonWAN control.
    {
        void* guiObject = ObjectPointerFromValue(self->field_0038);
        self->field_0018 = static_cast<uint32_t>(
            reinterpret_cast<VirtualIntNameArg>(CSF_AT(void**, guiObject, 0)[63])(guiObject, nullptr, CSF_STR(0x00845C40, "BotonWAN")));
    }

    // Bind the StaticInfo control.
    {
        void* guiObject = ObjectPointerFromValue(self->field_0038);
        self->field_002c = static_cast<uint32_t>(
            reinterpret_cast<VirtualIntNameArg>(CSF_AT(void**, guiObject, 0)[63])(guiObject, nullptr, CSF_STR(0x00843098, "StaticInfo")));
    }

    // If a global state object exists, call its slot 0x44 with argument 0.
    if (GetGlobalStateObject() != nullptr)
    {
        void* stateObject = GetGlobalStateObject();
        reinterpret_cast<VirtualVoidIntArg>(CSF_AT(void**, stateObject, 0)[17])(stateObject, nullptr, 0);
    }

    // Look up an object from the global int and call its slot 0x18, twice.
    {
        int globalIntValue = GetGlobalInt();
        void* lookedUpObject = GetObjectForGlobalInt(globalIntValue, nullptr);
        reinterpret_cast<VirtualVoidNoArgs>(CSF_AT(void**, lookedUpObject, 0)[6])(lookedUpObject, nullptr);
    }
    {
        int globalIntValue = GetGlobalInt();
        void* lookedUpObject = GetObjectForGlobalInt(globalIntValue, nullptr);
        reinterpret_cast<VirtualVoidNoArgs>(CSF_AT(void**, lookedUpObject, 0)[6])(lookedUpObject, nullptr);
    }

    MenuAccounts_ResetConfiguration(&g_SelectedAccountIndex, nullptr);

    // Virtual slot 0x44 on this screen, passing the GUI object address as argument.
    {
        int guiObjectArgument = static_cast<int>(self->field_0038);
        self->field_0028 = 0;
        reinterpret_cast<VirtualVoidIntArg>(CSF_AT(void**, self, 0)[17])(self, nullptr, guiObjectArgument);
    }

    CMenuMpNetworkType_ScrollViewportToTarget(self, nullptr);

    return static_cast<int>(self->field_0038);
}

HOOK(0x00565790, CMenuMpNetworkType_InitializeScreen, "auto");
