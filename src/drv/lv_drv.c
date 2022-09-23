/**
 * @file lv_drv.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_drv.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_drv_init(lv_drv_t * drv)
{
    lv_memzero(drv, sizeof(*drv));
}

lv_res_t lv_drv_reset_dev(lv_drv_t * drv)
{
    if(drv->send_cmd_cb == NULL) return LV_RES_INV;

    return drv->reset_dev_cb(drv);
}

lv_res_t lv_drv_send_cmd(lv_drv_t * drv, uint8_t cmd, const void * params, uint32_t len)
{
    if(drv->send_cmd_cb == NULL) return LV_RES_INV;

    return drv->send_cmd_cb(drv, cmd, params, len);
}

lv_res_t lv_drv_read_data(lv_drv_t * drv, uint8_t cmd, const void * params, uint32_t params_len, void * rxbuf,
                          uint32_t rxbuf_len)
{
    if(drv->read_data_cb == NULL) return LV_RES_INV;

    return drv->read_data_cb(drv, cmd, params, params_len, rxbuf, rxbuf_len);
}

lv_res_t lv_drv_send_image(lv_drv_t * drv, lv_disp_t * disp, const lv_area_t * area, const void * buf)
{
    if(drv->send_image_cb == NULL) return LV_RES_INV;

    return drv->send_image_cb(drv, disp, area, buf);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/
