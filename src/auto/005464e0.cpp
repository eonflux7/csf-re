#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, CompareNamePrefix, 0x007B00B0,
               (const char* name, const char* prefix, std::size_t length));
static CSF_FUNC(std::int32_t, __cdecl, ParseThumbnailIndex, 0x007AFD89,
               (const char* text));
static CSF_FUNC(int, __cdecl, ResolveThumbnailResource, 0x00692440,
               (int resource, int mode));
static CSF_FUNC(int, __fastcall, ForwardThumbnailToChild, 0x00720DF0,
               (void* self, void* edx, int resource));

static void __fastcall CMenuExtras_SelectThumbnailFromControl(
    CMenuExtras* self, void* /*edx*/, void* control)
{
    if (!control)
        return;

    const char* name =
        reinterpret_cast<const char* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x74 / 4])(control, nullptr);

    if (CompareNamePrefix(name, CSF_STR(0x00843C48, "thumb"), 5) != 0)
        return;

    const char* suffix =
        reinterpret_cast<const char* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x74 / 4])(control, nullptr) + 5;

    char indexText[12];
    char* destination = indexText;
    char character;
    do
    {
        character = *suffix++;
        *destination++ = character;
    } while (character != '\0');

    const std::uint32_t index =
        static_cast<std::uint32_t>(ParseThumbnailIndex(indexText));

    const std::uint32_t begin = self->field_0038;
    if (!begin)
        return;

    const std::int32_t byteDifference =
        static_cast<std::int32_t>(self->field_003c - begin);
    const std::uint32_t count =
        static_cast<std::uint32_t>(byteDifference / 68);

    if (index >= count)
        return;

    const std::uint32_t entryAddress = self->field_0038 + index * 68u;
    const int resource = ResolveThumbnailResource(
        CSF_AT(int, reinterpret_cast<void*>(entryAddress), 0), 1);

    ForwardThumbnailToChild(
        reinterpret_cast<void*>(self->field_0030), nullptr, resource);
    self->field_0028 = index;
}

HOOK(0x005464E0, CMenuExtras_SelectThumbnailFromControl, "auto");
