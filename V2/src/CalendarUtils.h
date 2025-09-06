// CalendarUtils.h - Versión Mejorada
#pragma once
#include <TimeLib.h>

struct DateInfo {
    int year;
    int month;
    int day;
    int dayOfWeek;  // 0=Domingo, 1=Lunes, etc.
    int dayOfYear;  // 1-366
    int weekOfYear; // 1-53
    bool isValid;
};

struct TimeInfo {
    int hour;
    int minute;
    int second;
    bool isAM;      // true para AM, false para PM
};

class CalendarUtils {
private:
    // Nombres de días y meses para mostrar
    static const char* dayNames[7];
    static const char* monthNames[12];
    static const char* monthNamesShort[12];

public:
    // ========== FUNCIONES BÁSICAS MEJORADAS ==========
    static bool isLeapYear(int year) {
        if (year < 1) return false;
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    static int daysInMonth(int month, int year) {
        if (month < 1 || month > 12 || year < 1) return 0;
        
        switch(month) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                return 31;
            case 4: case 6: case 9: case 11:
                return 30;
            case 2:
                return isLeapYear(year) ? 29 : 28;
            default:
                return 0;
        }
    }

    static long daysBetween(time_t start, time_t end) {
        return abs((long)(end - start) / SECS_PER_DAY);
    }

    static time_t addDays(time_t date, int days) {
        return date + ((long)days * SECS_PER_DAY);
    }

    static int daysInYear(int year) {
        if (year < 1) return 0;
        return isLeapYear(year) ? 366 : 365;
    }

    // ========== NUEVAS FUNCIONES ÚTILES ==========
    
    // Validar si una fecha es válida
    static bool isValidDate(int day, int month, int year) {
        if (year < 1 || month < 1 || month > 12 || day < 1) {
            return false;
        }
        return day <= daysInMonth(month, year);
    }

    // Obtener el día de la semana (0=Domingo, 1=Lunes, etc.)
    static int getDayOfWeek(int day, int month, int year) {
        if (!isValidDate(day, month, year)) return -1;
        
        // Algoritmo de Zeller modificado
        int q = day;
        int m = month;
        int k = year % 100;
        int j = year / 100;
        
        if (m < 3) {
            m += 12;
            k--;
            if (k < 0) {
                k = 99;
                j--;
            }
        }
        
        int h = (q + (13 * (m + 1)) / 5 + k + k/4 + j/4 - 2*j) % 7;
        return (h + 5) % 7;  // Ajustar para que 0=Domingo
    }

    // Obtener día del año (1-366)
    static int getDayOfYear(int day, int month, int year) {
        if (!isValidDate(day, month, year)) return 0;
        
        int dayOfYear = day;
        for (int i = 1; i < month; i++) {
            dayOfYear += daysInMonth(i, year);
        }
        return dayOfYear;
    }

    // Obtener semana del año
    static int getWeekOfYear(int day, int month, int year) {
        if (!isValidDate(day, month, year)) return 0;
        
        int dayOfYear = getDayOfYear(day, month, year);
        int jan1DayOfWeek = getDayOfWeek(1, 1, year);
        
        // Ajustar para que la semana empiece el lunes
        int adjustedDayOfWeek = (jan1DayOfWeek == 0) ? 6 : jan1DayOfWeek - 1;
        
        return (dayOfYear + adjustedDayOfWeek - 1) / 7 + 1;
    }

    // Crear estructura DateInfo completa
    static DateInfo getDateInfo(time_t timestamp) {
        DateInfo info;
        info.year = year(timestamp);
        info.month = month(timestamp);
        info.day = day(timestamp);
        info.isValid = isValidDate(info.day, info.month, info.year);
        
        if (info.isValid) {
            info.dayOfWeek = getDayOfWeek(info.day, info.month, info.year);
            info.dayOfYear = getDayOfYear(info.day, info.month, info.year);
            info.weekOfYear = getWeekOfYear(info.day, info.month, info.year);
        } else {
            info.dayOfWeek = -1;
            info.dayOfYear = 0;
            info.weekOfYear = 0;
        }
        
        return info;
    }

    // Crear estructura TimeInfo
    static TimeInfo getTimeInfo(time_t timestamp) {
        TimeInfo info;
        info.hour = hour(timestamp);
        info.minute = minute(timestamp);
        info.second = second(timestamp);
        info.isAM = info.hour < 12;
        return info;
    }

    // ========== FUNCIONES DE FORMATO Y DISPLAY ==========
    
    // Obtener nombre del día
    static const char* getDayName(int dayOfWeek) {
        if (dayOfWeek < 0 || dayOfWeek > 6) return "Invalid";
        return dayNames[dayOfWeek];
    }

    // Obtener nombre del mes
    static const char* getMonthName(int month, bool shortName = false) {
        if (month < 1 || month > 12) return "Invalid";
        return shortName ? monthNamesShort[month-1] : monthNames[month-1];
    }

    // Formatear fecha como string (DD/MM/YYYY)
    static void formatDate(time_t timestamp, char* buffer, int bufferSize) {
        if (!buffer || bufferSize < 11) return;
        
        DateInfo date = getDateInfo(timestamp);
        if (date.isValid) {
            snprintf(buffer, bufferSize, "%02d/%02d/%04d", 
                    date.day, date.month, date.year);
        } else {
            snprintf(buffer, bufferSize, "Invalid");
        }
    }

    // Formatear hora como string (HH:MM:SS)
    static void formatTime(time_t timestamp, char* buffer, int bufferSize, bool use12Hour = false) {
        if (!buffer || bufferSize < 9) return;
        
        TimeInfo time = getTimeInfo(timestamp);
        
        if (use12Hour) {
            int displayHour = time.hour == 0 ? 12 : (time.hour > 12 ? time.hour - 12 : time.hour);
            snprintf(buffer, bufferSize, "%02d:%02d:%02d %s", 
                    displayHour, time.minute, time.second, time.isAM ? "AM" : "PM");
        } else {
            snprintf(buffer, bufferSize, "%02d:%02d:%02d", 
                    time.hour, time.minute, time.second);
        }
    }

    // Formatear fecha y hora completa
    static void formatDateTime(time_t timestamp, char* buffer, int bufferSize, bool use12Hour = false) {
        if (!buffer || bufferSize < 20) return;
        
        DateInfo date = getDateInfo(timestamp);
        TimeInfo time = getTimeInfo(timestamp);
        
        if (date.isValid) {
            if (use12Hour) {
                int displayHour = time.hour == 0 ? 12 : (time.hour > 12 ? time.hour - 12 : time.hour);
                snprintf(buffer, bufferSize, "%02d/%02d/%04d %02d:%02d:%02d %s",
                        date.day, date.month, date.year,
                        displayHour, time.minute, time.second, time.isAM ? "AM" : "PM");
            } else {
                snprintf(buffer, bufferSize, "%02d/%02d/%04d %02d:%02d:%02d",
                        date.day, date.month, date.year,
                        time.hour, time.minute, time.second);
            }
        } else {
            snprintf(buffer, bufferSize, "Invalid DateTime");
        }
    }

    // ========== FUNCIONES DE CÁLCULO AVANZADAS ==========
    
    // Agregar meses (maneja cambios de año)
    static time_t addMonths(time_t timestamp, int months) {
        DateInfo date = getDateInfo(timestamp);
        if (!date.isValid) return timestamp;
        
        int newMonth = date.month + months;
        int newYear = date.year;
        
        // Ajustar año si es necesario
        while (newMonth > 12) {
            newMonth -= 12;
            newYear++;
        }
        while (newMonth < 1) {
            newMonth += 12;
            newYear--;
        }
        
        // Ajustar día si el mes destino tiene menos días
        int maxDays = daysInMonth(newMonth, newYear);
        int newDay = date.day > maxDays ? maxDays : date.day;
        
        tmElements_t tm;
        tm.Year = newYear - 1970;
        tm.Month = newMonth;
        tm.Day = newDay;
        tm.Hour = hour(timestamp);
        tm.Minute = minute(timestamp);
        tm.Second = second(timestamp);
        
        return makeTime(tm);
    }

    // Agregar años
    static time_t addYears(time_t timestamp, int years) {
        return addMonths(timestamp, years * 12);
    }

    // Obtener el primer día del mes
    static time_t getFirstDayOfMonth(time_t timestamp) {
        DateInfo date = getDateInfo(timestamp);
        if (!date.isValid) return timestamp;
        
        tmElements_t tm;
        tm.Year = date.year - 1970;
        tm.Month = date.month;
        tm.Day = 1;
        tm.Hour = 0;
        tm.Minute = 0;
        tm.Second = 0;
        
        return makeTime(tm);
    }

    // Obtener el último día del mes
    static time_t getLastDayOfMonth(time_t timestamp) {
        DateInfo date = getDateInfo(timestamp);
        if (!date.isValid) return timestamp;
        
        tmElements_t tm;
        tm.Year = date.year - 1970;
        tm.Month = date.month;
        tm.Day = daysInMonth(date.month, date.year);
        tm.Hour = 23;
        tm.Minute = 59;
        tm.Second = 59;
        
        return makeTime(tm);
    }

    // Calcular edad en años
    static int calculateAge(time_t birthDate, time_t currentDate = 0) {
        if (currentDate == 0) currentDate = now();
        
        DateInfo birth = getDateInfo(birthDate);
        DateInfo current = getDateInfo(currentDate);
        
        if (!birth.isValid || !current.isValid) return -1;
        
        int age = current.year - birth.year;
        
        // Ajustar si aún no ha cumplido años este año
        if (current.month < birth.month || 
            (current.month == birth.month && current.day < birth.day)) {
            age--;
        }
        
        return age;
    }

    // ========== FUNCIONES DE DEBUG Y DISPLAY ==========
    
    // Imprimir información completa de fecha
    static void printDateInfo(time_t timestamp) {
        DateInfo date = getDateInfo(timestamp);
        TimeInfo time = getTimeInfo(timestamp);
        
        if (date.isValid) {
            Serial.println("=== INFORMACIÓN DE FECHA ===");
            Serial.print("Fecha: ");
            Serial.print(getDayName(date.dayOfWeek));
            Serial.print(", ");
            Serial.print(date.day);
            Serial.print(" de ");
            Serial.print(getMonthName(date.month));
            Serial.print(" de ");
            Serial.println(date.year);
            
            char buffer[32];
            formatTime(timestamp, buffer, sizeof(buffer), true);
            Serial.print("Hora: ");
            Serial.println(buffer);
            
            Serial.print("Día del año: ");
            Serial.println(date.dayOfYear);
            Serial.print("Semana del año: ");
            Serial.println(date.weekOfYear);
            Serial.print("Año bisiesto: ");
            Serial.println(isLeapYear(date.year) ? "Sí" : "No");
            Serial.println("============================");
        } else {
            Serial.println("Fecha inválida");
        }
    }
};

// ========== DEFINICIÓN DE ARRAYS ESTÁTICOS ==========
const char* CalendarUtils::dayNames[7] = {
    "Domingo", "Lunes", "Martes", "Miércoles", 
    "Jueves", "Viernes", "Sábado"
};

const char* CalendarUtils::monthNames[12] = {
    "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
    "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"
};

const char* CalendarUtils::monthNamesShort[12] = {
    "Ene", "Feb", "Mar", "Abr", "May", "Jun",
    "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"
};
