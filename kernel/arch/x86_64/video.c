#include <kernel/video.h>
#include "video.h"

struct limine_framebuffer* _frameBuffer;
volatile uint32_t* fbAddress;

int32_t video_initialize(struct limine_framebuffer* framebuffer) {
    _frameBuffer = framebuffer;

    if (_frameBuffer->bpp != 32)
        return -1;

    fbAddress = (uint32_t *)_frameBuffer->address;

    return 0;
}

void video_putPixel(uint32_t x, uint32_t y, uint32_t pixelData)
{
    fbAddress[y * (_frameBuffer->pitch / 4) + x] = pixelData;
}

void video_setPixel(uint32_t x, uint32_t y, struct pixel32 pixelData)
{
    fbAddress[y * (_frameBuffer->pitch / 4) + x] = *(uint32_t*)&pixelData;
}
