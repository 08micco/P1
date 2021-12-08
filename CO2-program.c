#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "tiny-json/tiny-json.h"
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
void print_percentage_of_average(int, int, double, double);
void print_tips(appliance[], appliance[], int, int);
void print_switch(appliance[], int);
void print_break(void);
double percent(double, double);
double average_consumption(int, appliance[], user_profile);
double total_consumption(int, appliance[]);
void bar_chart(user_profile, double, int);
void charts(user_profile, int, average_profile);
double convert_power_to_cash(double);
double convert_power_to_CO2(double);
user_profile parse_json_data(user_profile user, int *days_simulated, int *time);

/* Main program */
int main(void)
{
    appliance above_average_consumption[PLUGS_MAX];
    appliance below_average_consumption[PLUGS_MAX];
    int index_above = 0, index_below = 0;
    user_profile user;
    user_profile user_data;
    average_profile average;
    int amount_of_plugs = 0;
    int days_simulated;
    int time;

    int i;
    for (i = 0; i < PLUGS_MAX; i++) /* Test. Skal slettes når json virker */
        user.plug[i].power_consumption = 6 + 1 * i;

    /* Test. Skal slettes når json virker */
    average.appliances[microwave].power_consumption = 7;
    average.appliances[kettle].power_consumption = 7;
    average.appliances[oven].power_consumption = 7;
    average.appliances[refrigerator].power_consumption = 7;
    average.appliances[coffee].power_consumption = 7;

    user = initialize_user_profile(user, &amount_of_plugs);
    user_data = parse_json_data(user_data, &days_simulated - 1, &time);
    user = parse_json_data(user, &days_simulated, &time);

    compare_plugs(user, average, above_average_consumption, below_average_consumption, amount_of_plugs, &index_above, &index_below);
    print_tips(above_average_consumption, below_average_consumption, index_above, index_below);

    charts(user, amount_of_plugs, average);

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

    while (run && *plug_index < PLUGS_MAX) /* This continues until the user indicates that there is no more plugs, or the max of plugs limit is reached. */
    {
        user = add_plug(user, *plug_index); /* All the kitchen appliances is added to user profile here */
        print_break();

        printf("Add one more appliance?:\n1 | Yes\n2 | No\nSelect option: ");
        scanf("%d", &scan_input);
        print_break();
        if (scan_input == 1)
        {
            if (*plug_index < PLUGS_MAX - 1) /* Plugs_max is 10, but the functions  starts with 0, therefore plugs_max-1 */
                *plug_index += 1;
            else
            {
                printf("You cannot add more plugs.\n");
                run = 0;
            }
        }
        else
            run = 0;
    }

    print_break();

    *plug_index += 1;                                                                 /* Plug_index starts on 0. To get the right amount of plug further in the program, it is raised by one. */
    printf("\nplugs: %d  Size of household %d.\n", *plug_index, user.household_size); /* Skal slettes senere */
    return user;
}

/* Assigns appliance to plug */
user_profile add_plug(user_profile user, int plug_index) /* Use the plug_index to indicate what spot in the "plug" array we should place a plug at in "user".*/
{
    int plug_id;

    printf("Add appliances from the list to plug %d:\n%d | %s\n%d | %s\n%d | %s\n%d | %s\n%d | %s\nSelect appliance: ",
           plug_index + 1,
           microwave, appliances_string[microwave],
           kettle, appliances_string[kettle],
           oven, appliances_string[oven],
           refrigerator, appliances_string[refrigerator],
           coffee, appliances_string[coffee]);

    scanf(" %d", &plug_id);                         /* Assigns appliance id to the related plug.*/
    user.plug[plug_index].id = (appliances)plug_id; /* Typecast the choosen appliance to the plug id */

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
        }

        print_percentage_of_average(i, current_appliance, user.plug[i].power_consumption, average.appliances[current_appliance].power_consumption);
    }

    print_break();
}

/* if the user power consumtion is higher than the average for that appliance,
it is placed into the array above_average_consumption. Otherwise places in below_average_consumption */
int place_in_correct_array(int id, double user_consumption, double average_consumption, appliance *above_average_consumption,
                           appliance *below_average_consumption, int **index_above, int **index_below)
{
    if (user_consumption > average_consumption)
    {
        above_average_consumption[**index_above].power_consumption = user_consumption;
        above_average_consumption[(**index_above)++].id = id;
    }
    else
    {
        below_average_consumption[**index_below].power_consumption = user_consumption;
        below_average_consumption[(**index_below)++].id = id;
    }

    return id;
}

/* Prints percentage of average consumption.*/
void print_percentage_of_average(int i, int app, double user_cons, double average_cons)
{
    printf("Consumption of your %-14s on plug %2d is %7.4f kWh. This is %6.2lf%% of the average.\n",
           appliances_string[app], i + 1, user_cons, percent(user_cons, average_cons));
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
        print_switch(below_average_consumption, index_below);

    print_break();
}

void print_switch(appliance consumption[], int amount)
{
    int i, micro_yes = 0, kettle_yes = 0, oven_yes = 0, refridgerator_yes = 0, coffee_yes = 0;
    char tips_microwave[500] = "Microwave:\nOnly use the microwave for smaller meals.\n";
    char tips_kettle[500] = "Kettle:\nMake sure you don't boil more water than needed.\n";
    char tips_oven[500] = "Oven:\nMake use of the ovens pre- and postheat to cook your meals.\nOnly use the oven for bigger meals.\n";
    char tips_refridgerator[500] = "Refridgerator:\nThaw frozen food in the refridgerator to help it keep?????.\n";
    char tips_coffee[500] = "Coffee Machine:\nDon't make more coffee than you are going to drink.\nRemember to remove calcium from your machine.\n";
    char tips_general[500] = "General Tips:\nMake sure appliances, pots and more is properly sealed, as to not waste the heat or cold.\n";

    for (i = 0; i < amount; i++)
    {
        switch (consumption[i].id)
        {
        case microwave:
            if (micro_yes != 1)
            {
                printf("%s\n", tips_microwave);
                micro_yes++;
            }
            break;

        case kettle:
            if (kettle_yes != 1)
            {
                printf("%s\n", tips_kettle);
                kettle_yes++;
            }
            break;

        case oven:
            if (oven_yes != 1)
            {
                printf("%s\n", tips_oven);
                oven_yes++;
            }
            break;

        case refrigerator:
            if (refridgerator_yes != 1)
            {
                printf("%s\n", tips_refridgerator);
                refridgerator_yes++;
            }
            break;

        case coffee:
            if (coffee_yes != 1)
            {
                printf("%s\n", tips_coffee);
                coffee_yes++;
            }
            break;

        default:
            printf("Error1\n");
        }
    }
    printf("%s\n", tips_general);
}

/* Print a breakline */
void print_break(void)
{
    printf("------------------------------------------------------------------------------------------------\n");
}

void charts(user_profile user, int amount_of_plugs, average_profile average)
{
    double your_total_consumption, average_power_consumption;

    /* Average power consumption, calculates from the appliances plugged in*/
    average_power_consumption = average_consumption(amount_of_plugs, average.appliances, user);
    printf("The average power consumption in Denmark for your appliances is %.4f kWh\n", average_power_consumption);

    /* Total user consumption calculates from the amount of plug.*/
    your_total_consumption = total_consumption(amount_of_plugs, user.plug);
    printf("Your total power consumption is %.4f kWh\n", your_total_consumption);

    if (your_total_consumption > average_power_consumption)
        printf("You use %.2f%% more power then the average public\n",
               (percent(your_total_consumption - average_power_consumption, average_power_consumption)));
    else
        printf("You use %.2f%% less power then the average public\n",
               (percent(average_power_consumption - your_total_consumption, average_power_consumption)));

    bar_chart(user, your_total_consumption, amount_of_plugs);
    printf("\n\nYou have used %.2f DKK on power\n", convert_power_to_cash(your_total_consumption));
    printf("You have emitted %.2f kg of CO2\n", convert_power_to_CO2(your_total_consumption));
}

/* Function that calculate total consumption. The function is called with different arrays of the type appliance*/
double average_consumption(int amount, appliance array[], user_profile user)
{
    double consumption = 0;
    int i, micro_yes = 0, kettle_yes = 0, oven_yes = 0, refrigerator_yes = 0, coffee_yes = 0;
    for (i = 0; i < amount; i++)

        switch (user.plug[i].id)
        {
        case microwave:
            if (micro_yes != 1)
            {
                consumption += array[microwave].power_consumption;
                micro_yes++;
            }
            break;

        case kettle:
            if (kettle_yes != 1)
            {
                consumption += array[kettle].power_consumption;
                kettle_yes++;
            }
            break;

        case oven:
            if (oven_yes != 1)
            {
                consumption += array[oven].power_consumption;
                oven_yes++;
            }
            break;

        case refrigerator:
            if (refrigerator_yes != 1)
            {
                consumption += array[refrigerator].power_consumption;
                refrigerator_yes++;
            }
            break;

        case coffee:
            if (coffee_yes != 1)
            {
                consumption += array[coffee].power_consumption;
                coffee_yes++;
            }
            break;

        default:
            printf("Error2\n");
        }

    return consumption;
}

double total_consumption(int amount, appliance array[])
{
    double tot_con = 0;
    int i;
    for (i = 0; i < amount; i++)
        tot_con += array[i].power_consumption;

    return tot_con;
}

/* Bar chart over the user consumption*/
void bar_chart(user_profile user, double consumption, int plug_amount)
{
    int i, j;
    for (i = 0; i < plug_amount; i++)
    {
        printf("\n%s \n", appliances_string[(appliances)user.plug[i].id]);

        for (j = 0; j < percent(user.plug[i].power_consumption, consumption); j++)
            printf("|");

        printf(" [%.2f%%]\n", percent(user.plug[i].power_consumption, consumption));
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

user_profile parse_json_data(user_profile user, int *days_simulated, int *time)
{
    /* Read file into a string */
    char *str = 0;
    long length;

    FILE *fp = fopen("data/user_data.json", "r");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        str = malloc(length);
        if (str)
            fread(str, 1, length, fp);
    }
    fclose(fp);

    /* Create json */
    json_t mem[320];
    json_t const *json = json_create(str, mem, sizeof mem / sizeof *mem);
    if (!json)
    {
        puts("Error: Json create.");
    }

    /* Parse days simulated */
    json_t const *days = json_getProperty(json, "days_simulated");
    int const days_sim_value = (int)json_getInteger(days);
    *days_simulated = days_sim_value;

    printf("\n---------------\n\n\nDays simulated: %d \n\n", *days_simulated);

    /* Parse time (date) and data*/

    json_t const *data = json_getProperty(json, "date");
    if (!data || JSON_ARRAY != json_getType(data))
    {
        puts("Error: No array found for DATE");
    }

    int j = 0;

    json_t const *appliance;
    for (appliance = json_getChild(data); j <= *days_simulated; appliance = json_getSibling(appliance))
    {
        int i = 0;
        int const date_time = (int)json_getInteger(appliance);
        *time = date_time;
        time_t date_time_t = *time;
        struct tm *date_time_struct = localtime(&date_time_t);
        char time_string[20];
        strftime(time_string, 20, "%d-%m-%Y", date_time_struct);
        printf("Date: %s:\n", time_string);

        appliance = json_getSibling(appliance);
        if (JSON_OBJ == json_getType(appliance))
        {
            /* Parse microwave data */
            json_t const *microwave = json_getProperty(appliance, "microwave");
            json_t const *microwave_appliance_id = json_getProperty(microwave, "appliance_id");
            int const microwave_appliance_id_value = (int)json_getInteger(microwave_appliance_id);
            user.plug[i].id = microwave_appliance_id_value;

            json_t const *microwave_power_consumption = json_getProperty(microwave, "power_consumption");
            double const microwave_power_consumption_value = (double)json_getReal(microwave_power_consumption);
            user.plug[i].power_consumption = microwave_power_consumption_value;
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
            i++;

            /* Parse kettle data */
            json_t const *kettle = json_getProperty(appliance, "kettle");
            json_t const *kettle_appliance_id = json_getProperty(kettle, "appliance_id");
            int const kettle_appliance_id_value = (int)json_getInteger(kettle_appliance_id);
            user.plug[i].id = kettle_appliance_id_value;

            json_t const *kettle_power_consumption = json_getProperty(kettle, "power_consumption");
            double const kettle_power_consumption_value = (double)json_getReal(kettle_power_consumption);
            user.plug[i].power_consumption = kettle_power_consumption_value;
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
            i++;

            /* Parse oven data */
            json_t const *oven = json_getProperty(appliance, "oven");
            json_t const *oven_appliance_id = json_getProperty(oven, "appliance_id");
            int const oven_appliance_id_value = (int)json_getInteger(oven_appliance_id);
            user.plug[i].id = oven_appliance_id_value;

            json_t const *oven_power_consumption = json_getProperty(oven, "power_consumption");
            double const oven_power_consumption_value = (double)json_getReal(oven_power_consumption);
            user.plug[i].power_consumption = oven_power_consumption_value;
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
            i++;

            /* Parse refrigerator data */
            json_t const *refrigerator = json_getProperty(appliance, "refrigerator");
            json_t const *refrigerator_appliance_id = json_getProperty(refrigerator, "appliance_id");
            int const refrigerator_appliance_id_value = (int)json_getInteger(refrigerator_appliance_id);
            user.plug[i].id = refrigerator_appliance_id_value;

            json_t const *refrigerator_power_consumption = json_getProperty(refrigerator, "power_consumption");
            double const refrigerator_power_consumption_value = (double)json_getReal(refrigerator_power_consumption);
            user.plug[i].power_consumption = refrigerator_power_consumption_value;
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
            i++;

            /* Parse coffee machine data */
            json_t const *coffee_machine = json_getProperty(appliance, "coffee_machine");
            json_t const *coffee_machine_appliance_id = json_getProperty(coffee_machine, "appliance_id");
            int const coffee_machine_appliance_id_value = (int)json_getInteger(coffee_machine_appliance_id);
            user.plug[i].id = coffee_machine_appliance_id_value;

            json_t const *coffee_machine_power_consumption = json_getProperty(coffee_machine, "power_consumption");
            double const coffee_machine_power_consumption_value = (double)json_getReal(coffee_machine_power_consumption);
            user.plug[i].power_consumption = coffee_machine_power_consumption_value;
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
        }
        j++;
    }

    return user;
}