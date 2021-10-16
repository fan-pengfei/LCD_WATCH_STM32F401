#include "my_lib.h"
#include "lcd_init.h"
#include "lcd.h"
#include "stdio.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_ex_conf.h"
#include "lv_examples.h"
#include "lv_demo_widgets.h"
#include "usbd_cdc_if.h"
static void keypad_init(void);
static bool keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static uint32_t keypad_get_key(void);
lv_indev_t *indev_keypad;
lv_group_t *group;
int8_t my_day = 15, my_month = 10;
uint16_t my_year = 2021;
lv_obj_t *calendar;
void lv_port_indev_init(void)
{
    lv_indev_drv_t indev_drv;
    /*------------------
     * Keypad
     * -----------------*/

    /*Initialize your keypad or keyboard if you have*/
    keypad_init();

    /*Register a keypad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);
    group = lv_group_create();
    lv_indev_set_group(indev_keypad, group);
}
static void keypad_init(void)
{
    /*Your code comes here*/
}
/* Will be called by the library to read the mouse */
static bool keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint32_t last_key = 0;
    /*Get the current x and y coordinates*/
    /*Get whether the a key is pressed and save the pressed key*/
    uint32_t act_key = keypad_get_key();
    if (act_key != 0)
    {
        data->state = LV_INDEV_STATE_PR;
        /*Translate the keys to LVGL control characters according to your key definitions*/
        switch (act_key)
        {
        case 1:
            act_key = LV_KEY_RIGHT;
            break;
        case 2:
            act_key = LV_KEY_LEFT;
            break;
        case 3:
            act_key = 81;
            break;
        case 4:
            act_key = 82;
            break;
        }
        last_key = act_key;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
    data->key = last_key;
    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}

/*Get the currently being pressed key.  0 if no key is pressed*/
static uint32_t keypad_get_key(void)
{
    if (HAL_GPIO_ReadPin(User_Key1_GPIO_Port, User_Key1_Pin) == 0)
    {
        return 1;
    }
    if (HAL_GPIO_ReadPin(User_Key2_GPIO_Port, User_Key2_Pin) == 0)
    {
        return 2;
    }
    if (HAL_GPIO_ReadPin(User_Key3_GPIO_Port, User_Key3_Pin) == 0)
    {
        return 3;
    }
    if (HAL_GPIO_ReadPin(User_Key4_GPIO_Port, User_Key4_Pin) == 0)
    {
        return 4;
    }
    /*Your code comes here*/
    return false;
}
static void event_handler(lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_KEY:
    {
        const uint32_t *key = lv_event_get_data();
        usb_printf("%d", *key);
        if (*key == 81)
        {
        }
        else if (*key == 82)
        {
        }
        else if (*key == LV_KEY_RIGHT)
        {
        }
        else if (*key == LV_KEY_LEFT)
        {
        }
    }
    break;
    default:
        break;
    }
}

void lv_ex_calendar_1(void)
{
    calendar = lv_calendar_create(lv_scr_act(), NULL);
    lv_obj_set_size(calendar, 240, 240);
    lv_obj_align(calendar, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_group_add_obj(group, calendar);
    lv_obj_set_event_cb(calendar, event_handler);

    /*Make the date number smaller to be sure they fit into their area*/
    lv_obj_set_style_local_text_font(calendar, LV_CALENDAR_PART_DATE, LV_STATE_DEFAULT, lv_theme_get_font_small());

    /*Set today's date*/
    lv_calendar_date_t today;
    today.year = 2021;
    today.month = 10;
    today.day = 16;

    lv_calendar_set_today_date(calendar, &today);
    lv_calendar_set_showed_date(calendar, &today);

    /*Highlight a few days*/
    static lv_calendar_date_t highlighted_days[3]; /*Only its pointer will be saved so should be static*/
    highlighted_days[0].year = my_year;
    highlighted_days[0].month = my_month;
    highlighted_days[0].day = my_day;

    highlighted_days[1].year = 2021;
    highlighted_days[1].month = 10;
    highlighted_days[1].day = 11;

    highlighted_days[2].year = 2021;
    highlighted_days[2].month = 10;
    highlighted_days[2].day = 22;

    lv_calendar_set_highlighted_dates(calendar, highlighted_days, 1);
}
