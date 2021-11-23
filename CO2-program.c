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
    int id;
    double power_consumption;
};
typedef struct appliance appliance;

struct user_profile
{
    unsigned int household_size;
    appliance plug[PLUGS_MAX];
};
typedef struct user_profile user_profile;

struct average_profile
{
    unsigned int household_size;
    appliance appliances[APPLIANCE_MAX];
};
typedef struct average_profile average_profile;

/* prototypes */
void add_plug(appliance *);
void compareFunction(user_profile, user_profile, appliance *);
void compare_plugs(user_profile, average_profile, appliance *, int);
void printTips(appliance[PLUGS_MAX], int);

/* Main program */
int main(void)
{
    appliance above_average_consumption[PLUGS_MAX];
    user_profile user;
    average_profile average;
    int amount_of_plugs;

    add_plug(plug);
    compare_plugs(user, average, above_average_consumption, amount_of_plugs);

    printTips(above_average_consumption, amount_of_plugs);

    return EXIT_SUCCESS;
}

void add_plug(appliance *plug)
{
    printf("Add appliances from the list:\n%d | %s\n%d | %s\n%d | %s\n%d | %s\n%d | %s\n\nSelect appliance: ",
           microwave, appliances_string[microwave],
           kettle, appliances_string[kettle],
           oven, appliances_string[oven],
           refrigerator, appliances_string[refrigerator],
           freezer, appliances_string[freezer]);

    scanf(" %d", &plug[1].id);
}

compare_plug(user_profile user, average_profile average, appliance *above_average_consumption, int amount_of_plugs)
{
    int i;
    int count = 1;
    int temp_appliance;
    for (i = 1; i < amount_of_plugs; i++)
    {
        switch (user.plug[i].id)
        {
        case microwave:
            if (user.plug[i].power_consumption > average.appliances[microwave].power_consumption)
            {
                above_average_consumption[count].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count].id = microwave;
                count++;
                temp_appliance = microwave;
            }
            break;

        case kettle:
            if (user.plug[i].power_consumption > average.appliances[kettle].power_consumption)
            {
                above_average_consumption[count].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count].id = kettle;
                count++;
                temp_appliance = kettle;
            }
            break;

        case oven:
            if (user.plug[i].power_consumption > average.appliances[oven].power_consumption)
            {
                above_average_consumption[count].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count].id = oven;
                count++;
                temp_appliance = oven;
            }
            else

                break;

        case refrigerator:
            if (user.plug[i].power_consumption > average.appliances[refrigerator].power_consumption)
            {
                above_average_consumption[count].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count].id = refrigerator;
                count++;
                temp_appliance = refrigerator;
            }
            break;

        case freezer:
            if (user.plug[i].power_consumption > average.appliances[freezer].power_consumption)
            {
                above_average_consumption[count].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count].id = freezer;
                count++;
                temp_appliance = freezer;
            }
            break;

        default:
            printf("\n\nPool mig på Eddies\n\n");
            break;
        }

        /* Prints percentage of average */
        printf("Consumption of your %s is %.2lf%% of the average.\n", appliances_string[temp_appliance],
               user.plug[i].power_consumption / average.appliances[temp_appliance].power_consumption * 100);
    }
}

/* This function compares whether the users power consumption of all appliances are bigger og smaller than the average.
The instances where the user power consumption is higher than average is sorted into an array of structs called above_average_consumption. Opposite for lower_consumption. */
void compareFunction(user_profile user, user_profile general, appliance *above_average_consumption)
{
    /* appliance lower_consumption[APPLIANCE_MAX];
    int countHigh = 0, countLow = 0;

    int i;
    for (i = 1; i < APPLIANCE_MAX; i++)
    {
        // Creates new array above_average_consumption which contains power consumptions bigger than average
        if (user.appliances[i].power_consumption > general.appliances[i].power_consumption)
        {
            printf("");
            above_average_consumption[countHigh].power_consumption = user.appliances[i].power_consumption;
            above_average_consumption[countHigh].id = user.appliances[i].id;
            countHigh++;
        }
        else
        {
            printf("");
            lower_consumption[countLow].power_consumption = user.appliances[i].power_consumption;
            lower_consumption[countLow].id = user.appliances[i].id;
            countLow++;
        }

        // Prints percentage of average
        printf("Consumption of your %s is %.2lf%% of the average.\n", appliances_string[i],
               user.appliances[i].power_consumption / general.appliances[i].power_consumption * 100);
    } */
}

/* Prints tips on areas, where the users consumption is higher than average */
void printTips(appliance list_of_appliances[PLUGS_MAX], int amount_of_plugs)
{
    char *tips[APPLIANCE_MAX] = {"empty", "dont boil too much water", "and so on..."};
    int i;

    printf("\nYou need to improve on the following appliances: \n");

    for (i = 1; i < amount_of_plugs; i++)
    {
        printf("Tip number %d: %s, is %s \n", i, appliances_string[list_of_appliances->id], tips[list_of_appliances->id]);
    }
}