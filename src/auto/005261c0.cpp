#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, AudioSoundManager_CollectSceneAudioEntries, 0x004EF620, (void* self, void* edx, int a2, char a3, void* a4));
CSF_FUNC(int, __fastcall, S_00525f70_ApplyAudioMode, 0x00525F70, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static char __fastcall S_00525f70_UpdateSceneAudioState(
    S_00525f70* self, void* /*edx*/)
{
    struct AudioUpdateContext
    {
        void* callback;
        void* entries;
        uint8_t category;
        uint8_t reserved[3];
        uint32_t scene;
        int mode;
        uint8_t reservedTail[2];
        uint8_t foundCategory;
        uint8_t foundMode;
    };
    static_assert(sizeof(AudioUpdateContext) == 0x18);
    static_assert(offsetof(AudioUpdateContext, scene) == 0x0C);
    static_assert(offsetof(AudioUpdateContext, foundCategory) == 0x16);

    AudioUpdateContext context{};
    context.scene = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (!context.scene)
        return 0;

    context.callback = reinterpret_cast<void*>(CSF_ADDR_FN(0x00525A10));
    int category = 0;

    void* audioEntries = reinterpret_cast<uint8_t*>(self) + 0x8C;
    void* service = reinterpret_cast<void*>(GetValue8B0554());
    if (static_cast<uint8_t>(AudioSoundManager_CollectSceneAudioEntries(
            CSF_AT(void*, service, 0x1FDC),
            nullptr,
            static_cast<int>(context.scene + 0x1C),
            0,
            audioEntries)))
    {
        for (int32_t index = static_cast<int32_t>(
                 CSF_AT(uint32_t, self, 0x94) - 1);
             index >= 0; --index)
        {
            if (context.foundCategory || context.foundMode)
                break;

            void* entries = CSF_AT(void*, self, 0x8C);
            void* entry = CSF_AT(void*, entries, index * 4);
            category = CSF_AT(int, entry, 0x5C);
            if (category)
            {
                context.entries = audioEntries;
                context.category = CSF_AT(uint8_t, entry, 0x64);
                context.foundCategory = 1;
            }

            if (CSF_AT(uint8_t, self, 0x14))
            {
                const uint32_t flags = CSF_AT(uint32_t, entry, 0x58);
                if (flags & 4)
                {
                    if (self->field_0020 == static_cast<uint32_t>(-1))
                        context.mode = 1;
                    else
                        context.mode = (flags & 8) ? 2 : 1;
                    context.foundMode = 1;
                }
            }
        }
    }

    int mode = context.mode;
    if (CSF_AT(uint8_t, self, 0x14) && !context.foundMode)
        mode = 3;

    void* scene = reinterpret_cast<void*>(context.scene);
    if ((CSF_AT(uint32_t, scene, 0x2E0) & 0x10000) &&
        CSF_AT(uint8_t, scene, 0x3D0))
    {
        category = 23;
    }
    else if (category == 23)
    {
        category = 0;
    }

    void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, manager, 0)[0x5C / 4])(
            manager, nullptr, category);

    manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, manager, 0)[0x64 / 4])(
            manager, nullptr, &context);

    // Retain the packet's vtable binding annotation for this method address.
    (void)CSF_VTBL(0x00525F70);

    if (CSF_AT(uint8_t, self, 0x14))
        return static_cast<char>(S_00525f70_ApplyAudioMode(self, nullptr, mode));
    return 0;
}

HOOK(0x005261C0, S_00525f70_UpdateSceneAudioState, "auto");
