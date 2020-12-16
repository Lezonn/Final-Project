#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void getTime() {
    struct tm* now;
    time_t t = time(NULL);
    now = localtime(&t);
    const char* day[7] = {
        "Mon", "Tue", "Wed",
        "Thu", "Fri", "Sat", "Sun"
    };

    const char* month[12] = {
        "Jan", "Feb", "Mar",
        "Apr", "May", "Jun",
        "Jul", "Aug", "Sep",
        "Oct", "Nov", "Dec"
    };
    printf("%s %s %d %02d:%02d:%02d %d\n",
        day[now->tm_wday - 1], month[now->tm_mon],
        now->tm_mday, now->tm_hour, now->tm_min,
        now->tm_sec, now->tm_year + 1900);
}

void clrscr() {
    system("cls||clear");
}

void pressEnter() {
    printf("Press enter to continue..."); getchar();
}

int strcmpi(char *str1, const char* str2) {
    for(int i = 0; i < str1[i]; i++) {
        if(str1[i] >= 'A' && str1[i] <= 'Z') {
            str1[i] += 32;
        }
    }
    return strcmp(str1, str2);
}

unsigned long DJB2(char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % 26;
}
