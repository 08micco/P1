#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define APPLIANCE_MAX 6

enum appliances
{
    microwave = 1,
    kettle,
    oven,
    refrigerator,
    freezer
};
typedef enum appliances appliances;

char *appliances_string[APPLIANCE_MAX] = {"empty", "Microwave", "Kettle", "Oven", "Refrigerator", "Freezer"};

struct appliance
{
    appliances appliance;
    double power_consumption;
};
typedef struct appliance appliance;

struct user_profile
{
    unsigned int household_size;
};
typedef struct user_profile user_profile;

appliance add_plug(appliance);
appliance get_data(double, int);

int main(void)
{
    srand(time(NULL));
    user_profile user_profile;
    appliance plug1;

    plug1 = add_plug(plug1);

    printf("\nPower usage of %s is %.2lf kWh.\n", appliances_string[plug1.appliance], plug1.power_consumption);

    return EXIT_SUCCESS;
}

appliance add_plug(appliance plug)
{
    int plug_type;
    printf("\n%d | %s\n%d | %s\n%d | %s\n%d | %s\n%d | %s\n\nSelect operation: ",
           microwave, appliances_string[microwave],
           kettle, appliances_string[kettle],
           oven, appliances_string[oven],
           refrigerator, appliances_string[refrigerator],
           freezer, appliances_string[freezer]);
    scanf(" %d", &plug_type);

    plug.appliance = plug_type;
    plug.power_consumption = (rand() % 1700) + (float)rand() / RAND_MAX;

    return plug;
}

appliance get_data(double power_in_kwh, int appliance_name)
{
    struct appliance temp_appliance;
    temp_appliance.appliance = appliance_name;
    temp_appliance.power_consumption = power_in_kwh;
    return temp_appliance;
}
