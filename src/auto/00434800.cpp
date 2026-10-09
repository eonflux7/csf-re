#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_DestroyAuxiliaryPanel, 0x00427520, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Hud_ProcessWidgetPair, 0x00433B90, (int* firstArgument, int* secondArgument, std::uintptr_t context, int* repeatedArgument));
CSF_FUNC(int, __fastcall, Hud_DestroyWidgetController, 0x00438280, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Hud_DestroyTextController, 0x00439AC0, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

static void Hud_DestroyOwnedObject(std::uint32_t& field,
                                   std::size_t slot = 0,
                                   std::size_t subobjectOffset = 0)
{
    const std::uint32_t object = field;
    if (object)
    {
        void* subobject = reinterpret_cast<std::uint8_t*>(object) + subobjectOffset;
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, subobject, 0)[slot])(subobject, nullptr, 1);
        field = 0;
    }
}

static void Hud_ReleaseReference(std::uint32_t& field)
{
    const std::uint32_t object = field;
    if (object)
    {
        ReleaseObjectReference(reinterpret_cast<std::uint8_t*>(object) + 4, nullptr);
        field = 0;
    }
}

// Keep the base and count as references: releasing a widget can change either.
static void Hud_ReleaseReferenceArray(const std::uint32_t& base,
                                      const std::uint32_t& count,
                                      std::uint32_t stride,
                                      std::uint32_t referenceOffset,
                                      bool clearElements = true)
{
    for (std::uint32_t index = 0; index < count; ++index)
    {
        const std::uint32_t object =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(base),
                   stride * index + referenceOffset);
        if (object)
        {
            ReleaseObjectReference(reinterpret_cast<std::uint8_t*>(object) + 4, nullptr);
            if (clearElements)
            {
                CSF_AT(std::uint32_t, reinterpret_cast<void*>(base),
                       stride * index + referenceOffset) = 0;
            }
        }
    }
}

static void Hud_FreeOwnedBlock(std::uint32_t& field)
{
    if (field)
    {
        Mem_Free(reinterpret_cast<void*>(field));
        field = 0;
    }
}

static int __fastcall CHud_Destroy(CHud* self, void* /*edx*/)
{
    const std::uint32_t renderHook = self->m_renderHook;
    self->vtbl = CSF_VTBL(0x00830A04);
    if (renderHook)
    {
        void* object = reinterpret_cast<void*>(renderHook);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->m_renderHook = 0;
    }

    Hud_UnregisterObject(reinterpret_cast<int>(self));

    Hud_DestroyOwnedObject(self->field_02c8);
    Hud_DestroyOwnedObject(self->field_026c);
    Hud_DestroyOwnedObject(self->field_0268);

    void* controller = reinterpret_cast<void*>(self->field_02d0);
    if (controller)
    {
        Hud_DestroyWidgetController(controller, nullptr);
        Mem_Free(controller);
        self->field_02d0 = 0;
    }

    void* auxiliaryPanel = reinterpret_cast<void*>(self->field_02cc);
    if (auxiliaryPanel)
    {
        Hud_DestroyAuxiliaryPanel(auxiliaryPanel, nullptr);
        Mem_Free(auxiliaryPanel);
        self->field_02cc = 0;
    }

    Hud_ReleaseReferenceArray(self->field_0004, self->field_000c, 4, 0);
    Hud_ReleaseReferenceArray(self->field_0014, self->field_001c, 4, 0);
    Hud_ReleaseReferenceArray(self->field_0024, self->field_002c, 4, 0);
    Hud_ReleaseReferenceArray(self->field_00c8, self->field_00d0, 8, 4);
    Hud_ReleaseReferenceArray(self->field_00d8, self->field_00e0, 8, 4);
    Hud_ReleaseReferenceArray(self->field_0108, self->field_0110, 8, 4);
    Hud_ReleaseReferenceArray(self->field_0044, self->field_004c, 4, 0);
    Hud_ReleaseReferenceArray(self->field_0054, self->field_005c, 4, 0);
    Hud_ReleaseReferenceArray(self->field_00ac, self->field_00b4, 4, 0);
    Hud_ReleaseReferenceArray(self->field_0064, self->field_006c, 4, 0);
    Hud_ReleaseReferenceArray(self->field_0084, self->field_008c, 4, 0);

    Hud_ReleaseReference(self->field_0104);
    Hud_ReleaseReference(self->field_00e8);
    Hud_ReleaseReference(self->field_00ec);

    Hud_DestroyOwnedObject(self->field_0100);
    Hud_DestroyOwnedObject(self->field_00c0);
    Hud_DestroyOwnedObject(self->field_00c4);

    std::uint32_t widgetIndex = 0;
    std::uint32_t widgetOffset = 0;
    for (;;)
    {
        const std::uint32_t begin = self->field_015c;
        if (!begin ||
            widgetIndex >= static_cast<std::uint32_t>(
                static_cast<std::int32_t>(self->field_0160 - begin) / 48))
        {
            break;
        }

        const std::uint32_t object =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_015c),
                   widgetOffset + 28);
        if (object)
        {
            ReleaseObjectReference(reinterpret_cast<std::uint8_t*>(object) + 4, nullptr);
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_015c),
                   widgetOffset + 28) = 0;
        }
        ++widgetIndex;
        widgetOffset += 48;
    }

    auto* ownedObjects = reinterpret_cast<std::uint8_t*>(self) + 0x168;
    for (int group = 0; group < 4; ++group)
    {
        for (int element = 0; element < 8; ++element)
        {
            Hud_DestroyOwnedObject(CSF_AT(std::uint32_t, ownedObjects, 0));
            ownedObjects += 4;
        }
    }

    Hud_ReleaseReferenceArray(self->field_0140, self->field_0148, 12, 0);
    Hud_ReleaseReferenceArray(self->field_01e8, self->field_01f0, 4, 0);
    Hud_ReleaseReferenceArray(self->field_01f8, self->field_0200, 4, 0);
    Hud_DestroyOwnedObject(self->field_0208);

    // These entries are released without being cleared.
    Hud_ReleaseReferenceArray(self->field_0288, self->field_0290, 4, 0, false);

    if (self->field_0127)
    {
        const std::uint32_t object =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0074), 12);
        if (object)
        {
            ReleaseObjectReference(reinterpret_cast<std::uint8_t*>(object) + 4, nullptr);
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0074), 12) = 0;
        }
    }

    Hud_DestroyOwnedObject(self->field_0244);
    Hud_DestroyOwnedObject(self->m_textLabel);
    Hud_DestroyOwnedObject(self->field_0250);
    Hud_DestroyOwnedObject(self->field_0248, 0, 4);

    const std::uint32_t pairBegin = self->field_00a0;
    const std::uint32_t pairCount = pairBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_00a4 - pairBegin) >> 3)
        : 0;
    std::uint32_t pairIndex = 0;
    for (; pairIndex < pairCount; ++pairIndex)
    {
        const std::uint32_t object =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_00a0),
                   8 * pairIndex);
        if (object)
        {
            ReleaseObjectReference(reinterpret_cast<std::uint8_t*>(object) + 4, nullptr);
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_00a0),
                   8 * pairIndex) = 0;
        }
    }

    void* textController = reinterpret_cast<void*>(self->field_024c);
    if (textController)
    {
        Hud_DestroyTextController(textController, nullptr);
        Mem_Free(textController);
        self->field_024c = 0;
    }

    Hud_DestroyOwnedObject(self->m_letterboxBottom);
    Hud_DestroyOwnedObject(self->m_letterboxTop);
    Hud_DestroyOwnedObject(self->m_subtitlePanel);
    Hud_DestroyOwnedObject(self->field_0298);

    const std::uint32_t reference = self->field_029c;
    if (reference)
        ReleaseObjectReference(reinterpret_cast<std::uint8_t*>(reference) + 4, nullptr);

    if (self->field_0310)
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(self->field_030c));

    Hud_FreeOwnedBlock(self->field_0288);
    Hud_FreeOwnedBlock(self->field_0274);
    Hud_FreeOwnedBlock(self->field_022c);

    CSF_AT(void*, self, 0x21C) = CSF_VTBL(0x0082D410);

    Hud_FreeOwnedBlock(self->field_01f8);
    Hud_FreeOwnedBlock(self->field_01e8);

    int* widgets = reinterpret_cast<int*>(self->field_015c);
    if (widgets)
    {
        Hud_ProcessWidgetPair(
            widgets, reinterpret_cast<int*>(self->field_0160),
            reinterpret_cast<std::uintptr_t>(
                reinterpret_cast<std::uint8_t*>(self) + 0x158),
            reinterpret_cast<int*>(pairIndex));
        void* block = reinterpret_cast<void*>(self->field_015c);
        if (block)
            Mem_FreeDebug(block);
    }
    self->field_015c = 0;
    self->field_0160 = 0;
    self->field_0164 = 0;

    Hud_FreeOwnedBlock(self->field_0140);
    Hud_FreeOwnedBlock(self->field_0108);
    Hud_FreeOwnedBlock(self->field_00d8);
    Hud_FreeOwnedBlock(self->field_00c8);
    Hud_FreeOwnedBlock(self->field_00ac);

    if (self->field_00a0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_00a0));
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;

    Hud_FreeOwnedBlock(self->field_0084);
    Hud_FreeOwnedBlock(self->field_0074);
    Hud_FreeOwnedBlock(self->field_0064);
    Hud_FreeOwnedBlock(self->field_0054);
    Hud_FreeOwnedBlock(self->field_0044);
    Hud_FreeOwnedBlock(self->field_0034);
    Hud_FreeOwnedBlock(self->field_0024);
    Hud_FreeOwnedBlock(self->field_0014);

    int result = static_cast<int>(self->field_0004);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x00434800, CHud_Destroy, "auto");
