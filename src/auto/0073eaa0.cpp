#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(int, g_PreviewTimeoutAction, 0x0085807C);
CSF_GLOBAL(uint8_t, g_MenuMpCharacterSelectActive, 0x008BC121);

CSF_FUNC(void, __cdecl, sub_6471C0, 0x006471C0, (int a2));
CSF_FUNC(char, __cdecl, sub_6476A0, 0x006476A0, (int a1));
CSF_FUNC(int, __cdecl, sub_647B20, 0x00647B20, ());
CSF_FUNC(int, __cdecl, sub_6489E0, 0x006489E0, (int a1));
CSF_FUNC(bool, __fastcall, sub_650BA0, 0x00650BA0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_651B40, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_651C70, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(unsigned int*, __fastcall, sub_6530D0, 0x006530D0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Sub_655520, 0x00655520, (void* self, void* edx, int a, int b));
CSF_FUNC(void, __fastcall, sub_65E130, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_65E200, 0x0065E200, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, sub_671260, 0x00671260, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __cdecl, sub_73C450, 0x0073C450, ());
CSF_FUNC(char, __fastcall, sub_73C730, 0x0073C730, (void* self, void* edx, int a3));
CSF_FUNC(char, __fastcall, sub_73D500, 0x0073D500, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, NetSession_GetCharacterPreviewInterface, 0x0073E770,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, sub_7437C0, 0x007437C0, ());
CSF_FUNC(void, __cdecl, sub_743A70, 0x00743A70, ());
CSF_FUNC(void, __cdecl, sub_743EF0, 0x00743EF0, (float a1));
CSF_FUNC(int, __cdecl, GetValue_8BC170, 0x0074A680, ());
CSF_FUNC(void, __fastcall, sub_74AE60, 0x0074AE60, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, sub_7A2970, 0x007A2970, (void* self, void* edx));

static void __fastcall NetSession_AdvanceCharacterPreviewTimer(
    void* self, void* /*edx*/, float elapsed)
{
    sub_73C450();

    const int state = CSF_AT(int, self, 0x98);
    if (state == 6)
    {
        if (!g_MenuMpCharacterSelectActive)
        {
            sub_743EF0(elapsed);
            void* manager = reinterpret_cast<void*>(sub_647B20());
            Sub_655520(manager, nullptr, -1, 1);

            const float remaining = CSF_AT(float, self, 0xC0) - elapsed;
            CSF_AT(float, self, 0xC0) = remaining;
            if (remaining < g_fZero)
                CSF_AT(float, self, 0xC0) = 1.0f;

            if (GetValue_8BC170())
            {
                void* preview = reinterpret_cast<void*>(GetValue_8BC170());
                sub_74AE60(preview, nullptr, elapsed);
            }

            alignas(8) uint8_t selectionBytes[0x4];
            alignas(8) uint8_t temporaryBytes[0x20];
            alignas(8) uint8_t outputBytes[0x4];

            sub_6530D0(selectionBytes, nullptr);
            const int selectedCharacter = CSF_AT(int, self, 0x108);
            if (selectedCharacter != sub_65E200(selectionBytes, nullptr))
            {
                const int newCharacter = sub_65E200(selectionBytes, nullptr);
                sub_73C730(self, nullptr, newCharacter);
            }

            sub_65E130(selectionBytes, nullptr, 0);
            CSF_AT(uint32_t, outputBytes, 0) = 0;
            sub_651C70(temporaryBytes, nullptr,
                      reinterpret_cast<uint32_t*>(outputBytes));
            sub_651B40(temporaryBytes, nullptr);
        }

        if (CSF_AT(uint8_t, self, 0x10C))
        {
            if (CSF_AT(float, self, 0x110) <= g_fZero)
            {
                // Hex-Rays names this action as 2000; assembly loads its data cell.
                const int timeoutAction = g_PreviewTimeoutAction;
                sub_6476A0(timeoutAction == 2000 ? 2000 : timeoutAction);
                CSF_AT(uint8_t, self, 0x10C) = 0;
            }
            else
            {
                CSF_AT(float, self, 0x110) =
                    CSF_AT(float, self, 0x110) - elapsed;
            }
        }

        if (!CSF_AT(uint8_t, self, 0xFC))
        {
            void* previewInterface =
                NetSession_GetCharacterPreviewInterface(self, nullptr);
            if (previewInterface)
            {
                void* previewObject = reinterpret_cast<void*>(
                    reinterpret_cast<uintptr_t>(previewInterface) - 0x308);
                if (previewObject)
                {
                    if (((bool(__fastcall*)(void*, void*))
                         CSF_AT(void**, previewInterface, 0)[2])(
                            previewInterface, nullptr))
                    {
                        CSF_AT(uint8_t, self, 0xFC) = 1;
                    }
                }
            }
        }
    }
    else if (state == 7)
    {
        void* pendingObject = CSF_AT(void*, self, 0x118);
        if (pendingObject)
        {
            sub_650BA0(pendingObject, nullptr, -1);
            pendingObject = CSF_AT(void*, self, 0x118);
            if (CSF_AT(int, pendingObject, 0xC) != 1)
            {
                if (pendingObject)
                {
                    ((void(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, pendingObject, 0)[0])(
                        pendingObject, nullptr, 1);
                    CSF_AT(void*, self, 0x118) = nullptr;
                }

                sub_743A70();
                void* menu = reinterpret_cast<void*>(sub_6489E0(1));
                CSF_AT(uint8_t, menu, 0x10) = 0;

                void* context = sub_671260();
                CSF_AT(uint32_t, context, 0x10) |= 0x4000000u;

                void* resource = CSF_AT(void*, self, 0);
                if (resource)
                {
                    sub_7A2970(resource, nullptr);
                    sub_6471C0(static_cast<int>(
                        reinterpret_cast<intptr_t>(resource)));
                    CSF_AT(void*, self, 0) = nullptr;
                }

                CSF_AT(uint8_t, self, 0x14F) = 0;
                sub_7437C0();

                void* firstObject = CSF_AT(void*, self, 0x4);
                if (firstObject)
                {
                    ((void(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, firstObject, 0)[0])(
                        firstObject, nullptr, 1);
                    CSF_AT(void*, self, 0x4) = nullptr;
                }

                void* secondObject = CSF_AT(void*, self, 0x8);
                if (secondObject)
                {
                    ((void(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, secondObject, 0)[0])(
                        secondObject, nullptr, 1);
                    CSF_AT(void*, self, 0x8) = nullptr;
                }

                void* previewData = CSF_AT(void*, self, 0xC8);
                if (previewData)
                {
                    sub_73D500(previewData, nullptr);
                    Mem_Free(previewData);
                    CSF_AT(void*, self, 0xC8) = nullptr;
                }

                CSF_AT(int, self, 0x98) = 0;
            }
        }
    }
}

HOOK(0x0073EAA0, NetSession_AdvanceCharacterPreviewTimer, "auto");
