// Copyright 2020-2021 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include <esp_console.h>
#include <esp_heap_caps.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "esp_main.h"
#include "esp_cli.h"
#include "esp_timer.h"
#include "esp_task_wdt.h"

#if CLI_ONLY_INFERENCE
#define IMAGE_COUNT 51
static uint8_t *image_database[IMAGE_COUNT];


extern const uint8_t image0_start[]   asm("_binary_image0_start");
extern const uint8_t image1_start[]   asm("_binary_image1_start");
extern const uint8_t image2_start[]   asm("_binary_image2_start");
extern const uint8_t image3_start[]   asm("_binary_image3_start");
extern const uint8_t image4_start[]   asm("_binary_image4_start");
extern const uint8_t image5_start[]   asm("_binary_image5_start");
extern const uint8_t image6_start[]   asm("_binary_image6_start");
extern const uint8_t image7_start[]   asm("_binary_image7_start");
extern const uint8_t image8_start[]   asm("_binary_image8_start");
extern const uint8_t image9_start[]   asm("_binary_image9_start");
extern const uint8_t image10_start[]  asm("_binary_image10_start");
extern const uint8_t image11_start[]  asm("_binary_image11_start");
extern const uint8_t image12_start[]  asm("_binary_image12_start");
extern const uint8_t image13_start[]  asm("_binary_image13_start");
extern const uint8_t image14_start[]  asm("_binary_image14_start");
extern const uint8_t image15_start[]  asm("_binary_image15_start");
extern const uint8_t image16_start[]  asm("_binary_image16_start");
extern const uint8_t image17_start[]  asm("_binary_image17_start");
extern const uint8_t image18_start[]  asm("_binary_image18_start");
extern const uint8_t image19_start[]  asm("_binary_image19_start");
extern const uint8_t image20_start[]  asm("_binary_image20_start");
extern const uint8_t image21_start[]  asm("_binary_image21_start");
extern const uint8_t image22_start[]  asm("_binary_image22_start");
extern const uint8_t image23_start[]  asm("_binary_image23_start");
extern const uint8_t image24_start[]  asm("_binary_image24_start");
extern const uint8_t image25_start[]  asm("_binary_image25_start");
extern const uint8_t image26_start[]  asm("_binary_image26_start");
extern const uint8_t image27_start[]  asm("_binary_image27_start");
extern const uint8_t image28_start[]  asm("_binary_image28_start");
extern const uint8_t image29_start[]  asm("_binary_image29_start");
extern const uint8_t image30_start[]  asm("_binary_image30_start");
extern const uint8_t image31_start[]  asm("_binary_image31_start");
extern const uint8_t image32_start[]  asm("_binary_image32_start");
extern const uint8_t image33_start[]  asm("_binary_image33_start");
extern const uint8_t image34_start[]  asm("_binary_image34_start");
extern const uint8_t image35_start[]  asm("_binary_image35_start");
extern const uint8_t image36_start[]  asm("_binary_image36_start");
extern const uint8_t image37_start[]  asm("_binary_image37_start");
extern const uint8_t image38_start[]  asm("_binary_image38_start");
extern const uint8_t image39_start[]  asm("_binary_image39_start");
extern const uint8_t image40_start[]  asm("_binary_image40_start");
extern const uint8_t image41_start[]  asm("_binary_image41_start");
extern const uint8_t image42_start[]  asm("_binary_image42_start");
extern const uint8_t image43_start[]  asm("_binary_image43_start");
extern const uint8_t image44_start[]  asm("_binary_image44_start");
extern const uint8_t image45_start[]  asm("_binary_image45_start");
extern const uint8_t image46_start[]  asm("_binary_image46_start");
extern const uint8_t image47_start[]  asm("_binary_image47_start");
extern const uint8_t image48_start[]  asm("_binary_image48_start");
extern const uint8_t image49_start[]  asm("_binary_image49_start");
extern const uint8_t image50_start[]  asm("_binary_image50_start");

#endif

static const char *TAG = "[esp_cli]";

static int task_dump_cli_handler(int argc, char *argv[])
{
    int num_of_tasks = uxTaskGetNumberOfTasks();
    TaskStatus_t *task_array = calloc(1, num_of_tasks * sizeof(TaskStatus_t));
    /* Just to go to the next line */
    printf("\n");
    if (!task_array) {
        ESP_LOGE(TAG, "Memory not allocated for task list.");
        return 0;
    }
    num_of_tasks = uxTaskGetSystemState(task_array, num_of_tasks, NULL);
    printf("\tName\tNumber\tPriority\tStackWaterMark\n");
    for (int i = 0; i < num_of_tasks; i++) {
        printf("%16s\t%u\t%u\t%u\n",
               task_array[i].pcTaskName,
               (unsigned) task_array[i].xTaskNumber,
               (unsigned) task_array[i].uxCurrentPriority,
               (unsigned) task_array[i].usStackHighWaterMark);
    }
    free(task_array);
    return 0;
}

static int cpu_dump_cli_handler(int argc, char *argv[])
{
    /* Just to go to the next line */
    printf("\n");
#ifndef CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
    printf("%s: To use this utility enable: Component config --> FreeRTOS --> Enable FreeRTOS to collect run time stats\n", TAG);
#else
    char *buf = calloc(1, 2 * 1024);
    vTaskGetRunTimeStats(buf);
    printf("%s: Run Time Stats:\n%s\n", TAG, buf);
    free(buf);
#endif
    return 0;
}

static int mem_dump_cli_handler(int argc, char *argv[])
{
    /* Just to go to the next line */
    printf("\n");
    printf("\tDescription\tInternal\tSPIRAM\n");
    printf("Current Free Memory\t%d\t\t%d\n",
           heap_caps_get_free_size(MALLOC_CAP_8BIT) - heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
           heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    printf("Largest Free Block\t%d\t\t%d\n",
           heap_caps_get_largest_free_block(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL),
           heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM));
    printf("Min. Ever Free Size\t%d\t\t%d\n",
           heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL),
           heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM));
    return 0;
}

#if CLI_ONLY_INFERENCE
static int inference_cli_handler(int argc, char *argv[])
{
    /* Just to go to the next line */
    printf("\n");
    if (argc != 2) {
        printf("%s: Incorrect arguments\n", TAG);
        return 0;
    }
    int image_number = atoi(argv[1]);

    if((image_number < 0) || (image_number >= IMAGE_COUNT)) {
        ESP_LOGE(TAG, "Please Enter a valid Number ( 0 - %d)", IMAGE_COUNT-1);
        return -1;
    }
   // char file_name[30];
   // sprintf(file_name, "image%d.raw", image_number);
    unsigned detect_time;
    detect_time = esp_timer_get_time();
    run_inference((void *)image_database[image_number]);
    detect_time = (esp_timer_get_time() - detect_time)/1000;
    ESP_LOGI(TAG,"Time required for the inference is %u ms", detect_time);

    return 0;
}

int esp_cli_register_inference_command() {
    esp_console_cmd_t command = {
        .command = "detect_image",
        .help = "detect_image <image_number>"
                "Note: image numbers ranging from 0 - 9 only are valid",
        .func = inference_cli_handler,
    };
    esp_console_cmd_register(&command);
    return 0;
}
#endif

static esp_console_cmd_t diag_cmds[] = {
    {
        .command = "mem-dump",
        .help = "",
        .func = mem_dump_cli_handler,
    },
    {
        .command = "task-dump",
        .help = "",
        .func = task_dump_cli_handler,
    },
    {
        .command = "cpu-dump",
        .help = "",
        .func = cpu_dump_cli_handler,
    },
};

int esp_cli_register_cmds()
{
    int cmds_num = sizeof(diag_cmds) / sizeof(esp_console_cmd_t);
    int i;
    for (i = 0; i < cmds_num; i++) {
        ESP_LOGI(TAG, "Registering command: %s", diag_cmds[i].command);
        esp_console_cmd_register(&diag_cmds[i]);
    }
    return 0;
}

static void image_database_init()
{
#if CLI_ONLY_INFERENCE
    image_database[0] = (uint8_t *) image0_start;
    image_database[1] = (uint8_t *) image1_start;
    image_database[2] = (uint8_t *) image2_start;
    image_database[3] = (uint8_t *) image3_start;
    image_database[4] = (uint8_t *) image4_start;
    image_database[5] = (uint8_t *) image5_start;
    image_database[6] = (uint8_t *) image6_start;
    image_database[7] = (uint8_t *) image7_start;
    image_database[8] = (uint8_t *) image8_start;
    image_database[9] = (uint8_t *) image9_start;
    image_database[10] = (uint8_t *) image10_start;
    image_database[11] = (uint8_t *) image11_start;
    image_database[12] = (uint8_t *) image12_start;
    image_database[13] = (uint8_t *) image13_start;
    image_database[14] = (uint8_t *) image14_start;
    image_database[15] = (uint8_t *) image15_start;
    image_database[16] = (uint8_t *) image16_start;
    image_database[17] = (uint8_t *) image17_start;
    image_database[18] = (uint8_t *) image18_start;
    image_database[19] = (uint8_t *) image19_start;
    image_database[20] = (uint8_t *) image20_start;
    image_database[21] = (uint8_t *) image21_start;
    image_database[22] = (uint8_t *) image22_start;
    image_database[23] = (uint8_t *) image23_start;
    image_database[24] = (uint8_t *) image24_start;
    image_database[25] = (uint8_t *) image25_start;
    image_database[26] = (uint8_t *) image26_start;
    image_database[27] = (uint8_t *) image27_start;
    image_database[28] = (uint8_t *) image28_start;
    image_database[29] = (uint8_t *) image29_start;
    image_database[30] = (uint8_t *) image30_start;
    image_database[31] = (uint8_t *) image31_start;
    image_database[32] = (uint8_t *) image32_start;
    image_database[33] = (uint8_t *) image33_start;
    image_database[34] = (uint8_t *) image34_start;
    image_database[35] = (uint8_t *) image35_start;
    image_database[36] = (uint8_t *) image36_start;
    image_database[37] = (uint8_t *) image37_start;
    image_database[38] = (uint8_t *) image38_start;
    image_database[39] = (uint8_t *) image39_start;
    image_database[40] = (uint8_t *) image40_start;
    image_database[41] = (uint8_t *) image41_start;
    image_database[42] = (uint8_t *) image42_start;
    image_database[43] = (uint8_t *) image43_start;
    image_database[44] = (uint8_t *) image44_start;
    image_database[45] = (uint8_t *) image45_start;
    image_database[46] = (uint8_t *) image46_start;
    image_database[47] = (uint8_t *) image47_start;
    image_database[48] = (uint8_t *) image48_start;
    image_database[49] = (uint8_t *) image49_start;
    image_database[50] = (uint8_t *) image50_start;
#endif
}

extern void run_inference(void *ptr);

static void run_all_images(void)
{
    printf("latency_us,person_score,no_person_score,free_heap,min_heap,"
           "predicted,ground_truth,correct\n");

    for (int i = 0; i < IMAGE_COUNT; i++) {
        run_inference((void *)image_database[i]);
        vTaskDelay(pdMS_TO_TICKS(200)); 
    }

    //printf("Fim das inferencias.\n");
}

int esp_cli_start()
{
    image_database_init();

    run_all_images();

    return 0;
}


/*int esp_cli_start()
{
    image_database_init();
    static int cli_started;
    if (cli_started) {
        return 0;
    }

    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();

    esp_console_register_help_command();
    esp_cli_register_cmds();
#if defined(CONFIG_ESP_CONSOLE_UART_DEFAULT) || defined(CONFIG_ESP_CONSOLE_UART_CUSTOM)
    esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));

#elif defined(CONFIG_ESP_CONSOLE_USB_CDC)
    esp_console_dev_usb_cdc_config_t hw_config = ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_cdc(&hw_config, &repl_config, &repl));

#elif defined(CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG)
    esp_console_dev_usb_serial_jtag_config_t hw_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&hw_config, &repl_config, &repl));

#else
#error Unsupported console type
#endif
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
    cli_started = 1;
    return 0;
}*/