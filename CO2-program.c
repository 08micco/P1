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

char *appliances_string[APPLIANCE_MAX] = {"empty", "Microwave", "Kettle", "Oven", "Refrigerator", "Freezer"};

struct appliance
{
    appliances id;
    double power_consumption;
};
typedef struct appliance appliance;

struct user_profile
{
    unsigned int household_size;
    appliance plug[PLUGS_MAX];
};
typedef struct user_profile user_profile;

/* prototypes */
user_profile initialize_user_profile(user_profile);
void add_plug(appliance *);
void compareFunction(user_profile, user_profile, appliance *);
void printTips(appliance *);

/* Main program */
int main(void)
{
    appliance higher_consumption[APPLIANCE_MAX];
    user_profile user;
    user_profile general;

    initialize_user_profile(user);

    compareFunction(user, general, higher_consumption);
    printTips(higher_consumption);

    return EXIT_SUCCESS;
}

void add_plug(user_profile user, int plug_index)
{
    printf("Add appliances from the list:\n%d | %s\n%d | %s\n%d | %s\n%d | %s\n%d | %s\n\nSelect appliance: ",
           microwave, appliances_string[microwave],
           kettle, appliances_string[kettle],
           oven, appliances_string[oven],
           refrigerator, appliances_string[refrigerator],
           freezer, appliances_string[freezer]);

    scanf(" %d", user.plug[plug_index].id);
}

/* This function compares whether the users power consumption of all appliances are bigger og smaller than the average.
The instances where the user power consumption is higher than average is sorted into an array of structs called higher_consumption. Opposite for lowerConsumption. */
void compareFunction(user_profile user, user_profile general, appliance *higher_consumption)
{
    appliance lowerConsumption[APPLIANCE_MAX];
    int countHigh = 0, countLow = 0;

    int i;
    for (i = 1; i < APPLIANCE_MAX; i++)
    {
        /* Creates new array higher_consumption which contains power consumptions bigger than average */
        if (user.appliances[i].power_consumption > general.appliances[i].power_consumption)
        {
            printf("");
            higher_consumption[countHigh].power_consumption = user.appliances[i].power_consumption;
            higher_consumption[countHigh].id = user.appliances[i].id;
            countHigh++;
        }
        else
        {
            printf("");
            lowerConsumption[countLow].power_consumption = user.appliances[i].power_consumption;
            lowerConsumption[countLow].id = user.appliances[i].id;
            countLow++;
        }

        /* Prints percentage of average */
        printf("Consumption of your %s is %.2lf%% of the average.\n", appliances_string[i],
               user.appliances[i].power_consumption / general.appliances[i].power_consumption * 100);
    }
}

void printTips(appliance *higher_consumption)
{
    printf("Din mor er et tip");
}

user_profile initialize_user_profile(user_profile user) /* User profile informations is set here */
{
    printf("What is the size of the household?\n"); /* Scan for household size to user profile*/
    printf("Number of peoples: ");
    scanf("%d", &user.household_size);

    while (run && plug_index < PLUGS_MAX) /* All the kitchen appliances is added to user profile here */
    {
       /* add_plug(user, plug_index);*/
       
    }

    return user;
}