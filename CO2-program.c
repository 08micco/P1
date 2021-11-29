#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "json-c/json.h"

#define APPLIANCE_MAX 6 /* Amount of appliances the system knows */
#define PLUGS_MAX 10 /* Max amount of plugs in the system */
#define KWH_TO_DKK 2.3 /* Price for 1 kWh in DKK */
#define KWH_TO_CO2_G 122 /* CO2 g emmission pr 1 kWh production */
#define KWH_TO_CO2_KG (KWH_TO_CO2_G/1000) /* CO2 kg emmission pr 1 kWh production */

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
void compare_plugs(user_profile, average_profile, appliance *, appliance *, int);
void printTips(appliance[PLUGS_MAX], int);
void print_break(void);
void write_appliance_data_to_file(FILE *, user_profile);
double percent(double, double);
double total_consumption (user_profile, int);
void bar_chart(user_profile, double *ref, int);
void charts (user_profile, int );
double convert_power_to_cash (double *);
double convert_power_to_CO2(double *);

/* Main program */
int main(void)
{
    FILE *data_file;

    appliance above_average_consumption[PLUGS_MAX];
    appliance below_average_comsumption[PLUGS_MAX];
    user_profile user;
    average_profile average;
    int amount_of_plugs;

    int i;
    for (i = 0; i < 5; i++)
    {
        user.plug[i].power_consumption = 100 + 10 * i;
        average.appliances[i].power_consumption = 90 + 25 * i;
    }

    user = initialize_user_profile(user, &amount_of_plugs);
    amount_of_plugs += 1;

    for (int i = 0; i < amount_of_plugs; i++)
    {
        printf("Her er id'et for %d lig med %d\n", i, user.plug[i].id);
    }

    compare_plugs(user, average, above_average_consumption, below_average_comsumption, amount_of_plugs);
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
           freezer, appliances_string[freezer]);

    scanf(" %d", &user.plug[plug_index].id); /* Assigns appliance id to the related plug.*/
    return user;
}

void compare_plugs(user_profile user, average_profile average, appliance *above_average_consumption,
                   appliance *below_average_comsumtion, int amount_of_plugs)
{
    printf("vi er inde i compare funktionen: ");
    int i;
    int count_above = 1, count_below = 1;
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
                below_average_comsumtion[count_below].power_consumption = user.plug[i].power_consumption;
                below_average_comsumtion[count_below].id = microwave;
                count_below++;
                printf(" inde i else-statement. \n");
            }
            temp_appliance = microwave;
            break;

        case kettle:
            printf("case: microwave. ");
            if (user.plug[i].power_consumption > average.appliances[kettle].power_consumption)
            {
                above_average_consumption[count_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count_above].id = kettle;
                count_above++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_comsumtion[count_below].power_consumption = user.plug[i].power_consumption;
                below_average_comsumtion[count_below].id = kettle;
                count_below++;
                printf(" inde i else-statement. \n");
            }
            temp_appliance = kettle;
            break;

        case oven:
            printf("case: microwave. ");
            if (user.plug[i].power_consumption > average.appliances[oven].power_consumption)
            {
                above_average_consumption[count_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count_above].id = oven;
                count_above++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_comsumtion[count_below].power_consumption = user.plug[i].power_consumption;
                below_average_comsumtion[count_below].id = oven;
                count_below++;
                printf(" inde i else-statement. \n");
            }
            temp_appliance = oven;
            break;

        case refrigerator:
            printf("case: microwave. ");
            if (user.plug[i].power_consumption > average.appliances[refrigerator].power_consumption)
            {
                above_average_consumption[count_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count_above].id = refrigerator;
                count_above++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_comsumtion[count_below].power_consumption = user.plug[i].power_consumption;
                below_average_comsumtion[count_below].id = refrigerator;
                count_below++;
                printf(" inde i else-statement. \n");
            }
            temp_appliance = refrigerator;
            break;

        case freezer:
            printf("case: microwave. ");
            if (user.plug[i].power_consumption > average.appliances[freezer].power_consumption)
            {
                above_average_consumption[count_above].power_consumption = user.plug[i].power_consumption;
                above_average_consumption[count_above].id = freezer;
                count_above++;
                printf("inde i if-statement. \n");
            }
            else
            {
                below_average_comsumtion[count_below].power_consumption = user.plug[i].power_consumption;
                below_average_comsumtion[count_below].id = freezer;
                count_below++;
                printf(" inde i else-statement. \n");
            }
            temp_appliance = freezer;
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

/* general_power_consumption skal være den gennemsnitlige strømmængde brugt af danske køkkner*/

void charts (user_profile user, int amount_of_plugs){
    double general_power_consumption = 150; //kWh
    /* Det totale strøm forbrug for forbrugeren beregnes og printes. 
    Den generalle mængde af strømforbrug i danmark printes også*/
    double your_total_consumption = total_consumption(user, amount_of_plugs);
    printf("Your total power consumtion %f:\n", your_total_consumption);

    printf("The general powerusaged in Denmark is %f:\n", general_power_consumption);

    if(your_total_consumption > general_power_consumption){
        printf("You use %f%% more power then the general public\n", 
        (percent(your_total_consumption, general_power_consumption)));
    }
    else {
        printf("You use %f%% less power then the general public\n",
        (percent(general_power_consumption, your_total_consumption)));
    }
    /* Værdieren fra de forskellige appliances laves som et bar-chart, og viser hvilke appliances der bruger mest strøm*/
    bar_chart(user, &your_total_consumption, amount_of_plugs);

    printf("You have used %f Kr on power.",convert_power_to_cash(&your_total_consumption));

    printf("You have emittet %f kg of CO2.",convert_power_to_CO2(&your_total_consumption));



}

double total_consumption (user_profile user, int amount_of_plugs){
    double tot_con = 0;
    for (int i = 0; i < amount_of_plugs; i++){
        tot_con += user.plug[i].power_consumption;
        printf("%d\n", i);
    }

    return tot_con;
}

void bar_chart (user_profile user, double *ref, int amount_of_plugs){
    for (int j = 0; j < amount_of_plugs; j++ ){
        printf("\n\n");
        for(double i = 0; i < (user.plug[j].power_consumption / *ref) * 100 ; i++){
            printf("|");
        }
    }
}
/* Stor på a, lille på b, hvis du går efter percent af
Og omvent hvis du går efter hvor meget b er større end a */
double percent (double a, double b){
    return (a / b) * 100;
}
/* Har en kilde på KWH_TO_DKK konstanten */
double convert_power_to_cash (double *power){
    return *power * KWH_TO_DKK;
}

/* Bliver beregnet i kg. Har også en kilde på det.  */
double convert_power_to_CO2(double *power){
    return *power * KWH_TO_CO2_KG;
}
