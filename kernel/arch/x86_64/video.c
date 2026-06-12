#include <kernel/video.h>
#include <stddef.h>

static bool _enableBlend;
struct limine_framebuffer* _frameBuffer;
volatile uint32_t* fbAddress;

int32_t video_initialize(struct limine_framebuffer_request* framebufferRequest) {

    // Tell the OS to kill itself when it does not have the framebuffer or if it did not recieve the framebuffer request
    if (framebufferRequest == NULL || framebufferRequest->response == NULL || framebufferRequest->response->framebuffer_count < 1)
        return -1;

    _frameBuffer = framebufferRequest->response->framebuffers[0];

    // Tell the OS to kill itself if bits per pixel is not 32
    if (_frameBuffer->bpp != 32)
        return -1;

    fbAddress = (uint32_t *)_frameBuffer->address;

    video_clear();
    
    video_setEnableBlend(false);

    return 0;
}

uint32_t video_readPixel(uint32_t x, uint32_t y)
{
    if (x >= _frameBuffer->width || y >= _frameBuffer->height)
        return 0;
    return fbAddress[y * (_frameBuffer->pitch / 4) + x];
}

struct color32 video_getPixel(uint32_t x, uint32_t y)
{
    return *(const struct color32 *)video_readPixel(x, y);
}

void video_putPixel(uint32_t x, uint32_t y, uint32_t pixelData)
{
    if (x >= _frameBuffer->width || y >= _frameBuffer->height)
        return;
    if (_enableBlend) {
        uint32_t finalColor = video_uint32_blend(video_readPixel(x, y), pixelData);
        fbAddress[y * (_frameBuffer->pitch / 4) + x] = finalColor;
    }
    else {
        fbAddress[y * (_frameBuffer->pitch / 4) + x] = pixelData;
    }
}

void video_setPixel(uint32_t x, uint32_t y, struct color32 pixelData)
{
    video_putPixel(x, y, *(uint32_t*)&pixelData);
}

void video_clear()
{
	for (size_t y = 0; y < _frameBuffer->height; y++) {
		for (size_t x = 0; x < _frameBuffer->width; x++) {
			fbAddress[y * (_frameBuffer->pitch / 4) + x] = 0;
		}
	}
}

void video_setEnableBlend(bool enableBlend)
{
    _enableBlend = enableBlend;
}

bool video_isBlendEnabled()
{
    return _enableBlend;
}

uint64_t video_getWidth()
{
    return _frameBuffer->width;
}

uint64_t video_getHeight()
{
    return _frameBuffer->height;
}

uint32_t video_uint32_blend(uint32_t dst, uint32_t src)
{
    uint32_t alpha = (src >> 24) & 0xFF;

    if (alpha == 255) return src;
    if (alpha == 0)   return dst;

    uint32_t inv_alpha = 255 - alpha;

    uint32_t b = (((src >> 0)  & 0xFF) * alpha + ((dst >> 0)  & 0xFF) * inv_alpha + 255) >> 8;
    uint32_t g = (((src >> 8)  & 0xFF) * alpha + ((dst >> 8)  & 0xFF) * inv_alpha + 255) >> 8;
    uint32_t r = (((src >> 16) & 0xFF) * alpha + ((dst >> 16) & 0xFF) * inv_alpha + 255) >> 8;

    return b | (g << 8) | (r << 16) | (255 << 24);
}

struct color32 video_color32_blend(struct color32 dst, struct color32 src)
{
    return *(const struct color32 *)video_uint32_blend(*(uint32_t*)&dst, *(uint32_t*)&src);
}
