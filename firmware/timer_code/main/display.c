#include "display.h"
#include "app_state.h"
#include "tft_init.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
 
static const char *TAG = "DISPLAY";


void display_init(void)
{
    tft_controller_init(TFT_MODE_1);

    tft_fill_screen(0xFFFF);
    ESP_LOGI(TAG, "Display initialise");
}

static const char *state_to_str(timer_state_t state){
    switch (state)
    {
        case TIMER_IDLE:        return "IDLE";
        case TIMER_RUNNING:     return "RUNNING";
        case TIMER_FINISHED:    return "FINISHED";
        default:                return ".....";
    }
}

// Centre horizontalement une chaine (scale donne) sur la largeur d'ecran
/*static uint16_t center_x(const char *str, uint8_t scale){
    uint16_t w = tft_string_width_scaled(str, scale);
    uint16_t screen_w = tft_get_width();
    return (screen_w > w) ? (screen_w - w) / 2 : 0;
}
 
static void draw_centered_scaled(uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t scale){
    tft_draw_string_scaled(center_x(str, scale), y, str, color, bg, scale);
}
 
// ---------------------------------------------------------------------
// Ecran IDLE : affiche le temps regle via le potentiometre + jauge
// ---------------------------------------------------------------------
static void draw_idle_static(void){
    tft_fill_screen(COL_WHITE);
    draw_centered_scaled(TITLE_Y, "REGLAGE", COL_BLUE, COL_WHITE, TITLE_SCALE);
    draw_centered_scaled(HINT_Y, "Tournez le bouton", COL_GRAY, COL_WHITE, HINT_SCALE);
    draw_centered_scaled(HINT_Y + 12, "puis appuyez sur START", COL_GRAY, COL_WHITE, HINT_SCALE);
}
 
static void draw_idle_dynamic(const timer_info_t *info){
    char buf[16];
    int minutes = info->seconds_set / 60;
    int seconds = info->seconds_set % 60;
    snprintf(buf, sizeof(buf), "%02d:%02d", minutes, seconds);
 
    draw_centered_scaled(TIME_Y, buf, COL_BLUE, COL_WHITE, TIME_SCALE);
 
    float fraction = (float)info->seconds_set / (float)TIMER_MAX_SECONDS;
    tft_draw_gauge(GAUGE_X, GAUGE_Y, GAUGE_W, GAUGE_H, fraction,
                   COL_BLUE, COL_GRAY, COL_BLACK);
}
 
// ---------------------------------------------------------------------
// Ecran RUNNING : chrono qui s'ecoule + jauge qui se vide
// ---------------------------------------------------------------------
static void draw_running_static(void){
    tft_fill_screen(COL_BLACK);
    draw_centered_scaled(TITLE_Y, "MINUTEUR EN COURS", COL_GREEN, COL_BLACK, TITLE_SCALE);
}
 
static void draw_running_dynamic(const timer_info_t *info){
    char buf[16];
    int minutes = info->seconds_remaining / 60;
    int seconds = info->seconds_remaining % 60;
    snprintf(buf, sizeof(buf), "%02d:%02d", minutes, seconds);
 
    draw_centered_scaled(TIME_Y, buf, COL_WHITE, COL_BLACK, TIME_SCALE);
 
    float fraction = (info->seconds_set > 0)
        ? (float)info->seconds_remaining / (float)info->seconds_set
        : 0.0f;
 
    // Couleur qui vire du vert -> orange -> rouge au fil du temps
    uint16_t fill_color;
    if (fraction > 0.5f)      fill_color = COL_GREEN;
    else if (fraction > 0.2f) fill_color = COL_ORANGE;
    else                       fill_color = COL_RED;
 
    tft_draw_gauge(GAUGE_X, GAUGE_Y, GAUGE_W, GAUGE_H, fraction,
                   fill_color, COL_GRAY, COL_WHITE);
}
 
// ---------------------------------------------------------------------
// Ecran FINISHED : ecran de fin avec un petit clignotement
// ---------------------------------------------------------------------
static void draw_finished_static(void){
    tft_fill_screen(COL_DARKBLUE);
 
    // Cadre decoratif
    tft_fill_rect(10, 10, tft_get_width() - 20, 3, COL_YELLOW);
    tft_fill_rect(10, tft_get_height() - 13, tft_get_width() - 20, 3, COL_YELLOW);
    tft_fill_rect(10, 10, 3, tft_get_height() - 20, COL_YELLOW);
    tft_fill_rect(tft_get_width() - 13, 10, 3, tft_get_height() - 20, COL_YELLOW);
 
    draw_centered_scaled(90, "TEMPS ECOULE !", COL_YELLOW, COL_DARKBLUE, 3);
    draw_centered_scaled(HINT_Y, "Appuyez sur STOP", COL_CYAN, COL_DARKBLUE, HINT_SCALE);
}
 
static void draw_finished_blink(bool on){
    // Fait clignoter une petite icone "cloche" simplifiee (rectangle) sous le texte
    uint16_t color = on ? COL_YELLOW : COL_DARKBLUE;
    uint16_t w = 16, h = 16;
    uint16_t x = (tft_get_width() - w) / 2;
    uint16_t y = 140;
    tft_fill_rect(x, y, w, h, color);
}*/


/*void display_task(void *pvParameters){

    while (g_state_mutex == NULL) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    while (1)
    {
        timer_info_t info;
        app_state_get(&info);

        int minutes = info.seconds_remaining / 60;
        int seconds = info.seconds_remaining % 60;

        if (info.state == TIMER_IDLE)
        {
            tft_draw_string(10, 10, "state_to_str(TIMER_IDLE)",0xF800, 0xFFFF );
        }else if (info.state == TIMER_RUNNING)
        {
            ESP_LOGI(TAG, "%02d:%02d   [%s]", minutes, seconds, state_to_str(info.state));
            //tft_fill_screen(0x0000);
            tft_draw_string(10, 10, state_to_str(TIMER_RUNNING),0xF800, 0xFFFF);
        }
        
        



        // completer avec ecran tft

        //ESP_LOGI(TAG, "%02d:%02d   [%s]", minutes, seconds, state_to_str(info.state));
        
        vTaskDelay(pdMS_TO_TICKS(200));
    }
    
}*/


void display_task(void *pvParameters){

    // Attente que le mutex de l'état soit prêt
    while (g_state_mutex == NULL) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // On force un état initial différent pour déclencher le premier tft_fill_screen
    timer_state_t last_state = -1; 

    while (1)
    {
        timer_info_t info;
        app_state_get(&info);

        int minutes = info.seconds_remaining / 60;
        int seconds = info.seconds_remaining % 60;

        // 🚨 GESTION DU CHANGEMENT D'ÉTAT (On efface l'écran UNE SEULE fois lors d'une transition)
        if (info.state != last_state) {
            ESP_LOGI(TAG, "Changement d'etat detecte: %s", state_to_str(info.state));
            if (info.state == TIMER_RUNNING) {
                tft_fill_screen(0x0000); // Fond NOIR en mode running
            } else {
                tft_fill_screen(0xFFFF); // Fond BLANC pour les autres modes
            }
            last_state = info.state;
        }

        // 🖌️ DESSIN DE L'ÉCRAN SELON L'ÉTAT
        if (info.state == TIMER_IDLE)
        {
            // Fond Blanc (0xFFFF), Texte Rouge (0xF800)
            ESP_LOGI(TAG, "STATUT: IDLE");
            tft_draw_string(10, 40, "STATUT: IDLE",2, 0xF800, 0xFFFF);
            //draw_string(40, 40, "B", 0xF800, 0xFFFF);
        }
        else if (info.state == TIMER_RUNNING)
        {
            ESP_LOGI(TAG, "%02d:%02d   [%s]", minutes, seconds, state_to_str(info.state));
            
            // 1. Afficher le statut en haut (Texte Vert 0x07E0 sur fond Noir 0x0000)
            tft_draw_string(10, 10, "STATUT: RUNNING",1, 0x07E0, 0x0000);

            // 2. Préparer et afficher le Chrono en gros au milieu
            char time_str[32];
            snprintf(time_str, sizeof(time_str), "TEMPS: %02d:%02d", minutes, seconds);
            
            // Texte Blanc (0xFFFF) sur fond Noir (0x0000)
            //tft_draw_string(10, 40, time_str,2, 0x03E0, 0x0000); 
            draw_string(10, 40, time_str, 0x03E0, 0x0000);
        }
        else if (info.state == TIMER_FINISHED)
        {
            // Fond Blanc (0xFFFF), Texte Rouge (0xF800) Blinking ou fixe
            tft_draw_string(10, 40, "STATUT: FINISHED",2, 0xF800, 0xFFFF);
            //ESP_LOGI(TAG, "Timer finished");
        }
        
        // Rafraîchissement régulier (4 fois par seconde, parfait pour un chrono)
        vTaskDelay(pdMS_TO_TICKS(250)); 
    }
}