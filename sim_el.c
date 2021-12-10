#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define APPLIANCE_MAX 5
#define PLUGS_MAX 10
//#define DAYS_SIMULATED 18

enum appliances
{
    microwave = 1,
    kettle,
    oven,
    refrigerator,
    coffee_machine
};
typedef enum appliances appliances;

char *appliances_string[APPLIANCE_MAX + 1] = {"empty", "microwave", "kettle",
                                              "oven", "refrigerator", "coffee_machine"};

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
void write_appliance_data_to_file(user_data[], int);
int calc_time(struct tm *, int, int);

int main()
{
    int days_simulated;

    printf("Power Plug Simulator\n\nEnter amount of days you want to simulate data for (MIN: 1, MAX: 100): ");
    scanf(" %d", &days_simulated);
    if (days_simulated > 100)
        days_simulated = 100;
    if (days_simulated < 1)
        days_simulated = 1;

    user_data user[days_simulated];
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
        0.3698,
        1.4793,
        /* Refigerator */
        0.5023,
        0.5552,
        /* Coffee machine */
        0.1578,
        0.6311};

    int i;
    for (i = 0; i < days_simulated; i++)
    {
        user[i] = initialize_user_data(user[i], min_max_appliances);
        /* print_data(user[i]); */
    }

    write_appliance_data_to_file(user, days_simulated);

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
void write_appliance_data_to_file(user_data user[], int days_simulated)
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
    fprintf(file, " \"days_simulated\": \"%d\",", days_simulated);
    fprintf(file, "\n \"date\": [");

    /* For dates */
    for (i = 0; i < days_simulated /* DATES */; i++)
    {
        fprintf(file, "\n   \"%d\",\n    {\n", calc_time(current_time, i + 1, days_simulated)); /* Date */

        /* For all appliances */
        for (x = 0; x < APPLIANCE_MAX; x++)
        {

            fprintf(file, "     \"%s\": {\n", appliances_string[x + 1]);                                       /* Writes appliance name */
            fprintf(file, "         \"appliance_id\": %d,\n", user[i].plug[x].id);                             /* Writes appliance ID */
            fprintf(file, "         \"power_consumption\": %.4f\n        }", user[i].plug[x].power_consumption); /* Writes appliance power consumption */

            /* If we are on the last element: don't end with comma as it is not the correct syntax for .json */
            if (x != APPLIANCE_MAX - 1)
                fprintf(file, ",");
            fprintf(file, "\n");
        }
        fprintf(file, "    }");
        if (i != days_simulated - 1 /* DATES */)
            fprintf(file, ",");
    }
    fprintf(file, "\n ]\n}"); /* Close curly bracket */

    fclose(file);
}

int calc_time(struct tm *current_time, int index, int days_simulated)
{
    time_t new_time = mktime(current_time) - ((days_simulated - index) * (60 * 60 * 24));
    /* printf("\ntime: %d\n", new_time); */

    return new_time;
}
