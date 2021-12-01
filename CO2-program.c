#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
/* #include "Charts.h" */

#define APPLIANCE_MAX 6                   /* Amount of appliances the system knows */
#define PLUGS_MAX 10                      /* Max amount of plugs in the system */
#define KWH_TO_DKK 2.3                    /* Price for 1 kWh in DKK */
#define KWH_TO_CO2_G 122                  /* CO2 g emmission pr 1 kWh production */
#define KWH_TO_CO2_KG KWH_TO_CO2_G / 1000 /* CO2 kg emmission pr 1 kWh production */

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
user_profile initialize_user_profile(user_profile, int *);
user_profile add_plug(user_profile, int);
void compareFunction(user_profile, user_profile, appliance *);
void compare_plugs(user_profile, average_profile, appliance *, appliance *, int, int *, int *);
int place_in_correct_array(int, double, double, appliance *, appliance *, int **, int **);
void print_percentage_of_average(int, double, double);
void print_tips(appliance[], appliance[], int, int);
void print_switch(appliance[], int);
void print_break(void);
double percent(double, double);
double total_consumption(user_profile, int);
void bar_chart(user_profile, double, int);
void charts(user_profile, int);
double convert_power_to_cash(double);
double convert_power_to_CO2(double);

/* Main program */
int main(void)
{
    appliance above_average_consumption[PLUGS_MAX];
    appliance below_average_consumption[PLUGS_MAX];
    int index_above = 0, index_below = 0;
    user_profile user;
    average_profile average;
    int amount_of_plugs = 0;

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

    compare_plugs(user, average, above_average_consumption, below_average_consumption, amount_of_plugs, &index_above, &index_below);
    print_tips(above_average_consumption, below_average_consumption, index_above, index_below);

    charts(user, amount_of_plugs);

    return EXIT_SUCCESS;
}

/* User profile informations is set here */
user_profile initialize_user_profile(user_profile user, int *plug_index)
{
    int run = 1;
    int scan_input;

    printf("What is the size of the household?\n"); /* Scan for household size to the user*/
    printf("Number of people: ");
    scanf("%d", &user.household_size);
    print_break();

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

    printf("\nplugs: %d  Size of household %d.\n", *plug_index, user.household_size); /* Skal slettes senere */
    print_break();
    return user;
}

/* Assigns appliance to plug */
user_profile add_plug(user_profile user, int plug_index)
{
    int plug_id;

    printf("Add appliances from the list:\n%d | %s\n%d | %s\n%d | %s\n%d | %s\n%d | %s\nSelect appliance: ",
           microwave, appliances_string[microwave],
           kettle, appliances_string[kettle],
           oven, appliances_string[oven],
           refrigerator, appliances_string[refrigerator],
           coffee, appliances_string[coffee]);

    scanf(" %d", &plug_id);                         /* Assigns appliance id to the related plug.*/
    user.plug[plug_index].id = (appliances)plug_id; /* Typecast magic */

    return user;
}

/* This function compares the power consumption of plugs with the average power consumption of appliances. */
void compare_plugs(user_profile user, average_profile average, appliance *above_average_consumption,
                   appliance *below_average_consumption, int amount_of_plugs, int *index_above, int *index_below)
{
    int current_appliance;
    int i;
    for (i = 0; i < amount_of_plugs; i++)
    {
        /* this switch-case is sorted after the id of the users plugs.
        current_appliances is signed the int value of the current case. E.g. in case microwave: current_appliances is assigned microwave */
        switch (user.plug[i].id)
        {
        case microwave:
            current_appliance = place_in_correct_array(microwave, user.plug[i].power_consumption, average.appliances[microwave].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            break;

        case kettle:
            current_appliance = place_in_correct_array(kettle, user.plug[i].power_consumption, average.appliances[kettle].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            break;

        case oven:
            current_appliance = place_in_correct_array(oven, user.plug[i].power_consumption, average.appliances[oven].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            break;

        case refrigerator:
            current_appliance = place_in_correct_array(refrigerator, user.plug[i].power_consumption, average.appliances[refrigerator].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            break;

        case coffee:
            current_appliance = place_in_correct_array(coffee, user.plug[i].power_consumption, average.appliances[coffee].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            break;

        default:
            printf("\n\nPool mig på Eddies\n\n");
        }

        print_percentage_of_average(current_appliance, user.plug[i].power_consumption, average.appliances[current_appliance].power_consumption);
    }

    print_break();
}

/* if the user power consumtion is higher than the average for that appliance,
it is placed into the array called above_average_consumtion. Otherwise places in below_average_consumption */
int place_in_correct_array(int id, double user_con, double average_con, appliance *above_average_consumption,
                           appliance *below_average_consumption, int **index_above, int **index_below)
{
    if (user_con > average_con)
    {
        above_average_consumption[**index_above].power_consumption = user_con;
        above_average_consumption[**index_above].id = id;
        (**index_above)++;
    }
    else
    {
        below_average_consumption[**index_below].power_consumption = user_con;
        below_average_consumption[**index_below].id = id;
        (**index_below)++;
    }

    return id;
}

/* Prints percentage of average consumption.*/
void print_percentage_of_average(int app, double user_cons, double average_cons)
{
    printf("Consumption of your %s is %.2lf%% of the average.\n", appliances_string[app],
           percent(user_cons, average_cons));
}

/* Prints tips on areas, where the users consumption is higher than average */
void print_tips(appliance above_average_consumption[], appliance below_average_consumption[], int index_above, int index_below)
{
    int user_want_extra;

    /* Function for printing out all the instances where the appliances are above average in consumption. */
    print_switch(above_average_consumption, index_above);

    print_break();

    printf("Do you wish to get tips on appliances you didn't overuse as well?:\n1 | Yes\n2 | No\n Select option: ");
    scanf("%d", &user_want_extra);
    print_break();
    if (user_want_extra == 1)
    {
        print_switch(below_average_consumption, index_below);
    }

    print_break();
}

void print_switch(appliance consumption[], int amount)
{
    int i;
    char tips_microwave[500] = "Microwave:\nOnly use the microwave for smaller meals.\n";
    char tips_kettle[500] = "Kettle:\nMake sure you don't boil more water than needed.\n";
    char tips_oven[500] = "Oven:\nMake use of the ovens pre- and postheat to cook your meals.\nOnly use the oven for bigger meals.\n";
    char tips_refridgerator[500] = "Refridgerator:\nThaw frozen food in the refridgerator to help it keep?????.\n";
    char tips_coffee[500] = "Coffee Machine:\nDon't make more coffee than you are going to drink.\nRemember to remove calcium from your machine.\n";
    /* char tips_general[500] = "General Tips:\nMake sure appliances, pots and more is properly sealed, as to not waste the heat or cold.\n"; */

    for (i = 0; i < amount; i++)
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
            printf("%s\n", tips_coffee);
            break;

        default:
            printf("Me want corny delight!\n");
        }
    }
}

/* Print a breakline */
void print_break(void)
{
    printf("------------------------------------------------------------------------------------------------\n");
}

/* general_power_consumption skal være den gennemsnitlige strømmængde brugt af danske køkkner*/
void charts(user_profile user, int amount_of_plugs)
{
    double general_power_consumption = 150; /* kWh
    Det totale strøm forbrug for forbrugeren beregnes og printes. */

    /* Den generalle mængde af strømforbrug i danmark printes også*/
    double your_total_consumption = total_consumption(user, amount_of_plugs);
    printf("Your total power consumtion %.2f:\n", your_total_consumption);

    printf("The general power usage in Denmark is %.2f:\n", general_power_consumption);

    if (your_total_consumption > general_power_consumption)
    {
        printf("You use %.2f%% more power then the general public\n",
               (percent(your_total_consumption, general_power_consumption)));
    }
    else
    {
        printf("You use %.2f%% less power then the general public\n",
               (percent(general_power_consumption, your_total_consumption)));
    }
    /* Værdieren fra de forskellige appliances laves som et bar-chart, og viser hvilke appliances der bruger mest strøm*/
    bar_chart(user, your_total_consumption, amount_of_plugs);

    printf("\n\nYou have used %.2f DKK on power\n", convert_power_to_cash(your_total_consumption));

    printf("You have emittet %.2f kg of CO2.", convert_power_to_CO2(your_total_consumption));
}

double total_consumption(user_profile user, int amount_of_plugs)
{
    double tot_con = 0;

    int i;
    for (i = 0; i < amount_of_plugs; i++)
    {
        tot_con += user.plug[i].power_consumption;
    }

    return tot_con;
}

void bar_chart(user_profile user, double ref, int amount_of_plugs)
{
    int j, i;
    for (j = 0; j < amount_of_plugs; j++)
    {
        printf("\n\n");
        for (i = 0; i < (user.plug[j].power_consumption / ref) * 100; i++)
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
double convert_power_to_cash(double power)
{
    return power * KWH_TO_DKK;
}

/* Bliver beregnet i kg. Har også en kilde på det.  */
double convert_power_to_CO2(double power)
{
    return power * KWH_TO_CO2_KG;
}