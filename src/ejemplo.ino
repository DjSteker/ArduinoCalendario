#include <CalendarUtils.h>

void setup() {
    Serial.begin(9600);
    setTime(1615564800); // Fijar fecha inicial (12/3/2025)
}

void loop() {
    time_t now = now();
    
    // Ejemplo de uso
    int days = CalendarUtils::daysInMonth(month(now), year(now));
    Serial.print("Días en mes actual: ");
    Serial.println(days);

    delay(5000);
}
