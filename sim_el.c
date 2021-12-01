#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define APPLIANCE_MAX 6
#define PLUGS_MAX 10

enum appliances
{
    microwave = 1,
    kettle,
    oven,
    refrigerator,
    freezer
};
typedef enum appliances appliances;

char *appliances_string[APPLIANCE_MAX] = {"empty", "Microwave", "Kettle",
                                          "Oven", "Refrigerator", "Freezer"};

struct appliance
{
    appliances id;
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

void write_appliance_data_to_file(FILE *, user_data);

double random_num(double, double);

int main()
{

    FILE *data_file;

    user_data user;

    double min_microwave = 0.1115;
    double max_microwave = 0.4460;
    double min_kettle = 0.1890;
    double max_kettle = 0.7560;
    double min_oven = 0.4438;
    double max_oven = 1.4793;
    double min_refrigerator = 0.5023;
    double max_refrigerator = 0.5552;
    double min_coffeemachine = 0.1578;
    double max_coffeemachine = 0.6311;
    int count = 1;

    srand(time(NULL));

    for (int i = 0; i < count; i++)
    {
        printf(" Microwave consumption: %f kWh\n Kettle consumption: %f kWh\n Oven consumption: %f kWh\n Refrigerator consumption: %f kWh\n Coffee machine consumption: %f kWh", random_num(max_microwave, min_microwave), random_num(max_kettle, min_kettle), random_num(max_oven, min_oven), random_num(max_refrigerator, min_refrigerator), random_num(max_coffeemachine, min_coffeemachine));
    }

    return 0;
}

double random_num(double max, double min)
{
    return (rand() / (RAND_MAX / (max - min))) + min;
}

/* This function writes household size, appliance number and their power consumption to a file */
void write_appliance_data_to_file(FILE *file, user_data user)
{
    int i;
    file = fopen("data/test.txt", "w+");

    if (file == NULL)
    {
        printf("\nError while creating file\n");
        exit(1);
    }

    for (i = 0; i <= APPLIANCE_MAX; i++)
    {
        fprintf(file, "Appliance: %d, Power Consumption: %f\n", user.plug[i].id, user.plug[i].power_consumption);
    }

    fclose(file);
}
