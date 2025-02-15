#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"  

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
const uint BUTTON_A_PIN = 5;  
const uint BUTTON_B_PIN = 6;  
const uint JOYSTICK_LEFT_PIN = 26;  
const uint JOYSTICK_RIGHT_PIN = 27;  
const uint JOYSTICK_BUTTON_PIN = 22;  
const uint LED_MATRIX_DATA_PIN = 7;  

// Padrões para matriz de LED
int matriz[5][5][3] = {
    {{0, 0, 0}, {0, 0, 0}, {2, 247, 47}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {2, 247, 47}, {0, 0, 0}, {2, 247, 47}, {0, 0, 0}},
    {{2, 247, 47}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {2, 247, 47}},
    {{2, 247, 47}, {0, 0, 0}, {2, 247, 47}, {0, 0, 0}, {2, 247, 47}},
    {{0, 0, 0}, {2, 247, 47}, {0, 0, 0}, {2, 247, 47}, {0, 0, 0}}
};

int matriz2[5][5][3] = {
    {{0, 0, 0}, {0, 0, 0}, {247, 29, 2}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {247, 29, 2}, {0, 0, 0}, {247, 29, 2}, {0, 0, 0}},
    {{247, 29, 2}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {247, 29, 2}},
    {{247, 29, 2}, {0, 0, 0}, {247, 29, 2}, {0, 0, 0}, {247, 29, 2}},
    {{0, 0, 0}, {247, 29, 2}, {0, 0, 0}, {247, 29, 2}, {0, 0, 0}}
};

// Estrutura para questões e respostas 
typedef struct {
    char *question;
    char *answer_a;
    char *answer_b;
    int correct_answer;  // 0 para A, 1 para B
} Question;

// Array de questões
Question questions[] = {
    {"8 x 9 = ?", "A: 72", "B: 81", 0},  
    {"50 L 5 = ?", "A: 10", "B: 5", 0},   
    {"7 x 6 = ?", "A: 49", "B: 42", 1},  
    {"5 x 25 = ?", "A: 125", "B: 75", 0},
    {"6 x 7 = ?", "A: 42", "B: 52", 0},
    {"76 L 4 = ?", "A: 21", "B: 19", 1},
    {"4 x 6 = ?", "A: 24", "B: 28", 0},
    {"3 x 9 = ?", "A: 24", "B: 27", 1},
    {"4 x 7 = ?", "A: 28", "B: 32", 0},
    {"90 L 3 = ?", "A: 27", "B: 30", 1},
    {"15 L 5 = ?", "A: 5", "B: 3", 1},
    {"9 x 6 = ?", "A: 54", "B: 56", 0},
    {"42 L 6 = ?", "A: 6", "B: 7", 1},
    {"9 x 9 = ?", "A: 81", "B: 72", 0},
    {"6 x 8 = ?", "A: 48", "B: 52", 0},
    {"16 L 4 = ?", "A: 6", "B: 4", 1},
    {"8 L 4 = ?", "A: 4", "B: 2", 1},
    {"11 x 11 = ?", "A: 121", "B: 111", 0},
    {"36 L 4 = ?", "A: 9", "B: 7", 0}
};

int total_questions = sizeof(questions) / sizeof(questions[0]);  
int current_question = 0;  
int correct_answers = 0;  // Counter para questões corretas

// Configuração WS2812 PIO 
PIO pio = pio0;
uint sm = 0;

void initialize_joystick() {
    // Inicialização joystick
    gpio_init(JOYSTICK_LEFT_PIN);
    gpio_set_dir(JOYSTICK_LEFT_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_LEFT_PIN);

    gpio_init(JOYSTICK_RIGHT_PIN);
    gpio_set_dir(JOYSTICK_RIGHT_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_RIGHT_PIN);

    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);
}

void initialize_led_matrix() {
    // Inicialização WS2812 PIO 
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, LED_MATRIX_DATA_PIN, 800000, false);  // Correct number of arguments
}

void set_led_matrix_pattern(int pattern[5][5][3]) {
    // Conversão de padrão 5x5 para array
    uint32_t led_data[25];  // 5x5 = 25 LEDs
    int index = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            uint8_t red = pattern[i][j][0];
            uint8_t green = pattern[i][j][1];
            uint8_t blue = pattern[i][j][2];
            led_data[index++] = (green << 16) | (red << 8) | blue;  
        }
    }

    // Envia informações para o LED
    for (int i = 0; i < 25; i++) {
        pio_sm_put_blocking(pio, sm, led_data[i]);
    }
}

void clear_led_matrix() {
    // Desliga o LED
    for (int i = 0; i < 25; i++) {
        pio_sm_put_blocking(pio, sm, 0);
    }
}

int main()
{
    stdio_init_all();   

    // Inicialização I2C
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicialização display OLED
    ssd1306_init();

    // Inicialização botões
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    // Inicialização joystick
    initialize_joystick();

    // Inicialização matriz de LED
    initialize_led_matrix();

    // Preparação para display
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // Limpar o display
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    while (true) {
        // Mostrar a questão atual
        ssd1306_draw_string(ssd, 5, 0, questions[current_question].question);
        ssd1306_draw_string(ssd, 5, 16, questions[current_question].answer_a);
        ssd1306_draw_string(ssd, 5, 32, questions[current_question].answer_b);
        render_on_display(ssd, &frame_area);

        // Checar se o botão A foi acionado 
        if (!gpio_get(BUTTON_A_PIN)) {
            if (questions[current_question].correct_answer == 0) {
                ssd1306_draw_string(ssd, 5, 48, "Correto!");
                correct_answers++;  // Increment correct answer counter
                set_led_matrix_pattern(matriz2);  // Mostra o padrão de LED para acerto
            } else {
                ssd1306_draw_string(ssd, 5, 48, "Incorreto!");
                set_led_matrix_pattern(matriz);  // Mostra o padrão de LED para errado
            }
            render_on_display(ssd, &frame_area);
            sleep_ms(2000);  // Mostra a resposta por 2 segundos

            // Limpa a matriz de LED
            clear_led_matrix();

            // Proxima pergunta
            current_question = (current_question + 1) % total_questions;
            memset(ssd, 0, ssd1306_buffer_length);  // Limpa o display
            render_on_display(ssd, &frame_area);
        }

        // Checar se o botão B foi pressionado 
        if (!gpio_get(BUTTON_B_PIN)) {
            if (questions[current_question].correct_answer == 1) {
                ssd1306_draw_string(ssd, 5, 48, "Correto!");
                correct_answers++;  // Increment correct answer counter
                set_led_matrix_pattern(matriz2);  // Mostra o padrão de LED para correto
            } else {
                ssd1306_draw_string(ssd, 5, 48, "Incorreto!");
                set_led_matrix_pattern(matriz);  // Mostra o padrão de LED para errado
            }
            render_on_display(ssd, &frame_area);
            sleep_ms(2000);  // Mostra resposta por 2 segundos 

            // Limpa a matriz
            clear_led_matrix();

            // Proxima pergunta
            current_question = (current_question + 1) % total_questions;
            memset(ssd, 0, ssd1306_buffer_length);  // Limpa o display
            render_on_display(ssd, &frame_area);
        }

        // Checa o joystick (questão anterior)
        if (!gpio_get(JOYSTICK_LEFT_PIN)) {
            current_question = (current_question - 1 + total_questions) % total_questions;
            memset(ssd, 0, ssd1306_buffer_length);  // Limpa o display
            render_on_display(ssd, &frame_area);
            sleep_ms(200);  // Debounce delay
        }

        // Checa o joystick (proxima pergunta)
        if (!gpio_get(JOYSTICK_RIGHT_PIN)) {
            current_question = (current_question + 1) % total_questions;
            memset(ssd, 0, ssd1306_buffer_length);  // Limpa o display
            render_on_display(ssd, &frame_area);
            sleep_ms(200);  // Debounce delay
        }

        // Checa o joystick (reseta para a primeira questão)
        if (!gpio_get(JOYSTICK_BUTTON_PIN)) {
            current_question = 0;  // Reseta para primeira questão
            correct_answers = 0;  // Reseta o contador de respostas corretas
            memset(ssd, 0, ssd1306_buffer_length);  // Limpa o display
            render_on_display(ssd, &frame_area);
            sleep_ms(200);  // Debounce delay
        }

        // Mostra no display quantas questões foram acertadas
        char correct_answers_str[20];
        snprintf(correct_answers_str, sizeof(correct_answers_str), "Correto: %d", correct_answers);
        ssd1306_draw_string(ssd, 5, 48, correct_answers_str);
        render_on_display(ssd, &frame_area);

        sleep_ms(100);  
    }

    return 0;
}
