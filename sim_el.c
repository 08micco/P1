#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define APPLIANCE_MAX 5
#define PLUGS_MAX 10
#define DAYS_SIMULATED 8

enum appliances
{
    microwave = 1,
    kettle,
    oven,
    refrigerator,
    freezer
};
typedef enum appliances appliances;

char *appliances_string[APPLIANCE_MAX + 1] = {"empty", "microwave", "kettle",
                                              "oven", "refrigerator", "freezer"};

struct appliance
{
    int id;
    double power_consumption;
};
typedef struct appliance appliance;

struct user_data
{
    appliance plug[PLUGS_MAX];
};
typedef struct user_data user_data;

struct average_data
{
    appliance appliances[APPLIANCE_MAX];
};
typedef struct average_data average_data;

void print_data(user_data);
user_data initialize_user_data(user_data, double[]);
double random_num(double, double);
void write_appliance_data_to_file(user_data[]);
int calc_time(struct tm *, int, int);

int main()
{
    user_data user[DAYS_SIMULATED];
    srand(time(NULL)); /* Skal den være her??? */

    /* Min and max values for each appliance */
    /* Warning ved gcc -ansi -Wall -pedantic */
    double min_max_appliances[APPLIANCE_MAX * 2] = {
        /* Microwave */
        0.1115,
        0.4460,
        /* Kettle */
        0.1890,
        0.7560,
        /* Oven */
        0.4438,
        1.4793,
        /* Refigerator */
        0.5023,
        0.5552,
        /* Freezer */
        0.1578,
        0.6311};

    int i;
    for (i = 0; i < DAYS_SIMULATED; i++)
    {
        user[i] = initialize_user_data(user[i], min_max_appliances);
        print_data(user[i]);
    }

    write_appliance_data_to_file(user);

    return 0;
}

/* Prints user_data */
void print_data(user_data data)
{
    int i;
    for (i = 0; i < APPLIANCE_MAX; i++)
    {
        printf("%s: id = %d, consumption = %f kWh\n", appliances_string[i + 1], data.plug[i].id, data.plug[i].power_consumption);
    }
    printf("\n");
}

/* Initializes user_data by adding all appliances with an ID and power_consumption */
user_data initialize_user_data(user_data data, double min_max_appliances[])
{
    int i;
    for (i = 1; i <= APPLIANCE_MAX; i++)
    {
        data.plug[i - 1].id = i;
        data.plug[i - 1].power_consumption = random_num(min_max_appliances[i * 2 - 1], min_max_appliances[i * 2]);
    }

    return data;
}

/* Generates a random number between two doubles */
double random_num(double max, double min)
{
    /* Den returnerer ik et tal mellem min og max ved køleskabet??? */
    return (rand() / (RAND_MAX / (max - min))) + min;
}

/* This function writes household size, appliance number and their power consumption to a file */
/* Lots of spaces and newlines printed to file to make it more readable */
void write_appliance_data_to_file(user_data user[])
{
    FILE *file;
    int i, x; /* Days er midlertidig */

    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    /* Creates or edits new file user_data.json under the data folder */
    file = fopen("data/user_data.json", "w+");

    /* If file doesn't exist, call an error */
    if (file == NULL)
    {
        printf("\nError while creating file\n");
        exit(1);
    }
    fprintf(file, "{\n"); /* Start curly bracket */
    fprintf(file, " \"days_simulated\": \"%d\",", DAYS_SIMULATED);

    /* For dates */
    for (i = 0; i < DAYS_SIMULATED /* DATES */; i++)
    {
        fprintf(file, "\n   \"%d\": {\n", calc_time(current_time, i, DAYS_SIMULATED)); /* Date */

        /* For all appliances */
        for (x = 0; x < APPLIANCE_MAX; x++)
        {
            fprintf(file, "     \"%s\": {\n", appliances_string[x + 1]);                                       /* Writes appliance name */
            fprintf(file, "         \"appliance_id\": %d,\n", user[i].plug[x].id);                             /* Writes appliance ID */
            fprintf(file, "         \"power_consumption\": %f\n        }", user[i].plug[x].power_consumption); /* Writes appliance power consumption */

            /* If we are on the last element: don't end with comma as it is not the correct syntax for .json */
            if (x != APPLIANCE_MAX - 1)
                fprintf(file, ",");
            fprintf(file, "\n");
        }
        fprintf(file, "    }");
        if (i != DAYS_SIMULATED - 1 /* DATES */)
            fprintf(file, ",");
    }
    fprintf(file, "\n}"); /* Close curly bracket */

    fclose(file);
}

int calc_time(struct tm *current_time, int index, int days_simulated)
{
    current_time->tm_mday = current_time->tm_mday - (days_simulated - index);

    time_t new_time = mktime(current_time); /* Warning ved gcc -ansi -Wall -pedantic */

    return new_time;
}
