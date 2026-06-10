/*******************************************************************************
 * Size: 20 px
 * Bpp: 1
 * Opts: --bpp 1 --size 20 --font G:/stm32_project/lvgl_project/GUI/test/assets/FONT/iconfont.ttf -o G:/stm32_project/lvgl_project/GUI/test/assets/FONT\ui_font_myfont.c --format lvgl -r 0xe600 -r 0xec4a -r 0xec17 -r 0xe76c -r 0xe63f -r 0xe634 -r 0xe873 --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_MYFONT
#define UI_FONT_MYFONT 1
#endif

#if UI_FONT_MYFONT

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+E600 "" */
    0x3, 0x0, 0x7, 0x0, 0xf, 0x0, 0x1f, 0x80,
    0x33, 0x80, 0x61, 0x80, 0xc1, 0x81, 0x82, 0x3,
    0x0, 0x6, 0x0, 0xc, 0x0, 0x18, 0x7, 0xf0,
    0x1f, 0xe0, 0x3f, 0xc0, 0x7f, 0x80, 0xfe, 0x0,
    0x78, 0x0,

    /* U+E634 "" */
    0x0, 0x78, 0x0, 0x33, 0x0, 0xf8, 0x40, 0x67,
    0x10, 0x30, 0x5f, 0x8, 0x1c, 0xe2, 0x2, 0x19,
    0x80, 0x86, 0x80, 0x0, 0xe0, 0x0, 0x18, 0x0,
    0x5, 0x0, 0x2, 0x3f, 0xff, 0x0,

    /* U+E63F "" */
    0x2, 0x20, 0x39, 0x13, 0xb0, 0x88, 0x70, 0x0,
    0x18, 0x0, 0xc, 0x0, 0x7, 0xff, 0xff, 0x0,
    0x1, 0x87, 0xe0, 0xc0, 0x10, 0x60, 0x10, 0x30,
    0x8, 0x18, 0x8, 0xc, 0x4, 0x6, 0x2, 0x3,
    0x0, 0x1, 0xc0, 0x1, 0xbf, 0xff, 0x80,

    /* U+E76C "" */
    0x3, 0xfc, 0x1, 0xff, 0xf8, 0x3e, 0x7, 0xef,
    0x0, 0xf, 0x60, 0x0, 0x60, 0x3f, 0xc0, 0x7,
    0xfe, 0x0, 0xff, 0xf0, 0x6, 0x6, 0x0, 0x0,
    0x0, 0x0, 0xf0, 0x0, 0x1f, 0x80, 0x0, 0xf0,
    0x0, 0x6, 0x0,

    /* U+E873 "" */
    0x0, 0x0, 0x0, 0xc0, 0x0, 0x70, 0x0, 0x3e,
    0x0, 0x1b, 0x82, 0xc, 0xf3, 0xc6, 0x1e, 0x7b,
    0x3c, 0x1f, 0xbc, 0x3, 0xf8, 0x0, 0x70, 0x0,
    0xfe, 0x1, 0xfb, 0xc1, 0xec, 0xf3, 0xc6, 0x1e,
    0xc3, 0x3c, 0x1, 0xb8, 0x0, 0xf8, 0x0, 0x70,
    0x0, 0x30, 0x0, 0x0, 0x0,

    /* U+EC17 "" */
    0xfc, 0x0, 0x21, 0x80, 0x8, 0x20, 0x2, 0xf,
    0xff, 0x80, 0x0, 0x60, 0x0, 0x1f, 0xff, 0xfe,
    0x0, 0x1, 0x80, 0x0, 0x60, 0x0, 0x18, 0x0,
    0x6, 0x0, 0x1, 0x80, 0x0, 0x60, 0x0, 0x18,
    0x0, 0x6, 0x0, 0x1, 0xff, 0xff, 0xc0,

    /* U+EC4A "" */
    0x8, 0x0, 0x18, 0x0, 0x38, 0x0, 0x5c, 0x8,
    0x88, 0x59, 0x34, 0x9a, 0xc4, 0x9f, 0x9, 0x1c,
    0x12, 0x38, 0x24, 0xf8, 0x8b, 0x58, 0x1c, 0x98,
    0x51, 0x30, 0x82, 0xc0, 0x7, 0x0, 0xc, 0x0,
    0x10, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 320, .box_w = 15, .box_h = 18, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 34, .adv_w = 320, .box_w = 18, .box_h = 13, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 64, .adv_w = 320, .box_w = 17, .box_h = 18, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 103, .adv_w = 320, .box_w = 20, .box_h = 14, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 138, .adv_w = 320, .box_w = 17, .box_h = 21, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 183, .adv_w = 320, .box_w = 18, .box_h = 17, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 222, .adv_w = 320, .box_w = 15, .box_h = 18, .ofs_x = 2, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x34, 0x3f, 0x16c, 0x273, 0x617, 0x64a
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 58880, .range_length = 1611, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 7, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_myfont = {
#else
lv_font_t ui_font_myfont = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 21,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_MYFONT*/

