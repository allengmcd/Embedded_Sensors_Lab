# LVGL v9.3 component selection for TM4C129XL project
# This file contains all LVGL-specific component selections to keep the main CMakeLists.txt clean

# Path to LVGL repository - can be overridden from main CMakeLists
if(NOT DEFINED LVGL_DIR)
    set(LVGL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lvgl)
endif()

# ======== Core Components (essential for any LVGL application) ========
set(LVGL_CORE_COMPONENTS
    # Core functionality
    ${LVGL_DIR}/src/core/lv_obj.c
    ${LVGL_DIR}/src/core/lv_obj_class.c
    ${LVGL_DIR}/src/core/lv_obj_draw.c
    ${LVGL_DIR}/src/core/lv_obj_pos.c
    ${LVGL_DIR}/src/core/lv_obj_scroll.c
    ${LVGL_DIR}/src/core/lv_obj_style.c
    ${LVGL_DIR}/src/core/lv_obj_tree.c
    ${LVGL_DIR}/src/core/lv_obj_event.c
    ${LVGL_DIR}/src/core/lv_obj_property.c
    ${LVGL_DIR}/src/core/lv_refr.c
    ${LVGL_DIR}/src/display/lv_display.c
    ${LVGL_DIR}/src/core/lv_group.c
    ${LVGL_DIR}/src/indev/lv_indev.c
    ${LVGL_DIR}/src/indev/lv_indev_scroll.c
    ${LVGL_DIR}/src/themes/lv_theme.c
)

# ======== HAL (Hardware Abstraction Layer) ========
set(LVGL_HAL_COMPONENTS
    ${LVGL_DIR}/src/hal/lv_hal_tick.c
    ${LVGL_DIR}/src/hal/lv_hal_display.c
    ${LVGL_DIR}/src/hal/lv_hal_indev.c
)

# ======== Essential miscellaneous components ========
set(LVGL_MISC_COMPONENTS
    ${LVGL_DIR}/src/misc/lv_area.c
    ${LVGL_DIR}/src/misc/lv_color.c
    ${LVGL_DIR}/src/misc/lv_palette.c
    ${LVGL_DIR}/src/misc/lv_timer.c
    ${LVGL_DIR}/src/misc/lv_event.c
    ${LVGL_DIR}/src/misc/lv_math.c
    ${LVGL_DIR}/src/stdlib/lv_mem.c
    ${LVGL_DIR}/src/misc/lv_text.c
    ${LVGL_DIR}/src/misc/lv_bidi.c
    ${LVGL_DIR}/src/misc/lv_style.c
    ${LVGL_DIR}/src/misc/lv_anim.c
    ${LVGL_DIR}/src/misc/lv_async.c
    ${LVGL_DIR}/src/misc/lv_fs.c
    ${LVGL_DIR}/src/misc/lv_array.c
    ${LVGL_DIR}/src/misc/lv_ll.c
    # ${LVGL_DIR}/src/stdlib/lv_string.c
    # ${LVGL_DIR}/src/stdlib/lv_printf.c
)

# ======== Drawing functionality ========
option(LVGL_USE_DRAW_SW "Include software rendering" ON)
option(LVGL_USE_DRAW_HW "Include hardware acceleration" OFF)

set(LVGL_DRAW_CORE
    ${LVGL_DIR}/src/draw/lv_draw.c
    ${LVGL_DIR}/src/draw/lv_draw_layer.c
    ${LVGL_DIR}/src/draw/lv_img_cache.c
    ${LVGL_DIR}/src/draw/lv_img_decoder.c
    ${LVGL_DIR}/src/draw/lv_draw_buf.c
)

# Software rendering
if(LVGL_USE_DRAW_SW)
    set(LVGL_DRAW_SW
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_arc.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_bg.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_border.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_box_shadow.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_fill.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_gradient.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_img.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_layer.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_letter.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_line.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_mask.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_mask_rect.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_transform.c
        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_triangle.c
    )
endif()

# Hardware rendering (if needed)
if(LVGL_USE_DRAW_HW)
    set(LVGL_DRAW_HW
        # Add hardware-specific rendering files if needed
        # Example: ${LVGL_DIR}/src/draw/arm2d/lv_gpu_arm2d.c
    )
endif()

set(LVGL_DRAW_COMPONENTS
    ${LVGL_DRAW_CORE}
    ${LVGL_DRAW_SW}
    ${LVGL_DRAW_HW}
)

# ======== Font handling ========
set(LVGL_FONT_CORE
    ${LVGL_DIR}/src/font/lv_font.c
    ${LVGL_DIR}/src/font/lv_font_fmt_txt.c
    ${LVGL_DIR}/src/font/lv_font_loader.c
)

# Font options
option(LVGL_USE_FONT_MONTSERRAT_10 "Include Montserrat 10pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_12 "Include Montserrat 12pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_14 "Include Montserrat 14pt font" ON)
option(LVGL_USE_FONT_MONTSERRAT_16 "Include Montserrat 16pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_18 "Include Montserrat 18pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_20 "Include Montserrat 20pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_22 "Include Montserrat 22pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_24 "Include Montserrat 24pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_26 "Include Montserrat 26pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_28 "Include Montserrat 28pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_30 "Include Montserrat 30pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_32 "Include Montserrat 32pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_34 "Include Montserrat 34pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_36 "Include Montserrat 36pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_38 "Include Montserrat 38pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_40 "Include Montserrat 40pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_42 "Include Montserrat 42pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_44 "Include Montserrat 44pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_46 "Include Montserrat 46pt font" OFF)
option(LVGL_USE_FONT_MONTSERRAT_48 "Include Montserrat 48pt font" OFF)
option(LVGL_USE_FONT_UNSCII_8 "Include UNSCII 8pt font (smaller)" OFF)
option(LVGL_USE_FONT_UNSCII_16 "Include UNSCII 16pt font" OFF)

# Font selection
set(LVGL_FONT_SELECTION "")

if(LVGL_USE_FONT_MONTSERRAT_10)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_10.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_12)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_12.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_14)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_14.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_16)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_16.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_18)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_18.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_20)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_20.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_22)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_22.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_24)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_24.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_26)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_26.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_28)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_28.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_30) 
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_30.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_32)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_32.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_34)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_34.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_36)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_36.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_38)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_38.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_40)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_40.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_42)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_42.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_44)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_44.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_46)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_46.c)
endif()

if(LVGL_USE_FONT_MONTSERRAT_48)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_montserrat_48.c)
endif()

if(LVGL_USE_FONT_UNSCII_8)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_unscii_8.c)
endif()

if(LVGL_USE_FONT_UNSCII_16)
    list(APPEND LVGL_FONT_SELECTION ${LVGL_DIR}/src/font/lv_font_unscii_16.c)
endif()

set(LVGL_FONT_COMPONENTS
    ${LVGL_FONT_CORE}
    ${LVGL_FONT_SELECTION}
)

# ======== Widget selection ========
# Basic widgets (always needed)
set(LVGL_BASE_WIDGETS
    ${LVGL_DIR}/src/widgets/lv_label.c
    ${LVGL_DIR}/src/widgets/lv_btn.c
    ${LVGL_DIR}/src/widgets/lv_img.c
)

# Widget groups
option(LVGL_USE_CONTAINER_WIDGETS "Include container widgets" OFF)
option(LVGL_USE_INPUT_WIDGETS "Include input widgets" OFF)
option(LVGL_USE_VISUAL_WIDGETS "Include visual widgets" OFF)
option(LVGL_USE_EXTRA_WIDGETS "Include extra/advanced widgets" OFF)

# Container widgets
if(LVGL_USE_CONTAINER_WIDGETS)
    set(LVGL_CONTAINER_WIDGETS
        ${LVGL_DIR}/src/widgets/lv_line.c
        ${LVGL_DIR}/src/widgets/lv_table.c
        ${LVGL_DIR}/src/widgets/lv_list.c
        ${LVGL_DIR}/src/widgets/lv_menu.c
        ${LVGL_DIR}/src/widgets/lv_msgbox.c
        ${LVGL_DIR}/src/widgets/lv_tabview.c
        ${LVGL_DIR}/src/widgets/lv_tileview.c
        ${LVGL_DIR}/src/widgets/lv_win.c
    )
endif()

# Input widgets
if(LVGL_USE_INPUT_WIDGETS)
    set(LVGL_INPUT_WIDGETS
        ${LVGL_DIR}/src/widgets/lv_btnmatrix.c
        ${LVGL_DIR}/src/widgets/lv_checkbox.c
        ${LVGL_DIR}/src/widgets/lv_dropdown.c
        ${LVGL_DIR}/src/widgets/lv_roller.c
        ${LVGL_DIR}/src/widgets/lv_slider.c
        ${LVGL_DIR}/src/widgets/lv_switch.c
        ${LVGL_DIR}/src/widgets/lv_textarea.c
    )
endif()

# Visual widgets
if(LVGL_USE_VISUAL_WIDGETS)
    set(LVGL_VISUAL_WIDGETS
        ${LVGL_DIR}/src/widgets/lv_arc.c
        ${LVGL_DIR}/src/widgets/lv_bar.c
        ${LVGL_DIR}/src/widgets/lv_canvas.c
        ${LVGL_DIR}/src/widgets/lv_chart.c
        ${LVGL_DIR}/src/widgets/lv_colorwheel.c
        ${LVGL_DIR}/src/widgets/lv_led.c
        ${LVGL_DIR}/src/widgets/lv_meter.c
        ${LVGL_DIR}/src/widgets/lv_span.c
        ${LVGL_DIR}/src/widgets/lv_spinbox.c
    )
endif()

# Extra widgets
if(LVGL_USE_EXTRA_WIDGETS)
    set(LVGL_EXTRA_WIDGETS
        ${LVGL_DIR}/src/widgets/lv_calendar.c
        ${LVGL_DIR}/src/widgets/lv_calendar_header_arrow.c
        ${LVGL_DIR}/src/widgets/lv_calendar_header_dropdown.c
        ${LVGL_DIR}/src/widgets/lv_animimg.c
        ${LVGL_DIR}/src/widgets/lv_imgbtn.c
        ${LVGL_DIR}/src/widgets/lv_keyboard.c
    )
endif()

# Combine all widget sets
set(LVGL_WIDGETS_COMPONENTS
    ${LVGL_BASE_WIDGETS}
    ${LVGL_CONTAINER_WIDGETS}
    ${LVGL_INPUT_WIDGETS}
    ${LVGL_VISUAL_WIDGETS}
    ${LVGL_EXTRA_WIDGETS}
)

# ======== Layouts ========
option(LVGL_USE_LAYOUTS "Include layout engines" ON)

if(LVGL_USE_LAYOUTS)
    set(LVGL_LAYOUTS
        ${LVGL_DIR}/src/layouts/flex/lv_flex.c
        ${LVGL_DIR}/src/layouts/grid/lv_grid.c
    )
endif()

# ======== Themes ========
option(LVGL_USE_THEME_DEFAULT "Include default theme" ON)
option(LVGL_USE_THEME_MONO "Include monochrome theme" OFF)

set(LVGL_THEMES "")

if(LVGL_USE_THEME_DEFAULT)
    list(APPEND LVGL_THEMES ${LVGL_DIR}/src/themes/default/lv_theme_default.c)
endif()

if(LVGL_USE_THEME_MONO)
    list(APPEND LVGL_THEMES ${LVGL_DIR}/src/themes/mono/lv_theme_mono.c)
endif()

# ======== Combine all LVGL sources ========
set(LVGL_SOURCES
    ${LVGL_CORE_COMPONENTS}
    # ${LVGL_HAL_COMPONENTS}
    ${LVGL_MISC_COMPONENTS}
    ${LVGL_DRAW_COMPONENTS}
    ${LVGL_WIDGETS_COMPONENTS}
    ${LVGL_LAYOUTS}
    ${LVGL_THEMES}
    ${LVGL_FONT_COMPONENTS}
)

# ======== Add LVGL include directories for any target that will use this file ========
function(target_setup_lvgl TARGET_NAME)
    target_include_directories(${TARGET_NAME} PRIVATE
        ${LVGL_DIR}
    )
    
    target_compile_definitions(${TARGET_NAME} PRIVATE LV_CONF_INCLUDE_SIMPLE=1)
endfunction()