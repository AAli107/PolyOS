#include <video.h>

volatile uint32_t* fb_ptr;

void video_initialize(struct limine_framebuffer framebuffer) {
    fb_ptr = framebuffer.address;
}