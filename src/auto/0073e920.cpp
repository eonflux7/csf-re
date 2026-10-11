#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ReturnZero, 0x004B2A30, ());
CSF_FUNC(void, __cdecl, sub_6471C0, 0x006471C0, (int a2));
CSF_FUNC(int, __fastcall, Sub_647AA0, 0x00647AA0, (void* self, void* edx, int a, int b));
CSF_FUNC(void*, __cdecl, sub_671260, 0x00671260, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __cdecl, sub_73C450, 0x0073C450, ());
CSF_FUNC(char, __fastcall, sub_73D500, 0x0073D500, (void* self, void* edx));
CSF_FUNC(void, __cdecl, sub_73DE40, 0x0073DE40, ());
CSF_FUNC(int, __cdecl, sub_7437C0, 0x007437C0, ());
CSF_FUNC(void, __cdecl, sub_743A70, 0x00743A70, ());
CSF_FUNC(void, __fastcall, sub_78DE80, 0x0078DE80, (void* self, void* edx, int a1));
CSF_FUNC(void, __fastcall, sub_7A2970, 0x007A2970, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_MenuMpCharacterSelectLookup, 0x008B9A90);
CSF_GLOBAL(uint8_t*, g_MenuCharacterRecords, 0x008B9B40);
CSF_GLOBAL(uint8_t, g_DefaultMenuCharacterRecord, 0x008B9B48);

static int __fastcall NetSession_FinalizeCharacterPreviewSlot(
    void* self, void* /*edx*/)
{
    sub_73C450();

    int result = CSF_AT(int, self, 0x00);
    if (result)
    {
        sub_73DE40();

        int characterIndex = ReturnZero();
        int lookupResult = Sub_647AA0(
            &g_MenuMpCharacterSelectLookup, nullptr, 6, characterIndex);
        sub_78DE80(reinterpret_cast<void*>(lookupResult), nullptr, 1);
        sub_743A70();

        characterIndex = ReturnZero();
        uint8_t* characterRecord;
        if (characterIndex)
        {
            characterRecord = g_MenuCharacterRecords
                + 32u * static_cast<uint32_t>(characterIndex);
        }
        else
        {
            characterRecord = &g_DefaultMenuCharacterRecord;
        }
        CSF_AT(uint8_t, characterRecord, 0x10) = 0;

        void* state = sub_671260();
        CSF_AT(uint32_t, state, 0x10) |= 0x4000000u;

        int preview = CSF_AT(int, self, 0x00);
        if (preview)
        {
            sub_7A2970(reinterpret_cast<void*>(preview), nullptr);
            sub_6471C0(preview);
            CSF_AT(int, self, 0x00) = 0;
        }

        CSF_AT(uint8_t, self, 0x14F) = 0;
        CSF_AT(int, self, 0x00) = 0;
        result = sub_7437C0();

        void* firstObject = CSF_AT(void*, self, 0x04);
        if (firstObject)
        {
            result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, firstObject, 0x00)[0])(
                    firstObject, nullptr, 1);
            CSF_AT(void*, self, 0x04) = nullptr;
        }

        void* secondObject = CSF_AT(void*, self, 0x08);
        if (secondObject)
        {
            result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, secondObject, 0x00)[0])(
                    secondObject, nullptr, 1);
            CSF_AT(void*, self, 0x08) = nullptr;
        }
    }

    void* previewStorage = CSF_AT(void*, self, 0xC8);
    if (previewStorage)
    {
        sub_73D500(previewStorage, nullptr);
        result = Mem_Free(previewStorage);
        CSF_AT(void*, self, 0xC8) = nullptr;
    }

    CSF_AT(int, self, 0x98) = 0;
    return result;
}

HOOK(0x0073E920, NetSession_FinalizeCharacterPreviewSlot, "auto");
