#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "json-c/json.h"

#define APPLIANCE_MAX 6                     /* Amount of appliances the system knows */
#define PLUGS_MAX 10                        /* Max amount of plugs in the system */
#define KWH_TO_DKK 2.3                      /* Price for 1 kWh in DKK */
#define KWH_TO_CO2_G 122                    /* CO2 g emmission pr 1 kWh production */
#define KWH_TO_CO2_KG (KWH_TO_CO2_G / 1000) /* CO2 kg emmission pr 1 kWh production */

enum appliances
{
    microwave = 1,
    kettle,
    oven,
    refrigerator,
    coffee
};
typedef enum appliances appliances;

char *appliances_string[APPLIANCE_MAX] = {"empty", "Microwave", "Kettle", "Oven", "Refrigerator", "Coffee machine"};

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
void compare_plugs(user_profile, average_profile, appliance *, appliance *, int, int *, int *);
void print_percentage_of_average(int, double, double);
void printTips(appliance[], appliance[], int, int);
void printSwitch(int, appliance[]);
void print_break(void);
void write_appliance_data_to_file(FILE *, user_profile);
double percent(double, double);
double total_consumption(user_profile, int);
void bar_chart(user_profile, double *ref, int);
void charts(user_profile, int);
double convert_power_to_cash(double *);
double convert_power_to_CO2(double *);

/* Main program */
int main(void)
{
    FILE *data_file;

    appliance above_average_consumption[PLUGS_MAX];
    appliance below_average_consumption[PLUGS_MAX];
    int index_above = 0, index_below = 0;
    user_profile user;
    average_profile average;
    int amount_of_plugs;

    int i;
    for (i = 0; i < 5; i++) /* Test. Skal slettes når json virker */
        user.plug[i].power_consumption = 6 + 1 * i;

    /* Test. Skal slettes når json virker */
    average.appliances[microwave].power_consumption = 7;
    average.appliances[kettle].power_consumption = 7;
    average.appliances[oven].power_consumption = 7;
    average.appliances[refrigerator].power_consumption = 7;
    average.appliances[coffee].power_consumption = 7;

    user = initialize_user_profile(user, &amount_of_plugs);
    amount_of_plugs += 1;

    for (int i = 0; i < amount_of_plugs; i++)
        printf("Her er id'et for plug[%d] lig med %d\n", i, user.plug[i].id);
    print_break();

    printf("index is here %d\n", index_above);

    compare_plugs(user, average, above_average_consumption, below_average_consumption, amount_of_plugs, &index_above, &index_below);
    printTips(above_average_consumption, below_average_consumption, index_above, index_below);

    charts(user, amount_of_plugs);

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

/* This function compares the power consumption of plugs[] with the average power consumption of appliances. */
void compare_plugs(user_profile user, average_profile average, appliance *above_average_consumption,
                   appliance *below_average_consumption, int amount_of_plugs, int *index_above, int *index_below)
{
    printf("vi er inde i compare funktionen: \n");
    int i;
    int temp_appliance;

    for (i = 0; i < amount_of_plugs; i++)
    {
        /* average.appliances[] is a sorted array that is constant, while user.plug[] depends on the order in which the use assign plugs to appliances.
        This switch compares the according power consumptions from the two arrays*/
        switch (user.plug[i].id)
        {
        case microwave:
            printf("case: microwave. ");
            if (user.plug[i].power_consumption > average.appliances[microwave].power_consumption)
            {
                above_average_consumption[*index_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[*index_above].id = microwave;
                (*index_above)++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_consumption[*index_below].power_consumption = user.plug[i].power_consumption;
                below_average_consumption[*index_below].id = microwave;
                (*index_below)++;
                printf("inde i else-statement. \n");
            }
            printf("microwave forbrug er %f\n", user.plug[i].power_consumption);
            temp_appliance = microwave;
            break;

        case kettle:
            printf("case: kettle. ");
            if (user.plug[i].power_consumption > average.appliances[kettle].power_consumption)
            {
                above_average_consumption[*index_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[*index_above].id = kettle;
                (*index_above)++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_consumption[*index_below].power_consumption = user.plug[i].power_consumption;
                below_average_consumption[*index_below].id = kettle;
                (*index_below)++;
                printf(" inde i else-statement. \n");
            }
            printf("kettle forbrug er %f\n", user.plug[i].power_consumption);
            temp_appliance = kettle;
            break;

        case oven:
            printf("case: oven. ");
            if (user.plug[i].power_consumption > average.appliances[oven].power_consumption)
            {
                above_average_consumption[*index_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[*index_above].id = oven;
                (*index_above)++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_consumption[*index_below].power_consumption = user.plug[i].power_consumption;
                below_average_consumption[*index_below].id = oven;
                (*index_below)++;
                printf(" inde i else-statement. \n");
            }
            printf("oven forbrug er %f\n", user.plug[i].power_consumption);
            temp_appliance = oven;
            break;

        case refrigerator:
            printf("case: refrigerator. ");
            if (user.plug[i].power_consumption > average.appliances[refrigerator].power_consumption)
            {
                above_average_consumption[*index_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[*index_above].id = refrigerator;
                (*index_above)++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_consumption[*index_below].power_consumption = user.plug[i].power_consumption;
                below_average_consumption[*index_below].id = refrigerator;
                (*index_below)++;
                printf(" inde i else-statement. \n");
            }
            printf("refrigerator forbrug er %f\n", user.plug[i].power_consumption);
            temp_appliance = refrigerator;
            break;

        case coffee:
            printf("case: coffee. ");
            if (user.plug[i].power_consumption > average.appliances[coffee].power_consumption)
            {
                above_average_consumption[*index_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[*index_above].id = coffee;
                (*index_above)++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_consumption[*index_below].power_consumption = user.plug[i].power_consumption;
                below_average_consumption[*index_below].id = coffee;
                (*index_below)++;
                printf(" inde i else-statement. \n");
            }
            printf("coffee forbrug er %f\n", user.plug[i].power_consumption);
            temp_appliance = coffee;
            break;

        default:
            printf("\n\nPool mig på Eddies\n\n");
            break;
        }

        print_percentage_of_average(temp_appliance, user.plug[i].power_consumption, average.appliances[temp_appliance].power_consumption);
    }

    print_break();

    printf("Index above er %d\n", *index_above);

    /* sladeskank */
    for (i = 0; i < *index_above; i++)
        printf("For above average consumption: above-average-consumption[%d].id is %d\n", i, above_average_consumption[i].id);
    print_break();
    for (i = 0; i < *index_below; i++)
        printf("For below average consumption: below-average-consumption[%d].id is %d\n", i, below_average_consumption[i].id);
    print_break();
    print_break();
}

/* Prints percentage of average */
void print_percentage_of_average(int app, double user_cons, double average_cons)
{
    printf("Consumption of your %s is %.2lf%% of the average.\n", appliances_string[app],
           percent(user_cons, average_cons));
    print_break();
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/

/* Prints tips on areas, where the users consumption is higher than average */
void printTips(appliance above_average_consumption[], appliance below_average_consumption[], int index_below, int index_above)
{
    int i, userWantExtra;

    printSwitch(index_above, above_average_consumption);

    print_break();

    printf("Do you wish to get tips on appliances you didn't overuse as well?:\n1 | Yes\n2 | No\n Select option: ");
    scanf("%d", &userWantExtra);
    if (userWantExtra == 1)
    {
        printSwitch(index_below, below_average_consumption);
    }

    print_break();
}

void printSwitch(int amount_of_plugs, appliance consumption[])
{
    int i;
    char tips_microwave[100] = "Only use the microwave for smaller meals.\n";
    char tips_kettle[100] = "Make sure you dont boil more water than needed.\n";
    char tips_oven[100] = "Make use of the ovens pre- and postheat to cook your meals.\nOnly use the oven for bigger meals.\n";
    char tips_refridgerator[100] = "Thaw frozen food in the refridgerator to help it keep.\n";
    char tips_freezer[100] = "Remove frost from the freezers' surfaces.\n";
    char tips_general[100] = "Make sure appliances, pots and more is properly sealed, as to not waste the heat or cold.\n";

    for (i = 0; i < amount_of_plugs; i++)
    {
        switch (consumption[i].id)
        {
        case microwave:
            printf("%s\n", tips_microwave);
            break;

        case kettle:
            printf("%s\n", tips_kettle);
            break;

        case oven:
            printf("%s\n", tips_oven);
            break;

        case refrigerator:
            printf("%s\n", tips_refridgerator);
            break;

        case coffee:
            printf("%s\n", tips_freezer);
            break;

        default:
            printf("Me want corny delight!\n");
        }
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

/* general_power_consumption skal være den gennemsnitlige strømmængde brugt af danske køkkner*/

void charts(user_profile user, int amount_of_plugs)
{
    /* fucking ligegyldig */
    printf("Her er lasses funktion\n\n");

    double general_power_consumption = 150; // kWh
    /* Det totale strøm forbrug for forbrugeren beregnes og printes.

    Den generalle mængde af strømforbrug i danmark printes også*/
    double your_total_consumption = total_consumption(user, amount_of_plugs);
    printf("Your total power consumtion %f:\n", your_total_consumption);

    printf("The general power usage in Denmark is %f:\n", general_power_consumption);

    if (your_total_consumption > general_power_consumption)
    {
        printf("You use %f%% more power then the general public\n",
               (percent(your_total_consumption, general_power_consumption)));
    }
    else
    {
        printf("You use %f%% less power then the general public\n",
               (percent(general_power_consumption, your_total_consumption)));
    }
    /* Værdieren fra de forskellige appliances laves som et bar-chart, og viser hvilke appliances der bruger mest strøm*/
    bar_chart(user, &your_total_consumption, amount_of_plugs);

    printf("\n\nYou have used %f Kr on power.", convert_power_to_cash(&your_total_consumption));

    printf("You have emittet %f kg of CO2.", convert_power_to_CO2(&your_total_consumption));
}

double total_consumption(user_profile user, int amount_of_plugs)
{
    double tot_con = 0;
    for (int i = 0; i < amount_of_plugs; i++)
    {
        tot_con += user.plug[i].power_consumption;
        /* printf("%d\n", i); */
    }

    return tot_con;
}

void bar_chart(user_profile user, double *ref, int amount_of_plugs)
{
    for (int j = 0; j < amount_of_plugs; j++)
    {
        printf("\n\n");
        for (double i = 0; i < (user.plug[j].power_consumption / *ref) * 100; i++)
        {
            printf("|");
        }
    }
}
/* Stor på a, lille på b, hvis du går efter percent af
Og omvent hvis du går efter hvor meget b er større end a */
double percent(double a, double b)
{
    return (a / b) * 100;
}
/* Har en kilde på KWH_TO_DKK konstanten */
double convert_power_to_cash(double *power)
{
    return *power * KWH_TO_DKK;
}

/* Bliver beregnet i kg. Har også en kilde på det.  */
double convert_power_to_CO2(double *power)
{
    return *power * KWH_TO_CO2_KG;
}
