#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define APPLIANCE_MAX 5
#define PLUGS_MAX 10

/* This enum will associate the following words with the numbers 1, 2, 3, 4 and 5, 
   which will make the code much more readable throughout the program. */
enum appliances
{
    microwave = 1,
    kettle,
    oven,
    refrigerator,
    coffee_machine
};
typedef enum appliances appliances;

/* Here, an array-pointer of strings have been made with 6 array-slots, the first being "empty", 
   so the rest can align with the previously initiated enums. */
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

    /* The user is asked for how many days they wish to simulate data over, and the input is restricted so <100 is set to 100, and >1 is set to 1. */
    printf("Power Plug Simulator\nEnter amount of days you want to simulate data for.\n[MIN: 1, MAX: 100]: ");
    scanf(" %d", &days_simulated);
    if (days_simulated > 100)
        days_simulated = 100;
    if (days_simulated < 1)
        days_simulated = 1;

    user_data user[days_simulated];
    srand(time(NULL));

    /* Min and max values for each appliance */
    double min_max_appliances[APPLIANCE_MAX * 2] = {
        /* Microwave */
        0.0743,
        0.2973,
        /* Kettle */
        0.1260,
        0.5040,
        /* Oven */
        0.2466,
        0.9862,
        /* Refrigerator */
        0.5023,
        0.5552,
        /* Coffee machine */
        0.1052,
        0.4208};

    int i;
    for (i = 0; i < days_simulated; i++)
    {
        user[i] = initialize_user_data(user[i], min_max_appliances);
    }

    write_appliance_data_to_file(user, days_simulated);

    printf("Successfully simulated power consumption data for %d days.", days_simulated);
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
    return (rand() / (RAND_MAX / (max - min))) + min;
}

/* This function writes household size, appliance number and their power consumption to a file */
/* Lots of spaces and newlines printed to file to make it more readable */
void write_appliance_data_to_file(user_data user[], int days_simulated)
{
    FILE *file;
    int i, x;

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
    for (i = 0; i < days_simulated; i++)
    {
        fprintf(file, "\n   \"%d\",\n    {\n", calc_time(current_time, i + 1, days_simulated)); /* Date */

        /* For all appliances */
        for (x = 0; x < APPLIANCE_MAX; x++)
        {

            fprintf(file, "     \"%s\": {\n", appliances_string[x + 1]);                                         /* Writes appliance name */
            fprintf(file, "         \"appliance_id\": %d,\n", user[i].plug[x].id);                               /* Writes appliance ID */
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

/* Calculates time/date */
int calc_time(struct tm *current_time, int index, int days_simulated)
{
    time_t new_time = mktime(current_time) - ((days_simulated - index) * (60 * 60 * 24));

    return new_time;
}
