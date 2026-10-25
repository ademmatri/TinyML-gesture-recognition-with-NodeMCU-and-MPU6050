/* Includes ---------------------------------------------------------------- */
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ademmatri-project-1_inferencing.h>

// Définition des broches pour la LED RGB
#define RED_PIN   D5
#define GREEN_PIN D6
#define BLUE_PIN  D7

Adafruit_MPU6050 mpu;

// Tampon dynamique pour stocker les données en temps réel du MPU6050
float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

/**
 * @brief      Copie les données brutes des caractéristiques dans out_ptr
 *             Fonction appelée par la bibliothèque d'inférence
 */
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, buffer + offset, length * sizeof(float));
    return 0;
}

void print_inference_result(ei_impulse_result_t result);

void setup()
{
    Serial.begin(115200);
    
    // Configuration des broches de la LED RGB en sortie
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    
    // while (!Serial); // Commenté pour éviter le blocage sur ESP8266 au démarrage
    Serial.println("Edge Impulse Inferencing Demo - MPU6050");
    
    // Initialisation du capteur MPU6050
    if (!mpu.begin()) {
        Serial.println("Échec de la recherche de la puce MPU6050 !");
        while (1) { delay(10); }
    }
    Serial.println("MPU6050 initialisé avec succès !");
    
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop()
{
    ei_printf("Échantillonnage en cours...\n");

    // Échantillonnage du MPU6050 pour remplir le buffer en temps réel (~59 Hz)
    for (size_t i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i += 3) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        buffer[i + 0] = a.acceleration.x;
        buffer[i + 1] = a.acceleration.y;
        buffer[i + 2] = a.acceleration.z;

        // Délai pour respecter la fréquence d'échantillonnage (~17 ms)
        delay(17); 
    }

    ei_impulse_result_t result = { 0 };

    signal_t features_signal;
    features_signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
    features_signal.get_data = &raw_feature_get_data;

    // Lancer la classification de l'impulsion
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);
    if (res != EI_IMPULSE_OK) {
        ei_printf("ERR: Échec de l'exécution du classificateur (%d)\n", res);
        return;
    }

    // Afficher les résultats d'inférence dans le Moniteur Série
    print_inference_result(result);

    // --- Gestion de la LED RGB selon le geste détecté ---
    float circle_score = 0;
    float up_down_score = 0;
    float left_right_score = 0;

    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        String label = String(ei_classifier_inferencing_categories[i]);
        float value = result.classification[i].value;

        if (label == "circle") circle_score = value;
        else if (label == "up_down") up_down_score = value;
        else if (label == "left_right") left_right_score = value;
    }

    // Application de la couleur sur la LED RGB selon le score le plus élevé (> 0.8)
    if (circle_score > 0.8) {
        // Bleu pour "circle"
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, HIGH);
    } 
    else if (up_down_score > 0.8) {
        // Rouge pour "up_down"
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
    } 
    else if (left_right_score > 0.8) {
        // Vert pour "left_right"
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, LOW);
    } 
    else {
        // Éteint pour "idle" ou score incertain
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
    }
}

void print_inference_result(ei_impulse_result_t result) {
    ei_printf("Timing: DSP %d ms, inference %d ms, anomaly %d ms\r\n",
            result.timing.dsp,
            result.timing.classification,
            result.timing.anomaly);

    ei_printf("Predictions:\r\n");
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        ei_printf("  %s: ", ei_classifier_inferencing_categories[i]);
        ei_printf("%.5f\r\n", result.classification[i].value);
    }
}