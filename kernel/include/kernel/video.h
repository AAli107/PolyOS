#ifndef _KERNEL_VIDEO_H
#define _KERNEL_VIDEO_H

#include <limine.h>
#include <stdint.h>
#include <stdbool.h>

// Assumes each pixel is 32-bits and the last 8-bits are unused
// If bits per pixel is not 32, the video_initialize will reject it
struct pixel32
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t x;
};

int32_t video_initialize(struct limine_framebuffer_request* framebufferRequest);
uint32_t video_readPixel(uint32_t x, uint32_t y);
struct pixel32 video_getPixel(uint32_t x, uint32_t y);
void video_putPixel(uint32_t x, uint32_t y, uint32_t pixelData);
void video_setPixel(uint32_t x, uint32_t y, struct pixel32 pixelData);
void video_clear();
void video_setEnableBlend(bool enableBlend);
bool video_isBlendEnabled();
uint64_t video_getWidth();
uint64_t video_getHeight();
uint32_t video_color_blend(uint32_t dst, uint32_t src);
struct pixel32 video_pixel32_blend(struct pixel32 dst, struct pixel32 src);

#endif