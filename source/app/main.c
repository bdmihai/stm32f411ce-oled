/*_____________________________________________________________________________
 │                                                                            |
 │ COPYRIGHT (C) 2021 Mihai Baneu                                             |
 │                                                                            |
 | Permission is hereby  granted,  free of charge,  to any person obtaining a |
 | copy of this software and associated documentation files (the "Software"), |
 | to deal in the Software without restriction,  including without limitation |
 | the rights to  use, copy, modify, merge, publish, distribute,  sublicense, |
 | and/or sell copies  of  the Software, and to permit  persons to  whom  the |
 | Software is furnished to do so, subject to the following conditions:       |
 |                                                                            |
 | The above  copyright notice  and this permission notice  shall be included |
 | in all copies or substantial portions of the Software.                     |
 |                                                                            |
 | THE SOFTWARE IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS |
 | OR   IMPLIED,   INCLUDING   BUT   NOT   LIMITED   TO   THE  WARRANTIES  OF |
 | MERCHANTABILITY,  FITNESS FOR  A  PARTICULAR  PURPOSE AND NONINFRINGEMENT. |
 | IN NO  EVENT SHALL  THE AUTHORS  OR  COPYRIGHT  HOLDERS  BE LIABLE FOR ANY |
 | CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT, TORT |
 | OR OTHERWISE, ARISING FROM,  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  |
 | THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                 |
 |____________________________________________________________________________|
 |                                                                            |
 |  Author: Mihai Baneu                           Last modified: 02.Jan.2021  |
 |                                                                            |
 |___________________________________________________________________________*/

#include "stm32f4xx.h"
#include "stm32rtos.h"
#include "task.h"
#include "system.h"
#include "gpio.h"
#include "ssd1339.h"

extern const uint16_t image[];
extern const uint8_t u8x8_font_8x13B_1x2_f[];

static void vTaskLED(void *pvParameters)
{
    (void)pvParameters;

    /* led OFF */
    gpio_set_blue_led();

    for (;;) {
        gpio_reset_blue_led();
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        gpio_set_blue_led();
        vTaskDelay(100 / portTICK_PERIOD_MS);

        gpio_reset_blue_led();
        vTaskDelay(100 / portTICK_PERIOD_MS);

        gpio_set_blue_led();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

static void vTaskDisplay(void *pvParameters)
{
    (void)pvParameters;
    const uint16_t cDisplayColors[] = { 
        SSD1339_BLACK  ,SSD1339_WHITE  ,SSD1339_RED	   ,SSD1339_LIME   ,SSD1339_BLUE   ,SSD1339_YELLOW ,SSD1339_CYAN   ,SSD1339_MAGENTA,
        SSD1339_SILVER ,SSD1339_GRAY   ,SSD1339_MAROON ,SSD1339_OLIVE  ,SSD1339_GREEN  ,SSD1339_PURPLE ,SSD1339_TEAL   ,SSD1339_NAVY 
    };

    ssd1339_hw_control_t hw = {
        .config_control_out  = gpio_config_control_out,
        .config_data_out     = gpio_config_data_out,
        .config_data_in      = gpio_config_data_in,
        .cs_high             = gpio_cs_high,
        .cs_low              = gpio_cs_low,
        .res_high            = gpio_res_high,
        .res_low             = gpio_res_low,
        .dc_high             = gpio_dc_high,
        .dc_low              = gpio_dc_low,
        .wr_high             = gpio_wr_high,
        .wr_low              = gpio_wr_low,
        .rd_high             = gpio_rd_high,
        .rd_low              = gpio_rd_low,
        .cs_wr_high          = gpio_cs_wr_high,
        .cs_wr_low           = gpio_cs_wr_low,
        .cs_rd_high          = gpio_cs_rd_high,
        .cs_rd_low           = gpio_cs_rd_low,
        .dc_cs_wr_high       = gpio_dc_cs_wr_high,
        .dc_cs_wr_low        = gpio_dc_cs_wr_low,
        .data_wr             = gpio_data_wr,
        .data_rd             = gpio_data_rd
    };

    ssd1339_init(hw);
    ssd1339_set_column_address(0, SSD1339_128_COLS-1);
    ssd1339_set_row_address(0, SSD1339_128_ROWS-1);
    ssd1339_set_map_and_color_depth(SSD1339_REMAP_MODE_ODD_EVEN | SSD1339_COLOR_MODE_65K);
    ssd1339_set_use_buildin_lut();
    ssd1339_set_sleep_mode(SSD1339_SLEEP_OFF);
    ssd1339_set_display_mode(SSD1339_MODE_ALL_OFF);
    ssd1339_set_contrast_curent(1,200,200);
    ssd1339_set_precharge_voltage(2, 255, 255);
    ssd1339_set_display_mode(SSD1339_MODE_RESET_TO_NORMAL_DISPLAY);
 
    uint8_t all = 0;
    for (;;) {
        //goto draw_text;
        all = 1;

fill_screen:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLUE);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_GREEN);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_RED);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_WHITE);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        if (!all) continue;

direct_ram_access:
        ssd1339_draw_gradient_hfill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLUE);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_gradient_hfill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_LIME);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        
        ssd1339_draw_gradient_hfill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_RED);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_gradient_hfill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_WHITE);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_gradient_vfill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLUE);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_gradient_vfill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_LIME);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        
        ssd1339_draw_gradient_vfill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_RED);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_gradient_vfill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_WHITE);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        if (!all) continue;

region_fill:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_WHITE);
        vTaskDelay(100 / portTICK_PERIOD_MS);

        ssd1339_draw_fill(10, 20, 30, 100, SSD1339_BLUE);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_fill(50, 40, 70, 100, SSD1339_GREEN);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_fill(90, 60, 110, 100, SSD1339_RED);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        if (!all) continue;

check_colors:
        ssd1339_draw_fill(   0,   0,  16,   64, SSD1339_WHITE);
        ssd1339_draw_fill(  16,   0,  32,   64, SSD1339_MAROON);
        ssd1339_draw_fill(  32,   0,  48,   64, SSD1339_GREEN);
        ssd1339_draw_fill(  48,   0,  64,   64, SSD1339_OLIVE);
        ssd1339_draw_fill(  64,   0,  80,   64, SSD1339_NAVY);
        ssd1339_draw_fill(  80,   0,  96,   64, SSD1339_PURPLE);
        ssd1339_draw_fill(  96,   0, 112,   64, SSD1339_TEAL);
        ssd1339_draw_fill( 112,   0, 128,   64, SSD1339_SILVER);
        ssd1339_draw_fill(   0,  64,  16,  128, SSD1339_GRAY);
        ssd1339_draw_fill(  16,  64,  32,  128, SSD1339_RED);
        ssd1339_draw_fill(  32,  64,  48,  128, SSD1339_LIME);
        ssd1339_draw_fill(  48,  64,  64,  128, SSD1339_YELLOW);
        ssd1339_draw_fill(  64,  64,  80,  128, SSD1339_BLUE);
        ssd1339_draw_fill(  80,  64,  96,  128, SSD1339_MAGENTA);
        ssd1339_draw_fill(  96,  64, 112,  128, SSD1339_CYAN);
        ssd1339_draw_fill( 112,  64, 128,  128, SSD1339_BLACK);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (!all) continue;

check_white:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_WHITE);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (!all) continue;

draw_line:
        // 45 deg lines examples
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLACK);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        for (int i = 0; i < 128; i += 16) {
            ssd1339_draw_line(0, 0, i, 127, SSD1339_WHITE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        for (int i = 0; i < 128; i += 16) {
            ssd1339_draw_line(0, 0, 127, i, SSD1339_WHITE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        for (int i = 0; i < 128; i += 16) {
            ssd1339_draw_line(0, i, 127, 127, SSD1339_WHITE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        for (int i = 0; i < 128; i += 16) {
            ssd1339_draw_line(i, 0, 127, 127, SSD1339_WHITE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        ssd1339_draw_line(0, 127, 127, 127, SSD1339_WHITE);
        vTaskDelay(100 / portTICK_PERIOD_MS);

        // 135 deg lines examples
        ssd1339_set_map_and_color_depth(SSD1339_REMAP_MODE_131_SEG0 | SSD1339_REMAP_MODE_ODD_EVEN | SSD1339_COLOR_MODE_65K);
        for (int i = 0; i < 128; i += 16) {
            ssd1339_draw_line(SSD1339_MAX_COLS-SSD1339_128_COLS, 0, i+(SSD1339_MAX_COLS-SSD1339_128_COLS), 127, SSD1339_WHITE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        for (int i = 0; i < 128; i += 16) {
            ssd1339_draw_line(SSD1339_MAX_COLS-SSD1339_128_COLS, 0, SSD1339_MAX_COLS-1, i, SSD1339_WHITE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        for (int i = 0; i < 128; i += 16) {
            ssd1339_draw_line(SSD1339_MAX_COLS-SSD1339_128_COLS, i, SSD1339_MAX_COLS-1, 127, SSD1339_WHITE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        for (int i = 0; i < 128; i += 16) {
            ssd1339_draw_line(i+(SSD1339_MAX_COLS-SSD1339_128_COLS), 0, SSD1339_MAX_COLS-1, 127, SSD1339_WHITE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        ssd1339_set_map_and_color_depth(SSD1339_REMAP_MODE_0_SEG0 | SSD1339_REMAP_MODE_ODD_EVEN | SSD1339_COLOR_MODE_65K);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (!all) continue;

draw_rectangle:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLACK);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_set_fill_copy_mode(SSD1339_FILLMODE_DISABLE);
        for (int i = 0; i < 64; i += 16) {
            ssd1339_draw_rectangle(i, i, 127 - i, 127 - i, SSD1339_WHITE, SSD1339_BLACK);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (!all) continue;

draw_filled_rectangle:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLACK);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_set_fill_copy_mode(SSD1339_FILLMODE_ENABLE);
        for (int i = 0; i < 64; i += 8) {
            ssd1339_draw_rectangle(i, i, 127 - i, 127 - i, SSD1339_WHITE, cDisplayColors[i/8]);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (!all) continue;

draw_circle:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLACK);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_set_fill_copy_mode(SSD1339_FILLMODE_DISABLE);
        for (int i = 0; i < 64; i += 16) {
            ssd1339_draw_circle(64, 64, i, SSD1339_WHITE, SSD1339_BLACK);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (!all) continue;

draw_filled_circle:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLACK);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_set_fill_copy_mode(SSD1339_FILLMODE_ENABLE);
        for (int i = 63; i > 0; i -= 8) {
            ssd1339_draw_circle(64, 64, i, SSD1339_WHITE, cDisplayColors[i/8]);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (!all) continue;

draw_image:
        ssd1339_draw_image(0, 0, 128, 128, image);
        vTaskDelay(2500 / portTICK_PERIOD_MS);

        if (!all) continue;

draw_text:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLACK);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_draw_string(u8x8_font_8x13B_1x2_f, 2*8, 7*8, SSD1339_WHITE, SSD1339_BLACK, "Hello: ");
        for (int i = 0; i < 2500; i++) {
            char txt[5] = { 0, 0, 0, 0, 0};
            itoa(i, txt, 10);
            ssd1339_draw_string(u8x8_font_8x13B_1x2_f, 9*8, 7*8, SSD1339_WHITE, SSD1339_BLACK, txt);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (!all) continue;

draw_copy:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLACK);
        ssd1339_set_fill_copy_mode(SSD1339_FILLMODE_ENABLE);
        ssd1339_draw_rectangle(25, 25, 45, 45, SSD1339_WHITE, SSD1339_BLUE);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        ssd1339_draw_line(25, 25, 45, 45, SSD1339_WHITE);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        ssd1339_set_map_and_color_depth(SSD1339_REMAP_MODE_131_SEG0 | SSD1339_REMAP_MODE_ODD_EVEN | SSD1339_COLOR_MODE_65K);
        ssd1339_draw_line(SSD1339_MAX_COLS-1-45, 25, SSD1339_MAX_COLS-1-25, 45, SSD1339_WHITE);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        ssd1339_set_map_and_color_depth(SSD1339_REMAP_MODE_0_SEG0 | SSD1339_REMAP_MODE_ODD_EVEN | SSD1339_COLOR_MODE_65K);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        ssd1339_draw_copy(25, 25, 45, 45, 50, 25);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        ssd1339_draw_copy(25, 25, 45, 45, 75, 25);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        ssd1339_draw_copy(25, 25, 45, 45, 50, 50);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        ssd1339_draw_copy(25, 25, 45, 45, 75, 50);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        ssd1339_draw_copy(25, 25, 45, 45, 75, 75);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        if (!all) continue;

draw_dim:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLACK);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_set_fill_copy_mode(SSD1339_FILLMODE_ENABLE);
        ssd1339_draw_circle(64, 64, 55, SSD1339_WHITE, SSD1339_RED);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        ssd1339_draw_circle(64, 64, 45, SSD1339_RED, SSD1339_BLUE);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        ssd1339_draw_circle(64, 64, 30, SSD1339_RED, SSD1339_YELLOW);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        ssd1339_draw_circle(64, 64, 25, SSD1339_BLUE, SSD1339_OLIVE);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        for (int i = 0; i < 128; i++) {
            ssd1339_draw_dim(0, 0, i, 127);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
        
        if (!all) continue;

draw_clear:
        ssd1339_draw_fill(0, 0, SSD1339_128_ROWS, SSD1339_128_COLS, SSD1339_BLACK);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        ssd1339_set_fill_copy_mode(SSD1339_FILLMODE_ENABLE);
        ssd1339_draw_circle(64, 64, 55, SSD1339_WHITE, SSD1339_RED);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        ssd1339_draw_circle(64, 64, 45, SSD1339_RED, SSD1339_BLUE);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        ssd1339_draw_circle(64, 64, 30, SSD1339_RED, SSD1339_YELLOW);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        ssd1339_draw_circle(64, 64, 25, SSD1339_BLUE, SSD1339_OLIVE);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        for (int i = 0; i < 128; i++) {
            ssd1339_draw_clear(0, 0, i, 127);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
        

        if (!all) continue;

    }
}

int main(void)
{
    /* initialize the system */
    system_init();

    /* initialize the gpio */
    gpio_init();

    /* create the tasks specific to this application. */
    xTaskCreate(vTaskLED, "vTaskLED", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
    xTaskCreate(vTaskDisplay, "vTaskDisplay", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    /* start the scheduler. */
    vTaskStartScheduler();

    /* should never get here ... */
    blink(10);
    return 0;
}
