#ifndef _KERNEL_FONT_H
#define _KERNEL_FONT_H

#include <stddef.h>
#include <stdint.h>

extern const uint8_t _binary_font_psf_start[];
extern const uint8_t _binary_font_psf_end[];
extern const uint8_t _binary_font_psf_size[];

#define PSF2_MAGIC 0x864ab572

struct psf2_header
{
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
} __attribute__((packed));

// Get raw pointer to font data
const uint8_t *font_data(void);

// Get size of font blob
const size_t font_dataSize(void);

// get font header
const struct psf2_header *font_header(void);

// Get glyph table start after header
const uint8_t *font_glyphs(void);

// Glyph width
const uint32_t font_getWidth();

// Glyph height
const uint32_t font_getHeight();

// Draw a character
void font_drawChar(int x, int y, char c, uint32_t fg, uint32_t bg, uint32_t scale);

#endif