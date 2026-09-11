#include <zephyr/kernel.h>
#include <lvgl.h>
#include "brightness_status.h"

#define BRIGHTNESS_STATUS_HIDE_DELAY_MS 500

static struct k_timer brightness_status_timer;

static void brightness_status_timer_cb(struct k_timer *timer)
{
    struct zmk_widget_brightness_status *widget = k_timer_user_data_get(timer);
    if (widget && widget->obj) {
        lv_obj_add_flag(widget->obj, LV_OBJ_FLAG_HIDDEN);
    }
}

int zmk_widget_update_brightness_status(struct zmk_widget_brightness_status *widget, uint8_t brightness)
{
    char brightness_text[8] = {};
    snprintf(brightness_text, sizeof(brightness_text), "%i%%", brightness);
    lv_label_set_text(widget->label, brightness_text);

    // Unhide the widget
    lv_obj_clear_flag(widget->obj, LV_OBJ_FLAG_HIDDEN);

    // One-shot style: restart the timer to hide the widget after the delay
    k_timer_user_data_set(&brightness_status_timer, widget);
    k_timer_start(&brightness_status_timer, K_MSEC(BRIGHTNESS_STATUS_HIDE_DELAY_MS), K_FOREVER);

    return 0;
}

int zmk_widget_brightness_status_init(struct zmk_widget_brightness_status *widget, lv_obj_t *parent)
{
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 240, 280);
    lv_obj_set_style_bg_color(widget->obj, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(widget->obj, LV_OPA_60, 0);

    widget->label = lv_label_create(widget->obj);
    lv_obj_align(widget->label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(widget->label, "");
    lv_obj_set_style_text_font(widget->label, &lv_font_montserrat_40, 0);

    k_timer_init(&brightness_status_timer, brightness_status_timer_cb, NULL);

    lv_obj_add_flag(widget->obj, LV_OBJ_FLAG_HIDDEN);
    return 0;
}

lv_obj_t *zmk_widget_brightness_status_obj(struct zmk_widget_brightness_status *widget)
{
    return widget->obj;
}
