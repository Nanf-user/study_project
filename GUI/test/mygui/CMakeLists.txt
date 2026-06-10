SET(SOURCES screens/ui_Screen1.c
    screens/ui_Screen2.c
    screens/ui_Screen3.c
    screens/ui_Screen4.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_image_xiaoyu_png.c
    images/ui_img_image_qing_png.c
    images/ui_img_image_duoyun_png.c
    fonts/ui_font_myfont.c
    wifi_config.c)

add_library(ui ${SOURCES})
