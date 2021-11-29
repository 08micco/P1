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
    coffee
};
typedef enum appliances appliances;

char *appliances_string[APPLIANCE_MAX] = {"empty", "Microwave", "Kettle", "Oven", "Refrigerator", "coffee machine"};

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
void compare_plugs(user_profile, average_profile, appliance *, appliance *, int);
void printTips(appliance[PLUGS_MAX], int);
void print_break(void);
void write_appliance_data_to_file(FILE *, user_profile);

/* Main program */
int main(void)
{
    FILE *data_file;

    appliance above_average_consumption[PLUGS_MAX];
    appliance below_average_consumption[PLUGS_MAX];
    user_profile user;
    average_profile average;
    int amount_of_plugs;

    int i;
    for (i = 0; i < 5; i++)
    {
        user.plug[i].power_consumption = 6 + 1 * i;
    }

    average.appliances[microwave].power_consumption = 7;
    average.appliances[kettle].power_consumption = 7;
    average.appliances[oven].power_consumption = 7;
    average.appliances[refrigerator].power_consumption = 7;
    average.appliances[coffee].power_consumption = 7;

    user = initialize_user_profile(user, &amount_of_plugs);
    amount_of_plugs += 1;

    for (int i = 0; i < amount_of_plugs; i++)
    {
        printf("Her er id'et for plug[%d] lig med %d\n", i, user.plug[i].id);
    }
    print_break();

    compare_plugs(user, average, above_average_consumption, below_average_consumption, amount_of_plugs);
    printTips(above_average_consumption, amount_of_plugs);

    write_appliance_data_to_file(data_file, user);

    return EXIT_SUCCESS;
}

/* User profile informations is set here */
user_profile initialize_user_profile(user_profile user, int *plug_index)
{
    printf("What is the size of the household?\n"); /* Scan for household size to the user*/
    printf("Number of people: ");
    scanf("%d", &user.household_size);
    print_break();

    int run = 1;
    int scan_input;
    *plug_index = 0;

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

    printf("\nplugs: %d  Householdsize %d.\n", *plug_index, user.household_size); /* Skal slettes senere */
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
           coffee, appliances_string[coffee]);

    scanf(" %d", &user.plug[plug_index].id); /* Assigns appliance id to the related plug.*/
    return user;
}

void compare_plugs(user_profile user, average_profile average, appliance *above_average_consumption,
                   appliance *below_average_consumption, int amount_of_plugs)
{
    printf("vi er inde i compare funktionen: \n");
    int i;
    int count_above = 0, count_below = 0;
    int temp_appliance;
    for (i = 0; i < amount_of_plugs; i++)
    {
        switch (user.plug[i].id)
        {
        case microwave:
            printf("case: microwave. ");
            if (user.plug[i].power_consumption > average.appliances[microwave].power_consumption)
            {
                above_average_consumption[count_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count_above].id = microwave;
                count_above++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_consumption[count_below].power_consumption = user.plug[i].power_consumption;
                below_average_consumption[count_below].id = microwave;
                count_below++;
                printf("inde i else-statement. \n");
            }
            printf("microwave forbrug er %f\n", user.plug[i].power_consumption);
            temp_appliance = microwave;
            break;

        case kettle:
            printf("case: kettle. ");
            if (user.plug[i].power_consumption > average.appliances[kettle].power_consumption)
            {
                above_average_consumption[count_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count_above].id = kettle;
                count_above++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_consumption[count_below].power_consumption = user.plug[i].power_consumption;
                below_average_consumption[count_below].id = kettle;
                count_below++;
                printf(" inde i else-statement. \n");
            }
            printf("kettle forbrug er %f\n", user.plug[i].power_consumption);
            temp_appliance = kettle;
            break;

        case oven:
            printf("case: oven. ");
            if (user.plug[i].power_consumption > average.appliances[oven].power_consumption)
            {
                above_average_consumption[count_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count_above].id = oven;
                count_above++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_consumption[count_below].power_consumption = user.plug[i].power_consumption;
                below_average_consumption[count_below].id = oven;
                count_below++;
                printf(" inde i else-statement. \n");
            }
            printf("oven forbrug er %f\n", user.plug[i].power_consumption);
            temp_appliance = oven;
            break;

        case refrigerator:
            printf("case: refrigerator. ");
            if (user.plug[i].power_consumption > average.appliances[refrigerator].power_consumption)
            {
                above_average_consumption[count_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count_above].id = refrigerator;
                count_above++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_consumption[count_below].power_consumption = user.plug[i].power_consumption;
                below_average_consumption[count_below].id = refrigerator;
                count_below++;
                printf(" inde i else-statement. \n");
            }
            printf("refrigerator forbrug er %f\n", user.plug[i].power_consumption);
            temp_appliance = refrigerator;
            break;

        case coffee:
            printf("case: coffee. ");
            if (user.plug[i].power_consumption > average.appliances[coffee].power_consumption)
            {
                above_average_consumption[count_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count_above].id = coffee;
                count_above++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_consumption[count_below].power_consumption = user.plug[i].power_consumption;
                below_average_consumption[count_below].id = coffee;
                count_below++;
                printf(" inde i else-statement. \n");
            }
            printf("coffee forbrug er %f\n", user.plug[i].power_consumption);
            temp_appliance = coffee;
            break;

        default:
            printf("\n\nPool mig på Eddies\n\n");
            break;
        }

        /* Prints percentage of average */
        printf("Consumption of your %s is %.2lf%% of the average.\n", appliances_string[temp_appliance],
               user.plug[i].power_consumption / average.appliances[temp_appliance].power_consumption * 100);
        print_break();
    }

    print_break();

    /* sladeskank */
    for (i = 0; i < count_above; i++)
        printf("For above average consumption: above-average-consumption[%d].id is %d\n", i, above_average_consumption[i].id);
    print_break();
    for (i = 0; i < count_below; i++)
        printf("For below average consumption: below-average-consumption[%d].id is %d\n", i, below_average_consumption[i].id);
    print_break();
    print_break();
}

/* Prints tips on areas, where the users consumption is higher than average */
void printTips(appliance list_of_appliances[PLUGS_MAX], int amount_of_plugs)
{
    char *tips[APPLIANCE_MAX] = {"empty", "dont boil too much water", "and so on..."};

    printf("\nYou need to improve on the following appliances: \n");

    int i;
    for (i = 1; i < amount_of_plugs; i++)
    {
        printf("Tip number %d: %s, is %s \n", i, appliances_string[list_of_appliances->id], tips[list_of_appliances->id]);
    }
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
