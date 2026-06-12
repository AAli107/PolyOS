#include <kernel/font.h>
#include <kernel/video.h>

const uint8_t *font_data(void)
{
    return _binary_font_psf_start;
}

const size_t font_dataSize(void)
{
    return (size_t)(_binary_font_psf_end - _binary_font_psf_start);
}

const struct psf2_header *font_header(void)
{
    return (struct psf2_header *)_binary_font_psf_start;
}

const uint8_t *font_glyphs(void)
{
    return (const uint8_t *)_binary_font_psf_start + font_header()->headersize;
}

const uint32_t font_getWidth() {
    return font_header()->width;
}

const uint32_t font_getHeight() {
    return font_header()->height;
}

void font_drawChar(int x, int y, char c, uint32_t fg, uint32_t bg, uint32_t scale)
{
    if (scale < 1) scale = 1;

    struct psf2_header *hdr = font_header();
    if (!hdr || hdr->magic != PSF2_MAGIC)
        return;

    uint32_t height = hdr->height;
    uint32_t width = hdr->width;
    uint32_t bytesperglyph = hdr->bytesperglyph;
    
    uint16_t ch = (uint8_t)c;
    
    ch %= hdr->numglyph;

    const uint8_t *glyphs = font_glyphs();
    const uint8_t *glyph = glyphs + ((size_t)ch * bytesperglyph);

    int s = (int)scale;
    int pitch = (width + 7) / 8;

    for (int row = 0; row < (int)height; row++)  
    {
        int yPos = y + (row * s);

        for (int col = 0; col < (int)width; col++)
        {
            uint8_t line = glyph[row * pitch + (col / 8)];
            uint32_t color = (line & (0x80 >> (col % 8))) ? fg : bg;
            
            int xPos = x + (col * s);

            for (int i = 0; i < s; i++)
            {
                for (int j = 0; j < s; j++)
                {
                    video_putPixel(xPos + j, yPos + i, color);
                }
            }
        }
    }
}
