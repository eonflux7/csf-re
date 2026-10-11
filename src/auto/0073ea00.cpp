#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, ReturnZero, 0x004B2A30, ());
CSF_FUNC(int, __fastcall, Sub_647AA0, 0x00647AA0, (void* self, void* edx, int a, int b));
CSF_FUNC(char, __fastcall, Sub_655520, 0x00655520, (void* self, void* edx, int a, int b));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void*, __fastcall, NetSession_GetSessionInterface, 0x0073E770,
         (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_MenuMpCharacterSelectLookup, 0x008B9A90);

static hook::Tail __fastcall NetSession_UpdateAndForward(
    void* self, const uint32_t* /*args*/)
{
    if (CSF_AT(uint32_t, self, 0x98))
    {
        const int lookupKey = ReturnZero();
        const int lookupResult = Sub_647AA0(
            &g_MenuMpCharacterSelectLookup, nullptr, 4, lookupKey);

        void* characterSelect = nullptr;
        if (lookupResult)
        {
            void* entry = reinterpret_cast<void*>(
                static_cast<uintptr_t>(lookupResult));
            void* selection = CSF_AT(void*, entry, 0x0C);
            if (selection)
                characterSelect = CSF_AT(void*, selection, 0x08);
        }

        Sub_655520(characterSelect, nullptr, -1, 1);

        if (!CSF_AT(uint8_t, self, 0xFC))
        {
            void* sessionInterface =
                NetSession_GetSessionInterface(self, nullptr);
            if (sessionInterface)
            {
                auto* session = reinterpret_cast<uint8_t*>(
                    reinterpret_cast<uintptr_t>(sessionInterface) - 0x308);
                if (session)
                {
                    void* interfaceObject = session + 0x308;
                    if (((bool(__fastcall*)(void*, void*))
                             CSF_AT(void**, interfaceObject, 0)[0x08 / 4])(
                            interfaceObject, nullptr))
                    {
                        CSF_AT(uint8_t, self, 0xFC) = 1;
                    }
                }
            }
        }
    }

    int result = GetGlobalState_6A6D70();
    if (result)
    {
        void* state = reinterpret_cast<void*>(
            static_cast<uintptr_t>(GetGlobalState_6A6D70()));
        result = ((int(__fastcall*)(void*, void*))
                      CSF_AT(void**, state, 0)[0x1C / 4])(state, nullptr);
        if (result == 10)
        {
            void* obj = reinterpret_cast<void*>(
                static_cast<uintptr_t>(GetGlobalState_6A6D70()));
            return hook::tail_jump(
                obj, (*reinterpret_cast<void***>(obj))[0x18 / 4]);
        }
    }

    return hook::tail_ret(result, 4);
}

HOOK_TAIL(0x0073EA00, NetSession_UpdateAndForward, "net_session");
