#include <kernel/video.h>
#include <stddef.h>

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

    return 0;
}

uint32_t video_readPixel(uint32_t x, uint32_t y)
{
    if (x >= _frameBuffer->width || y >= _frameBuffer->height)
        return 0;
    return fbAddress[y * (_frameBuffer->pitch / 4) + x];
}

struct pixel32 video_getPixel(uint32_t x, uint32_t y)
{
    if (x >= _frameBuffer->width || y >= _frameBuffer->height)
        return (struct pixel32){0};
    return *(const struct pixel32 *)(&fbAddress[y * (_frameBuffer->pitch / 4) + x]);
}

void video_putPixel(uint32_t x, uint32_t y, uint32_t pixelData)
{
    if (x >= _frameBuffer->width || y >= _frameBuffer->height)
        return;

    fbAddress[y * (_frameBuffer->pitch / 4) + x] = pixelData;
}

void video_setPixel(uint32_t x, uint32_t y, struct pixel32 pixelData)
{
    if (x >= _frameBuffer->width || y >= _frameBuffer->height)
        return;
    fbAddress[y * (_frameBuffer->pitch / 4) + x] = *(uint32_t*)&pixelData;
}

void video_clear()
{
	for (size_t y = 0; y < _frameBuffer->height; y++) {
		for (size_t x = 0; x < _frameBuffer->width; x++) {
			fbAddress[y * (_frameBuffer->pitch / 4) + x] = 0;
		}
	}
}

uint64_t video_getWidth()
{
    return _frameBuffer->width;
}

uint64_t video_getHeight()
{
    return _frameBuffer->height;
}
