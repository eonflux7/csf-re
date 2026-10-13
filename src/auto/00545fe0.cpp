#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(char, __stdcall, CMenuExtras_IsThumbnailUnlocked, 0x00545F00, (int recordId));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(CGuiStatic*, __fastcall, CGuiStatic_Construct, 0x00720E10, (CGuiStatic* self, void* , int owner, int argument));
CSF_FUNC(int, __fastcall, CGuiButton_Construct, 0x00725390, (CGuiButton* self, void* , int renderer, int context));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_PointToCellScale, 0x0083E10C);
CSF_GLOBAL(float, g_ThumbnailLeftMargin, 0x00843C10);
CSF_GLOBAL(float, g_RasterByteScale, 0x00843C14);
CSF_GLOBAL(float, g_ThumbnailUvWidth, 0x00843C18);
CSF_GLOBAL(float, g_ThumbnailHeight, 0x00843C28);
CSF_GLOBAL(float, g_ThumbnailWidth, 0x00843C2C);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static void* MenuExtras_GetGuiManager()
{
    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* service = CSF_AT(void*, state, 0x14);
    return ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, service, 0)[0x54 / 4])(service, nullptr);
}

static char __fastcall CMenuExtras_CreateThumbnailGrid(CMenuExtras* self, void* /*edx*/)
{
    void* display = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    const unsigned int displayWidth = CSF_AT(unsigned int, dimensions, 0);

    display = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    const unsigned int displayHeight = CSF_AT(unsigned int, dimensions, 4);

    const int owner = reinterpret_cast<int>(
        MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* texturePaths = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);
    ((void(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, texturePaths, 0)[0x10 / 4])(
            texturePaths, nullptr, CSF_STR(0x00843C30, "menus\\textures\\extras\\"));

    alignas(8) std::uint8_t nameBytes[0x28];
    alignas(8) std::uint8_t thumbnailUvBytes[0x10];
    alignas(8) std::uint8_t lockedUvBytes[0x10];
    alignas(8) std::uint8_t colorBytes[0x10];
    alignas(8) std::uint8_t sizeBytes[0x8];
    alignas(8) std::uint8_t positionBytes[0x3C];
    char* name = reinterpret_cast<char*>(nameBytes);
    float* thumbnailUv = reinterpret_cast<float*>(thumbnailUvBytes);
    float* lockedUv = reinterpret_cast<float*>(lockedUvBytes);
    float* color = reinterpret_cast<float*>(colorBytes);
    int* size = reinterpret_cast<int*>(sizeBytes);
    int* position = reinterpret_cast<int*>(positionBytes);

    bool finished = false;
    unsigned int thumbnailIndex = 0;
    char result = 0;

    for (unsigned int row = 0; row < 6 && !finished; ++row)
    {
        unsigned int recordOffset = 68 * thumbnailIndex;
        for (unsigned int column = 0; column < 6; ++column)
        {
            const float columnNumber = static_cast<float>(column);
            const float imageX = columnNumber * g_ThumbnailWidth;
            const float rowNumber = static_cast<float>(row);
            const float imageY = rowNumber * g_ThumbnailHeight;
            void* lockedOverlay = nullptr;

            CGuiButton* storage = reinterpret_cast<CGuiButton*>(CoreMemory_AllocUntagged(496));
            if (storage)
            {
                void* manager = MenuExtras_GetGuiManager();
                int context = ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, manager, 0)[0x10 / 4])(manager, nullptr);
                void* button = reinterpret_cast<void*>(
                    CGuiButton_Construct(storage, nullptr, owner, context));
                if (button)
                {
                    void* controls = MenuControls_ResolveObject(
                        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
                    void* texture = ((void*(__fastcall*)(void*, void*, const char*))
                        CSF_AT(void**, controls, 0)[0x38 / 4])(
                            controls, nullptr, CSF_STR(0x00843C1C, "thumbs.png"));
                    if (texture)
                    {
                        const float u = imageX * g_PointToCellScale;
                        const float v = imageY * g_PointToCellScale;
                        thumbnailUv[0] = u;
                        thumbnailUv[1] = v;
                        thumbnailUv[2] = u + g_ThumbnailUvWidth;
                        thumbnailUv[3] = v + g_RasterByteScale;
                        ((void(__fastcall*)(void*, void*, float*))
                            CSF_AT(void**, texture, 0)[0x4C / 4])(
                                texture, nullptr, thumbnailUv);
                        ((void(__fastcall*)(void*, void*, void*))
                            CSF_AT(void**, button, 0)[0x244 / 4])(
                                button, nullptr, texture);
                        ReleaseObjectReference(static_cast<std::uint8_t*>(texture) + 4, nullptr);
                    }

                    position[0] = static_cast<int>(
                        columnNumber * g_MessageBoxAnimationScale + imageX + g_ThumbnailLeftMargin);
                    position[1] = static_cast<int>(
                        rowNumber * g_MessageBoxAnimationScale + imageY + g_fReturnedConstant_0082E030);
                    ((void(__fastcall*)(void*, void*, int*))
                        CSF_AT(void**, button, 0)[0x98 / 4])(button, nullptr, position);
                    size[0] = 85;
                    size[1] = 64;
                    ((void(__fastcall*)(void*, void*, int*))
                        CSF_AT(void**, button, 0)[0xA0 / 4])(button, nullptr, size);

                    color[0] = 255.0f;
                    color[1] = 129.0f;
                    color[2] = 24.0f;
                    color[3] = 255.0f;
                    ((void(__fastcall*)(void*, void*, float*, int, int))
                        CSF_AT(void**, button, 0)[0x1F8 / 4])(
                            button, nullptr, color, 4, 0);
                    ((void(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, button, 0)[0x1FC / 4])(button, nullptr, 1);

                    manager = MenuExtras_GetGuiManager();
                    void* fontReference = ((void*(__fastcall*)(void*, void*))
                        CSF_AT(void**, manager, 0)[0x48 / 4])(manager, nullptr);
                    ((void(__fastcall*)(void*, void*, std::uint32_t))
                        CSF_AT(void**, button, 0)[0x170 / 4])(
                            button, nullptr, CSF_AT(std::uint32_t, fontReference, 0));
                    _sprintf(name, CSF_STR(0x00843C04, "thumb%3d"), thumbnailIndex);
                    ((void(__fastcall*)(void*, void*, char*))
                        CSF_AT(void**, button, 0)[0x70 / 4])(button, nullptr, name);

                    std::uint8_t* record =
                        reinterpret_cast<std::uint8_t*>(self->field_0038) + recordOffset;
                    int recordId = CSF_AT(unsigned int, record, 0x40) < 0x10
                        ? reinterpret_cast<int>(record + 44)
                        : CSF_AT(int, record, 44);
                    if (!CMenuExtras_IsThumbnailUnlocked(recordId))
                    {
                        CGuiStatic* overlayStorage =
                            reinterpret_cast<CGuiStatic*>(CoreMemory_AllocUntagged(352));
                        if (overlayStorage)
                        {
                            manager = MenuExtras_GetGuiManager();
                            context = ((int(__fastcall*)(void*, void*))
                                CSF_AT(void**, manager, 0)[0x10 / 4])(manager, nullptr);
                            lockedOverlay =
                                CGuiStatic_Construct(overlayStorage, nullptr, owner, context);
                            if (lockedOverlay)
                            {
                                auto setPosition = reinterpret_cast<
                                    void(__fastcall*)(void*, void*, void*)>(
                                        CSF_AT(void**, lockedOverlay, 0)[0x98 / 4]);
                                void* buttonPosition = ((void*(__fastcall*)(void*, void*))
                                    CSF_AT(void**, button, 0)[0x9C / 4])(button, nullptr);
                                setPosition(lockedOverlay, nullptr, buttonPosition);

                                auto setSize = reinterpret_cast<
                                    void(__fastcall*)(void*, void*, void*)>(
                                        CSF_AT(void**, lockedOverlay, 0)[0xA0 / 4]);
                                void* buttonSize = ((void*(__fastcall*)(void*, void*))
                                    CSF_AT(void**, button, 0)[0xA4 / 4])(button, nullptr);
                                setSize(lockedOverlay, nullptr, buttonSize);

                                controls = MenuControls_ResolveObject(
                                    reinterpret_cast<void*>(GetGlobalInt()), nullptr);
                                texture = ((void*(__fastcall*)(void*, void*, const char*))
                                    CSF_AT(void**, controls, 0)[0x38 / 4])(
                                        controls, nullptr, CSF_STR(0x00843C1C, "thumbs.png"));
                                if (texture)
                                {
                                    lockedUv[0] = 0.830078125f;
                                    lockedUv[1] = 0.875f;
                                    lockedUv[2] = 0.99609375f;
                                    lockedUv[3] = 1.0f;
                                    ((void(__fastcall*)(void*, void*, float*))
                                        CSF_AT(void**, texture, 0)[0x4C / 4])(
                                            texture, nullptr, lockedUv);
                                    ((void(__fastcall*)(void*, void*, void*))
                                        CSF_AT(void**, lockedOverlay, 0)[0xD8 / 4])(
                                            lockedOverlay, nullptr, texture);
                                    ReleaseObjectReference(
                                        static_cast<std::uint8_t*>(texture) + 4, nullptr);
                                }

                                void* panel = reinterpret_cast<void*>(self->field_0018);
                                ((void(__fastcall*)(void*, void*, void*))
                                    CSF_AT(void**, panel, 0)[0xF8 / 4])(
                                        panel, nullptr, lockedOverlay);
                                const float scaleY =
                                    static_cast<float>(displayHeight) / static_cast<float>(g_ScreenHeight);
                                const float scaleX =
                                    static_cast<float>(displayWidth) / static_cast<float>(g_ScreenWidth);
                                ((void(__fastcall*)(void*, void*, float, float))
                                    CSF_AT(void**, lockedOverlay, 0)[0x150 / 4])(
                                        lockedOverlay, nullptr, scaleX, scaleY);
                            }
                        }
                    }

                    void* panel = reinterpret_cast<void*>(self->field_0018);
                    ((void(__fastcall*)(void*, void*, void*))
                        CSF_AT(void**, panel, 0)[0xF8 / 4])(panel, nullptr, button);
                    const float scaleY =
                        static_cast<float>(displayHeight) / static_cast<float>(g_ScreenHeight);
                    const float scaleX =
                        static_cast<float>(displayWidth) / static_cast<float>(g_ScreenWidth);
                    ((void(__fastcall*)(void*, void*, float, float))
                        CSF_AT(void**, button, 0)[0x150 / 4])(
                            button, nullptr, scaleX, scaleY);

                    const std::uint32_t recordsBegin = self->field_0038;
                    if (recordsBegin &&
                        thumbnailIndex < static_cast<unsigned int>(
                            static_cast<std::int32_t>(self->field_003c - recordsBegin) / 68))
                    {
                        CSF_AT(std::uint32_t,
                            reinterpret_cast<void*>(self->field_0038 + recordOffset), 32) =
                            reinterpret_cast<std::uint32_t>(button);
                        CSF_AT(std::uint32_t,
                            reinterpret_cast<void*>(self->field_0038 + recordOffset), 36) =
                            reinterpret_cast<std::uint32_t>(lockedOverlay);
                    }

                    std::uint32_t count = self->field_0038;
                    ++thumbnailIndex;
                    recordOffset += 68;
                    if (count)
                        count = static_cast<std::int32_t>(self->field_003c - count) / 68;
                    finished = thumbnailIndex == count;
                }
            }

            result = static_cast<char>(finished);
            if (finished)
                break;
            result = static_cast<char>(column + 1);
        }
    }

    self->field_0028 = 0;
    return result;
}

HOOK(0x00545FE0, CMenuExtras_CreateThumbnailGrid, "auto");
