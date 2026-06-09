#ifndef _KERNEL_VIDEO_H
#define _KERNEL_VIDEO_H

#include <limine.h>
#include <stdint.h>

// Assumes each pixel is 32-bits and the last 8-bits are unused
struct pixel32
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t x;
};


void video_initialize(struct limine_framebuffer framebuffer);
void video_setPixel(int32_t x, int32_t y, struct pixel32 pixelData);
void video_putPixel(int32_t x, int32_t y, uint32_t pixelData);

#endif