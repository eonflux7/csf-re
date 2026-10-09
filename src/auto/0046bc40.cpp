#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, DestroyScriptParser, 0x0046ACF0, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, LoadSceneResources, 0x0046B530, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, InitializeScriptParser, 0x0046E2C0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, ParseSceneScript, 0x0046EAD0, (void* self, void* edx, int a2, void* a3, int a4));
CSF_FUNC(uint8_t*, __fastcall, InitializeSceneScript, 0x00470E10, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, DestroySceneScript, 0x00471130, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int*, __fastcall, InitializeSceneVariables, 0x00770440, (void* self, void* edx, int a2, void* a3));

static char __fastcall S_0046b530_LoadGameSceneScript(
    S_0046b530* self, void* /*edx*/, int path)
{
    // Packet-listed vtable references correspond to direct callees in the assembly.
    (void)CSF_VTBL(0x00692B10);
    (void)CSF_VTBL(0x0046B530);
    (void)CSF_VTBL(0x00692B80);
    (void)CSF_VTBL(0x0046E2C0);
    (void)CSF_VTBL(0x0041E5F0);
    (void)CSF_VTBL(0x00470E10);

    alignas(4) std::uint8_t file[0x54];
    char success = 1;
    MenuExtras_InitializeFile(file, nullptr);

    if (!MenuExtras_LoadFile(file, nullptr, reinterpret_cast<const char*>(path)))
    {
        MenuExtras_CloseFile(file, nullptr);
        return 0;
    }

    auto* root = CSF_AT(std::uint8_t*, file, 0);
    auto* resources = FindProperty(
        root, nullptr, const_cast<char*>(CSF_STR(0x00831580, ".RECURSOS")));
    if (resources)
        LoadSceneResources(self, nullptr, resources);

    auto* variableNode = FindProperty(
        root, nullptr, const_cast<char*>(CSF_STR(0x00831574, ".VARIABLES")));
    void* variables = Mem_Alloc(20);
    if (variables)
        InitializeSceneVariables(variables, nullptr, 0, variableNode);
    CSF_AT(void*, self, 0x3C) = variables;

    auto* scripts = FindProperty(
        root, nullptr, const_cast<char*>(CSF_STR(0x00831568, ".SCRIPTS")));
    if (scripts)
    {
        const std::uint32_t childCount =
            MenuExtras_GetChildCount(scripts, nullptr);
        if (childCount)
        {
            void* parserStorage = Mem_Alloc(80);
            void* parser = nullptr;
            if (parserStorage)
            {
                parser = InitializeScriptParser(
                    parserStorage, nullptr, CSF_AT(int, self, 0x3C));
            }

            const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x1C);
            auto* scriptArray = reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x18);
            if (childCount > capacity)
                GrowButtonArray(scriptArray, nullptr,
                                static_cast<int>(childCount - capacity));

            for (std::uint32_t index = 0; index < childCount; ++index)
            {
                void* component = reinterpret_cast<void*>(
                    GetComponent(scripts, nullptr, index));
                void* scriptStorage = Mem_Alloc(128);
                std::uint8_t* script = nullptr;
                if (scriptStorage)
                {
                    script = InitializeSceneScript(
                        scriptStorage, nullptr,
                        reinterpret_cast<int>(
                            reinterpret_cast<std::uint8_t*>(self) + 0x40));
                }

                if (ParseSceneScript(parser, nullptr, path, component,
                                     reinterpret_cast<int>(script)))
                {
                    if (scriptArray->field_0008 >= scriptArray->field_0004)
                    {
                        GrowButtonArray(
                            scriptArray, nullptr,
                            static_cast<int>(scriptArray->field_000c));
                    }

                    const std::uint32_t count = scriptArray->field_0008;
                    auto* slot = reinterpret_cast<std::uint8_t*>(
                        scriptArray->field_0000) + 4 * count;
                    scriptArray->field_0008 = count + 1;
                    CSF_AT(std::uint8_t*, slot, 0) = script;
                }
                else
                {
                    if (CSF_AT(std::uint8_t,
                               reinterpret_cast<void*>(GetGlobalInt()), 4))
                        success = 0;

                    if (script)
                    {
                        DestroySceneScript(script, nullptr);
                        Mem_Free(script);
                    }
                }
            }

            if (parser)
            {
                DestroyScriptParser(parser, nullptr);
                Mem_Free(parser);
            }
        }
    }

    MenuExtras_CloseFile(file, nullptr);
    return success;
}

HOOK(0x0046BC40, S_0046b530_LoadGameSceneScript, "auto");
