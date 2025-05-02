cmake_minimum_required(VERSION 3.12)
project(lvgl VERSION 9.3.0 LANGUAGES C)

option(LV_CONF_INCLUDE_SIMPLE "Include lvgl configuration as simple header" OFF)
option(LV_CONF_PATH "Path to LVGL configuration file" "")

# Core components
set(LVGL_CORE_SOURCES
    third_party/lvgl/src/lv_init.c
    third_party/lvgl/src/core/lv_group.c
    third_party/lvgl/src/core/lv_obj.c
    third_party/lvgl/src/core/lv_obj_class.c
    third_party/lvgl/src/core/lv_obj_draw.c
    third_party/lvgl/src/core/lv_obj_event.c
    third_party/lvgl/src/core/lv_obj_id_builtin.c
    third_party/lvgl/src/core/lv_obj_pos.c
    third_party/lvgl/src/core/lv_obj_property.c
    third_party/lvgl/src/core/lv_obj_scroll.c
    third_party/lvgl/src/core/lv_obj_style.c
    third_party/lvgl/src/core/lv_obj_style_gen.c
    third_party/lvgl/src/core/lv_obj_tree.c
    third_party/lvgl/src/core/lv_refr.c
)

# Display components
set(LVGL_DISPLAY_SOURCES
    third_party/lvgl/src/display/lv_display.c
)

# Draw components
set(LVGL_DRAW_SOURCES
    third_party/lvgl/src/draw/lv_draw.c
    third_party/lvgl/src/draw/lv_draw_arc.c
    third_party/lvgl/src/draw/lv_draw_buf.c
    third_party/lvgl/src/draw/lv_draw_image.c
    third_party/lvgl/src/draw/lv_draw_label.c
    third_party/lvgl/src/draw/lv_draw_line.c
    third_party/lvgl/src/draw/lv_draw_mask.c
    third_party/lvgl/src/draw/lv_draw_rect.c
    third_party/lvgl/src/draw/lv_draw_triangle.c
    third_party/lvgl/src/draw/lv_draw_vector.c
    third_party/lvgl/src/draw/lv_image_decoder.c
)

# Draw DMA2D components
set(LVGL_DRAW_DMA2D_SOURCES
    third_party/lvgl/src/draw/dma2d/lv_draw_dma2d.c
    third_party/lvgl/src/draw/dma2d/lv_draw_dma2d_fill.c
    third_party/lvgl/src/draw/dma2d/lv_draw_dma2d_img.c
)

# Draw NXP components
set(LVGL_DRAW_NXP_SOURCES
    third_party/lvgl/src/draw/nxp/g2d/lv_draw_buf_g2d.c
    third_party/lvgl/src/draw/nxp/g2d/lv_draw_g2d.c
    third_party/lvgl/src/draw/nxp/g2d/lv_draw_g2d_fill.c
    third_party/lvgl/src/draw/nxp/g2d/lv_draw_g2d_img.c
    third_party/lvgl/src/draw/nxp/g2d/lv_g2d_buf_map.c
    third_party/lvgl/src/draw/nxp/g2d/lv_g2d_utils.c
    third_party/lvgl/src/draw/nxp/pxp/lv_draw_buf_pxp.c
    third_party/lvgl/src/draw/nxp/pxp/lv_draw_pxp.c
    third_party/lvgl/src/draw/nxp/pxp/lv_draw_pxp_fill.c
    third_party/lvgl/src/draw/nxp/pxp/lv_draw_pxp_img.c
    third_party/lvgl/src/draw/nxp/pxp/lv_draw_pxp_layer.c
    third_party/lvgl/src/draw/nxp/pxp/lv_pxp_cfg.c
    third_party/lvgl/src/draw/nxp/pxp/lv_pxp_osa.c
    third_party/lvgl/src/draw/nxp/pxp/lv_pxp_utils.c
    third_party/lvgl/src/draw/nxp/vglite/lv_draw_buf_vglite.c
    third_party/lvgl/src/draw/nxp/vglite/lv_draw_vglite.c
    third_party/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.c
    third_party/lvgl/src/draw/nxp/vglite/lv_draw_vglite_border.c
    third_party/lvgl/src/draw/nxp/vglite/lv_draw_vglite_fill.c
    third_party/lvgl/src/draw/nxp/vglite/lv_draw_vglite_img.c
    third_party/lvgl/src/draw/nxp/vglite/lv_draw_vglite_label.c
    third_party/lvgl/src/draw/nxp/vglite/lv_draw_vglite_layer.c
    third_party/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.c
    third_party/lvgl/src/draw/nxp/vglite/lv_draw_vglite_triangle.c
    third_party/lvgl/src/draw/nxp/vglite/lv_vglite_buf.c
    third_party/lvgl/src/draw/nxp/vglite/lv_vglite_matrix.c
    third_party/lvgl/src/draw/nxp/vglite/lv_vglite_path.c
    third_party/lvgl/src/draw/nxp/vglite/lv_vglite_utils.c
)

# Draw Nema GFX components
set(LVGL_DRAW_NEMA_GFX_SOURCES
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx.c
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_arc.c
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_border.c
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_fill.c
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_img.c
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_label.c
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_layer.c
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_line.c
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.c
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.c
    third_party/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_utils.c
    third_party/lvgl/src/draw/nema_gfx/lv_nema_gfx_path.c
)

# Draw OpenGLES components
set(LVGL_DRAW_OPENGLES_SOURCES
    third_party/lvgl/src/draw/opengles/lv_draw_opengles.c
)

# Draw Renesas components
set(LVGL_DRAW_RENESAS_SOURCES
    third_party/lvgl/src/draw/renesas/dave2d/lv_draw_dave2d.c
    third_party/lvgl/src/draw/renesas/dave2d/lv_draw_dave2d_arc.c
    third_party/lvgl/src/draw/renesas/dave2d/lv_draw_dave2d_border.c
    third_party/lvgl/src/draw/renesas/dave2d/lv_draw_dave2d_fill.c
    third_party/lvgl/src/draw/renesas/dave2d/lv_draw_dave2d_image.c
    third_party/lvgl/src/draw/renesas/dave2d/lv_draw_dave2d_label.c
    third_party/lvgl/src/draw/renesas/dave2d/lv_draw_dave2d_line.c
    third_party/lvgl/src/draw/renesas/dave2d/lv_draw_dave2d_mask_rectangle.c
    third_party/lvgl/src/draw/renesas/dave2d/lv_draw_dave2d_triangle.c
    third_party/lvgl/src/draw/renesas/dave2d/lv_draw_dave2d_utils.c
)

# Draw SDL components
set(LVGL_DRAW_SDL_SOURCES
    third_party/lvgl/src/draw/sdl/lv_draw_sdl.c
)

# Draw SW (Software rendering) components
set(LVGL_DRAW_SW_SOURCES
    third_party/lvgl/src/draw/sw/lv_draw_sw.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_arc.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_border.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_box_shadow.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_fill.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_grad.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_img.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_letter.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_line.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_mask.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_mask_rect.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_transform.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_triangle.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_utils.c
    third_party/lvgl/src/draw/sw/lv_draw_sw_vector.c
)

# Draw SW Blend components
set(LVGL_DRAW_SW_BLEND_SOURCES
    third_party/lvgl/src/draw/sw/blend/lv_draw_sw_blend.c
    third_party/lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_al88.c
    third_party/lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_argb8888.c
    third_party/lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_argb8888_premultiplied.c
    third_party/lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_i1.c
    third_party/lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_l8.c
    third_party/lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb565.c
    third_party/lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb888.c
)

# Draw VG Lite components
set(LVGL_DRAW_VG_LITE_SOURCES
    third_party/lvgl/src/draw/vg_lite/lv_draw_buf_vg_lite.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_arc.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_border.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_box_shadow.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_fill.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_img.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_label.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_layer.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_line.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_mask_rect.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_triangle.c
    third_party/lvgl/src/draw/vg_lite/lv_draw_vg_lite_vector.c
    third_party/lvgl/src/draw/vg_lite/lv_vg_lite_decoder.c
    third_party/lvgl/src/draw/vg_lite/lv_vg_lite_grad.c
    third_party/lvgl/src/draw/vg_lite/lv_vg_lite_math.c
    third_party/lvgl/src/draw/vg_lite/lv_vg_lite_path.c
    third_party/lvgl/src/draw/vg_lite/lv_vg_lite_pending.c
    third_party/lvgl/src/draw/vg_lite/lv_vg_lite_stroke.c
    third_party/lvgl/src/draw/vg_lite/lv_vg_lite_utils.c
)

# Drivers components
set(LVGL_DRIVERS_SOURCES
    # Display drivers
    third_party/lvgl/src/drivers/display/st7735/lv_st7735.c
)

# Font components
set(LVGL_FONT_SOURCES
    third_party/lvgl/src/font/lv_binfont_loader.c
    third_party/lvgl/src/font/lv_font.c
    third_party/lvgl/src/font/lv_font_dejavu_16_persian_hebrew.c
    third_party/lvgl/src/font/lv_font_fmt_txt.c
    third_party/lvgl/src/font/lv_font_montserrat_10.c
    third_party/lvgl/src/font/lv_font_montserrat_12.c
    third_party/lvgl/src/font/lv_font_montserrat_14.c
    third_party/lvgl/src/font/lv_font_montserrat_16.c
    third_party/lvgl/src/font/lv_font_montserrat_18.c
    third_party/lvgl/src/font/lv_font_montserrat_20.c
    third_party/lvgl/src/font/lv_font_montserrat_22.c
    third_party/lvgl/src/font/lv_font_montserrat_24.c
    third_party/lvgl/src/font/lv_font_montserrat_26.c
    third_party/lvgl/src/font/lv_font_montserrat_28.c
    third_party/lvgl/src/font/lv_font_montserrat_28_compressed.c
    third_party/lvgl/src/font/lv_font_montserrat_30.c
    third_party/lvgl/src/font/lv_font_montserrat_32.c
    third_party/lvgl/src/font/lv_font_montserrat_34.c
    third_party/lvgl/src/font/lv_font_montserrat_36.c
    third_party/lvgl/src/font/lv_font_montserrat_38.c
    third_party/lvgl/src/font/lv_font_montserrat_40.c
    third_party/lvgl/src/font/lv_font_montserrat_42.c
    third_party/lvgl/src/font/lv_font_montserrat_44.c
    third_party/lvgl/src/font/lv_font_montserrat_46.c
    third_party/lvgl/src/font/lv_font_montserrat_48.c
    third_party/lvgl/src/font/lv_font_montserrat_8.c
    third_party/lvgl/src/font/lv_font_simsun_14_cjk.c
    third_party/lvgl/src/font/lv_font_simsun_16_cjk.c
    third_party/lvgl/src/font/lv_font_unscii_16.c
    third_party/lvgl/src/font/lv_font_unscii_8.c
)

# Input components
set(LVGL_INDEV_SOURCES
    third_party/lvgl/src/indev/lv_indev.c
    third_party/lvgl/src/indev/lv_indev_gesture.c
    third_party/lvgl/src/indev/lv_indev_scroll.c
)

# Layout components
set(LVGL_LAYOUT_SOURCES
    third_party/lvgl/src/layouts/flex/lv_flex.c
    third_party/lvgl/src/layouts/grid/lv_grid.c
    third_party/lvgl/src/layouts/lv_layout.c
)

# Libraries components
set(LVGL_LIBS_SOURCES
    # Barcode
    third_party/lvgl/src/libs/barcode/code128.c
    third_party/lvgl/src/libs/barcode/lv_barcode.c
    
    # Binary decoder
    third_party/lvgl/src/libs/bin_decoder/lv_bin_decoder.c
    
    # BMP
    third_party/lvgl/src/libs/bmp/lv_bmp.c
    
    # Expat
    third_party/lvgl/src/libs/expat/xmlparse.c
    third_party/lvgl/src/libs/expat/xmlrole.c
    third_party/lvgl/src/libs/expat/xmltok.c
    third_party/lvgl/src/libs/expat/xmltok_impl.c
    third_party/lvgl/src/libs/expat/xmltok_ns.c
    
    # FFmpeg
    third_party/lvgl/src/libs/ffmpeg/lv_ffmpeg.c
    
    # FreeType
    third_party/lvgl/src/libs/freetype/lv_freetype.c
    third_party/lvgl/src/libs/freetype/lv_freetype_glyph.c
    third_party/lvgl/src/libs/freetype/lv_freetype_image.c
    third_party/lvgl/src/libs/freetype/lv_freetype_outline.c
    third_party/lvgl/src/libs/freetype/lv_ftsystem.c
    
    # FS drivers
    third_party/lvgl/src/libs/fsdrv/lv_fs_cbfs.c
    third_party/lvgl/src/libs/fsdrv/lv_fs_fatfs.c
    third_party/lvgl/src/libs/fsdrv/lv_fs_littlefs.c
    third_party/lvgl/src/libs/fsdrv/lv_fs_memfs.c
    third_party/lvgl/src/libs/fsdrv/lv_fs_posix.c
    third_party/lvgl/src/libs/fsdrv/lv_fs_stdio.c
    third_party/lvgl/src/libs/fsdrv/lv_fs_uefi.c
    third_party/lvgl/src/libs/fsdrv/lv_fs_win32.c
    
    # GIF
    third_party/lvgl/src/libs/gif/gifdec.c
    third_party/lvgl/src/libs/gif/lv_gif.c
    
    # JPEG Turbo
    third_party/lvgl/src/libs/libjpeg_turbo/lv_libjpeg_turbo.c
    
    # PNG
    third_party/lvgl/src/libs/libpng/lv_libpng.c
    
    # LodePNG
    third_party/lvgl/src/libs/lodepng/lodepng.c
    third_party/lvgl/src/libs/lodepng/lv_lodepng.c
    
    # LZ4
    third_party/lvgl/src/libs/lz4/lz4.c
    
    # QR Code
    third_party/lvgl/src/libs/qrcode/lv_qrcode.c
    third_party/lvgl/src/libs/qrcode/qrcodegen.c
    
    # RLE
    third_party/lvgl/src/libs/rle/lv_rle.c
    
    # Rlottie
    third_party/lvgl/src/libs/rlottie/lv_rlottie.c
    
    # SVG
    third_party/lvgl/src/libs/svg/lv_svg.c
    third_party/lvgl/src/libs/svg/lv_svg_decoder.c
    third_party/lvgl/src/libs/svg/lv_svg_parser.c
    third_party/lvgl/src/libs/svg/lv_svg_render.c
    third_party/lvgl/src/libs/svg/lv_svg_token.c
    
    # Tiny TTF
    third_party/lvgl/src/libs/tiny_ttf/lv_tiny_ttf.c
    
    # TJPGD
    third_party/lvgl/src/libs/tjpgd/lv_tjpgd.c
    third_party/lvgl/src/libs/tjpgd/tjpgd.c
)

# Misc components
set(LVGL_MISC_SOURCES
    third_party/lvgl/src/misc/lv_anim.c
    third_party/lvgl/src/misc/lv_anim_timeline.c
    third_party/lvgl/src/misc/lv_area.c
    third_party/lvgl/src/misc/lv_array.c
    third_party/lvgl/src/misc/lv_async.c
    third_party/lvgl/src/misc/lv_bidi.c
    third_party/lvgl/src/misc/lv_circle_buf.c
    third_party/lvgl/src/misc/lv_color.c
    third_party/lvgl/src/misc/lv_color_op.c
    third_party/lvgl/src/misc/lv_event.c
    third_party/lvgl/src/misc/lv_fs.c
    third_party/lvgl/src/misc/lv_grad.c
    third_party/lvgl/src/misc/lv_iter.c
    third_party/lvgl/src/misc/lv_ll.c
    third_party/lvgl/src/misc/lv_log.c
    third_party/lvgl/src/misc/lv_lru.c
    third_party/lvgl/src/misc/lv_math.c
    third_party/lvgl/src/misc/lv_matrix.c
    third_party/lvgl/src/misc/lv_palette.c
    third_party/lvgl/src/misc/lv_profiler_builtin.c
    third_party/lvgl/src/misc/lv_rb.c
    third_party/lvgl/src/misc/lv_style.c
    third_party/lvgl/src/misc/lv_style_gen.c
    third_party/lvgl/src/misc/lv_templ.c
    third_party/lvgl/src/misc/lv_text.c
    third_party/lvgl/src/misc/lv_text_ap.c
    third_party/lvgl/src/misc/lv_timer.c
    third_party/lvgl/src/misc/lv_tree.c
    third_party/lvgl/src/misc/lv_utils.c
)

# Cache components
set(LVGL_CACHE_SOURCES
    third_party/lvgl/src/misc/cache/lv_cache.c
    third_party/lvgl/src/misc/cache/lv_cache_entry.c
    third_party/lvgl/src/misc/cache/lv_cache_lru_rb.c
    third_party/lvgl/src/misc/cache/lv_image_cache.c
    third_party/lvgl/src/misc/cache/lv_image_header_cache.c
)

# OSAL components
set(LVGL_OSAL_SOURCES
    third_party/lvgl/src