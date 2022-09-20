/**
 * @file lv_sdl_mousewheel.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_sdl_mousewheel.h"
#if LV_USE_SDL

#include "../../../src/hal/lv_hal_indev.h"
#include "../../../src/core/lv_group.h"

#include LV_PLATFORM_SDL_INCLUDE_PATH
/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void sdl_mousewheel_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

/**********************
 *  STATIC VARIABLES
 **********************/

typedef struct _lv_sdl_mousewheel_priv_t {
    int16_t diff;
    lv_indev_state_t state;
} _lv_sdl_mousewheel_priv_t;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_sdl_mousewheel_init(lv_sdl_mousewheel_t * dev)
{
    lv_memzero(dev, sizeof(lv_sdl_mousewheel_t));
    return;
}

lv_indev_t * lv_sdl_mousewheel_create(lv_sdl_mousewheel_t * dev)
{
    dev->_priv = lv_malloc(sizeof(lv_sdl_mousewheel_t));
    LV_ASSERT_MALLOC(dev->_priv);

    lv_indev_drv_t * indev_drv = lv_malloc(sizeof(lv_indev_drv_t));
    LV_ASSERT_MALLOC(indev_drv);

    if(dev->_priv == NULL || indev_drv == NULL) {
        lv_free(dev->_priv);
        lv_free(indev_drv);
        return NULL;
    }

    lv_memzero(dev->_priv, sizeof(_lv_sdl_mousewheel_priv_t));

    lv_indev_drv_init(indev_drv);
    indev_drv->type = LV_INDEV_TYPE_ENCODER;
    indev_drv->read_cb = sdl_mousewheel_read;
    indev_drv->user_data = dev;
    lv_indev_t * indev = lv_indev_drv_register(indev_drv);

    return indev;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void sdl_mousewheel_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    lv_sdl_mousewheel_t * dev = indev_drv->user_data;

    data->state = dev->_priv->state;
    data->enc_diff = dev->_priv->diff;
    dev->_priv->diff = 0;
}

void _lv_sdl_mousewheel_handler(SDL_Event * event)
{
    uint32_t win_id = UINT32_MAX;
    switch(event->type) {
        case SDL_MOUSEWHEEL:
            win_id = event->wheel.windowID;
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            win_id = event->button.windowID;
            break;
        default:
            return;
    }

    lv_disp_t * disp = lv_drv_sdl_get_disp_from_win_id(win_id);

    /*Find a suitable indev*/
    lv_indev_t * indev = lv_indev_get_next(NULL);
    while(indev) {
        if(indev->driver->disp == disp && lv_indev_get_type(indev) == LV_INDEV_TYPE_ENCODER) {
            break;
        }
        indev = lv_indev_get_next(indev);
    }

    if(indev == NULL) return;
    lv_sdl_mousewheel_t * indev_dev = indev->driver->user_data;

    switch(event->type) {
        case SDL_MOUSEWHEEL:
            // Scroll down (y = -1) means positive encoder turn,
            // so invert it
#ifdef __EMSCRIPTEN__
            /*Escripten scales it wrong*/
            if(event->wheel.y < 0) indev_dev->_priv->diff++;
            if(event->wheel.y > 0) indev_dev->_priv->diff--;
#else
            indev_dev->_priv->diff = -event->wheel.y;
#endif
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event->button.button == SDL_BUTTON_MIDDLE) {
                indev_dev->_priv->state = LV_INDEV_STATE_PRESSED;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(event->button.button == SDL_BUTTON_MIDDLE) {
                indev_dev->_priv->state = LV_INDEV_STATE_RELEASED;
            }
            break;
        default:
            break;
    }
}

#endif /*LV_USE_SDL*/

