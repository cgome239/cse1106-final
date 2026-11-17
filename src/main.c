#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pigpio.h>

#define PIN 17 // GPIO 17 (physical pin 11)

// save the current status to status.txt
void write_status(const char *status) {
    FILE *f = fopen("status.txt", "w");
    if (f == NULL) {
        printf("Could not open status.txt\n");
        return;
    }
    fprintf(f, "%s\n", status);
    fclose(f);
}

// append each temperature reading to the csv file
void save_to_csv(int num, double temp, const char *status) {
    FILE *f = fopen("data/temperature_log.csv", "a");
    if (f == NULL) {
        printf("Could not open temperature_log.csv\n");
        return;
    }

    // add headers if the file is empty
    fseek(f, 0, SEEK_END);
    if (ftell(f) == 0) {
        fprintf(f, "Reading,Temperature_F,Status\n");
    }

    fprintf(f, "%d,%.1f,%s\n", num, temp, status);
    fclose(f);
}

int main(int argc, char* argv[]) {
    if (gpioInitialise() < 0) {
        printf("pigpio setup failed\n");
        return 1;
    }

    gpioSetMode(PIN, PI_OUTPUT);
    gpioWrite(PIN, 0); // start with LED off

    // default comfort range
    double min_temp = 68.0;
    double max_temp = 76.0;

    double sum = 0.0;
    int count = 0;
    int option = 0;

    printf("Smart Classroom Comfort Monitor\n");

    while (1) {
        printf("\nAcceptable Range: %.1fF - %.1fF\n", min_temp, max_temp);
        printf("1. Enter temperature\n");
        printf("2. Change temperature range\n");
        printf("3. Quit\n");
        printf("Choice: ");

        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n'); // clear bad input
            printf("Please enter 1, 2, or 3.\n");
            continue;
        }

        if (option == 1) {
            double temp;
            printf("Enter temp (F): ");
            if (scanf("%lf", &temp) != 1) {
                while (getchar() != '\n');
                printf("Invalid number.\n");
                continue;
            }

            sum += temp;
            count++;
            double avg = sum / count;

            char status[16];
            if (temp >= min_temp && temp <= max_temp) {
                strcpy(status, "NORMAL");
                gpioWrite(PIN, 0); // LED off
            } else {
                strcpy(status, "WARNING");
                gpioWrite(PIN, 1); // LED on
            }

            printf("\nTemp: %.1fF | Status: %s | LED: %s\n", 
                   temp, status, (temp >= min_temp && temp <= max_temp) ? "OFF" : "ON");
            printf("Average Temp: %.1fF (%d readings)\n", avg, count);

            write_status(status);
            save_to_csv(count, temp, status);

        } else if (option == 2) {
            double new_min, new_max;
            printf("New min temp (F): ");
            scanf("%lf", &new_min);
            printf("New max temp (F): ");
            scanf("%lf", &new_max);

            if (new_min < new_max) {
                min_temp = new_min;
                max_temp = new_max;
                printf("Updated range to %.1fF - %.1fF\n", min_temp, max_temp);
            } else {
                printf("Min temp has to be lower than max temp.\n");
            }

        } else if (option == 3) {
            gpioWrite(PIN, 0); // turn off LED before quitting
            gpioTerminate();
            printf("Done.\n");
            break;

        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}

