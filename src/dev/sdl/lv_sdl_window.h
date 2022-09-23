/**
 * @file lv_sdl_window.h
 *
 */

#ifndef LV_SDL_DISP_H
#define LV_SDL_DISP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "../../hal/lv_hal_disp.h"

#if LV_USE_SDL

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
struct _lv_sdl_disp_priv_t;
typedef struct {
    lv_coord_t hor_res;
    lv_coord_t ver_res;
    uint8_t zoom;
    struct _lv_sdl_disp_priv_t * _priv;
} lv_sdl_window_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_sdl_window_t * lv_sdl_window_create(void);

lv_disp_t * lv_sdl_window_register(lv_sdl_window_t * dev);

lv_disp_t * _lv_sdl_get_disp_from_win_id(uint32_t win_id);

/**********************
 *      MACROS
 **********************/

#endif /* LV_DRV_SDL */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_SDL_DISP_H */
