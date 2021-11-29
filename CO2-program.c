#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "json-c/json.h"

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
    int household_size;
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
user_profile initialize_user_profile(user_profile user, int *);
user_profile add_plug(user_profile, int);
void compareFunction(user_profile, user_profile, appliance *);
void compare_plugs(user_profile, average_profile, appliance *, int);
void printTips(appliance[PLUGS_MAX], int);
void print_break(void);
void write_appliance_data_to_file(FILE *, user_profile);

/* Main program */
int main(void)
{
    FILE *data_file;

    appliance above_average_consumption[PLUGS_MAX];
    user_profile user;
    average_profile average;
    int amount_of_plugs;

    user = initialize_user_profile(user, &amount_of_plugs);
    amount_of_plugs += 1;

    compare_plugs(user, average, above_average_consumption, amount_of_plugs);
    printTips(above_average_consumption, amount_of_plugs);

    write_appliance_data_to_file(data_file, user);

    return EXIT_SUCCESS;
}

/* User profile informations is set here */
user_profile initialize_user_profile(user_profile user, int *plug_index)
{
    printf("What is the size of the household?\n"); /* Scan for household size to the user*/
    printf("Number of peoples: ");
    scanf("%d", &user.household_size);
    print_break();

    int run = 1;
    int scan_input;
    *plug_index = 1;

    while (run && *plug_index < PLUGS_MAX) /* All the kitchen appliances is added to user profile here */
    {
        user = add_plug(user, *plug_index);
        print_break();

        printf("Add one more appliance?:\n1 | Yes\n2 | No\nSelect option: ");
        scanf("%d", &scan_input);
        print_break();
        if (scan_input == 1)
        {
            *plug_index += 1;
            run = 1;
        }
        else
            run = 0;
    }

    printf("\nplugs: %d  size %d.\n", *plug_index, user.household_size); /* Skal slettes senere */
    return user;
}

/* Assigns appliance to plug */
user_profile add_plug(user_profile user, int plug_index)
{
    printf("Add appliances from the list:\n%d | %s\n%d | %s\n%d | %s\n%d | %s\n%d | %s\nSelect appliance: ",
           microwave, appliances_string[microwave],
           kettle, appliances_string[kettle],
           oven, appliances_string[oven],
           refrigerator, appliances_string[refrigerator],
           freezer, appliances_string[freezer]);

    scanf(" %d", &user.plug[plug_index].id); /* Assigns appliance id to the related plug.*/
    return user;
}

void compare_plugs(user_profile user, average_profile average, appliance *above_average_consumption, int amount_of_plugs)
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

/* Prints tips on areas, where the users consumption is higher than average */
void printTips(appliance list_of_appliances[PLUGS_MAX], int amount_of_plugs)
{
    /* Arrays all individual appliance-tips, as well as one for general tips.  */
    char *tips_microwave[APPLIANCE_MAX] = {"Only use the microwave for smaller meals."};
    char *tips_kettle[APPLIANCE_MAX] = {"Make sure you dont boil more water than needed."};
    char *tips_oven[APPLIANCE_MAX] = {"Make use of the ovens pre- and postheat to cook your meals.", "Only use the oven for bigger meals."};
    char *tips_refridgerator[APPLIANCE_MAX] = {"Thaw frozen food in the refridgerator to help it keep.", ""};
    char *tips_freezer[APPLIANCE_MAX] = {"Remove frost from the freezers' surfaces."};
    char *tips_general[APPLIANCE_MAX] = {"Make sure appliances, pots and more is properly sealed, as to not waste the heat or cold."};
    int i, ekstra, amount_of_low_plugs, amount_of_high_plugs; /* low og high er placeholders som erstattes senere. */

    /* Lav et loop der printer tips ud til antallet af plugs der går over gennemsnittet. */

    printf("\nYou have an above-average consumption on these applainces: \n");
    for (i = 1; i < amount_of_high_plugs; i++)
    {
        printf("Appliance nr. 1: %s is above average", list_of_appliances[i]);
    }

    /* A loop that will dispense tips for amount_of_plugs times, for the relevant appliances. */
    for (i = 1; i < amount_of_plugs; i++)
    {
        printf("Tip number %d:  for %s, is %s \n", i, appliances_string[i], tips_microwave[list_of_appliances->id] /* Placeholder */);
    }

    /* Giving the user the option to see tips for appliances that aren't consuming more than average. */
    printf("Do you wish to get tips on appliances you didn't overuse as well?:\n1 | Yes\n2 | No\n Select option: ");
    scanf("%d", ekstra);

    /* If the user wishes to see the ekstra tips, a loop printing the ekstra tips for the amount of plugs that aren't higher than average consumption. */
    if (ekstra == 1)
    {
        for (i = 1; i < amount_of_low_plugs; i++)
        {
            printf("\nTip nr. %d: For %s, is %s", i, appliances_string[i], tips_kettle[list_of_appliances->id] /* Placeholder */);
        }
    }

    // Gør det muligt at brugeren kan spørge efter tips på de plugs som brugeren ikke have et overforbrug af.
}

/* Print a breakline */
void print_break(void)
{
    printf("----------------------------------------------------\n");
}

/* This function writes household size, appliance number and their power consumption to a file */
void write_appliance_data_to_file(FILE *file, user_profile user)
{
    int i;
    file = fopen("data/test.txt", "w+");

    if (file == NULL)
    {
        printf("\nError while creating file\n");
        exit(1);
    }

    fprintf(file, "Household size: %d\n", user.household_size);
    for (i = 0; i <= APPLIANCE_MAX; i++)
    {
        fprintf(file, "Appliance: %d, Power Consumption: %f\n", user.plug[i].id, user.plug[i].power_consumption);
    }

    fclose(file);
}
