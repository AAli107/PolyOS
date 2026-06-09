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


int32_t video_initialize(struct limine_framebuffer_request* framebufferRequest);
void video_putPixel(uint32_t x, uint32_t y, uint32_t pixelData);
void video_setPixel(uint32_t x, uint32_t y, struct pixel32 pixelData);
void video_clear();
uint64_t video_getWidth();
uint64_t video_getHeight();

#endif