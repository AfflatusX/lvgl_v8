#include <emscripten.h>
#include <unistd.h>
#include <SDL.h>
#include "lvgl.h"
#include "../demos/music/lv_demo_music.h"

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    volatile bool sdl_refr_qry;
} monitor_t;

static void hal_init(void);
static void monitor_sdl_clean_up(void);
static void monitor_sdl_init(void);
static void monitor_sdl_refr(lv_timer_t *t);
static void window_create(monitor_t *m);

static volatile bool sdl_inited = false;
static volatile bool sdl_quit_qry = false;

#define MONITOR_HOR_RES (480)
#define MONITOR_VER_RES (480)
#define MONITOR_ZOOM (1)

monitor_t monitor;

void lv_loop()
{
    lv_tick_inc(16);
    lv_timer_handler();
}

int main(int argc, char *argv[])
{
    lv_init();
    hal_init();

    lv_demo_music();

    emscripten_set_main_loop(lv_loop, 60, 1);

    return 0;
}

/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixel to copy to the `area` part of the screen
 */
void monitor_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    lv_coord_t hres = disp_drv->hor_res;
    lv_coord_t vres = disp_drv->ver_res;

    /*Return if the area is out the screen*/
    if (area->x2 < 0 || area->y2 < 0 || area->x1 > hres - 1 || area->y1 > vres - 1)
    {
        lv_disp_flush_ready(disp_drv);
        return;
    }

    int32_t y;
    uint32_t w = lv_area_get_width(area);

    monitor.sdl_refr_qry = true;

    /* TYPICALLY YOU DO NOT NEED THIS
     * If it was the last part to refresh update the texture of the window.*/
    if (lv_disp_flush_is_last(disp_drv))
    {
        monitor_sdl_refr(NULL);
    }

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);
}

static void hal_init(void)
{
    /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    monitor_sdl_init();

    /*Create a display buffer*/
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t buf1_1[MONITOR_HOR_RES * MONITOR_VER_RES];
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, NULL, MONITOR_HOR_RES * MONITOR_VER_RES);

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = monitor_flush;
    disp_drv.hor_res = MONITOR_HOR_RES;
    disp_drv.ver_res = MONITOR_VER_RES;
    disp_drv.antialiasing = 1;
    disp_drv.user_data = (void *)(monitor.renderer);

    printf("rederer address on init: %p\n", (void *)monitor.renderer);

    lv_disp_drv_register(&disp_drv);
}

static void window_create(monitor_t *m)
{
    // SDL_CreateWindowAndRenderer(LV_HOR_RES, LV_VER_RES, 0, &m->window, &m->renderer);
    m->window = SDL_CreateWindow("LVGL Simulator",
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 MONITOR_HOR_RES * MONITOR_ZOOM, MONITOR_VER_RES * MONITOR_ZOOM, 0); /*last param. SDL_WINDOW_BORDERLESS to hide borders*/

    m->renderer = SDL_CreateRenderer(m->window, -1, SDL_RENDERER_SOFTWARE);
}

static void monitor_sdl_init(void)
{
    /*Initialize the SDL*/
    SDL_Init(SDL_INIT_VIDEO);
    window_create(&monitor);
    sdl_inited = true;
}

static void monitor_sdl_refr(lv_timer_t *t)
{
    /*Refresh handling*/
    if (monitor.sdl_refr_qry != false)
    {
        monitor.sdl_refr_qry = false;
        SDL_RenderPresent(monitor.renderer);
    }
}

static void monitor_sdl_clean_up(void)
{
    SDL_DestroyRenderer(monitor.renderer);
    SDL_DestroyWindow(monitor.window);

    SDL_Quit();
}