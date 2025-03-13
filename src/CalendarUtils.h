// CalendarUtils.h
#include <TimeLib.h>

class CalendarUtils {
public:
    static bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    static int daysInMonth(int month, int year) {
        switch(month) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                return 31;
            case 4: case 6: case 9: case 11:
                return 30;
            case 2:
                return isLeapYear(year) ? 29 : 28;
            default:
                return 0; // Mejor que lanzar excepción en Arduino
        }
    }

    static long daysBetween(time_t start, time_t end) {
        return (end - start) / SECS_PER_DAY;
    }

    static time_t addDays(time_t date, int days) {
        return date + (days * SECS_PER_DAY);
    }

    static int daysInYear(int year) {
        return isLeapYear(year) ? 366 : 365;
    }
};
