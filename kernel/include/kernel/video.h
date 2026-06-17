#ifndef _KERNEL_VIDEO_H
#define _KERNEL_VIDEO_H

#include <limine.h>
#include <stdint.h>
#include <stdbool.h>

// Assumes each pixel is 32-bits and the last 8-bits are unused
// If bits per pixel is not 32, the video_initialize will reject it
struct color32
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
};

// run at kernal_main to initialize the video rendering
int32_t video_initialize(struct limine_framebuffer_request* framebufferRequest);

// Read pixel at xy position, returns uint32_t
uint32_t video_readPixel(uint32_t x, uint32_t y);

// Read pixel at xy position, returns color32
struct color32 video_getPixel(uint32_t x, uint32_t y);

// Put pixel at xy position, requires a uint32_t
void video_putPixel(uint32_t x, uint32_t y, uint32_t pixelData);

// Put pixel at xy position, requires a color32
void video_setPixel(uint32_t x, uint32_t y, struct color32 pixelData);

// Clears the entire screen
void video_clear();

// Allows you to turn on or off alpha blending
void video_setEnableBlend(bool enableBlend);

// Returns true if alpha blending is turned on
bool video_isBlendEnabled();

// Returns screen width in pixels
uint64_t video_getWidth();

// Returns screen height in pixels
uint64_t video_getHeight();

// Blends two colors, which accepts and recieves uint32_t
uint32_t video_uint32_blend(uint32_t dst, uint32_t src);

// Blends two colors, which accepts and recieves color32
struct color32 video_color32_blend(struct color32 dst, struct color32 src);

bool video_isInit();

#endif