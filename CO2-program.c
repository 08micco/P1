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
    int appliances;
    double power_consumption;
};
typedef struct appliance appliance;

struct user_profile
{
    unsigned int household_size;
};
typedef struct user_profile user_profile;

appliance add_plug(appliance);
double percent(double, double);
double total_consumption (appliance plug);
void bar_chart(appliance plug, double *ref);
void charts (appliance plug );

int main(void)
{
    srand(time(NULL));
    user_profile user_profile;
    appliance plug1;

    plug1 = add_plug(plug1);

    printf("\nPower usage of %s is %.2lf kWh.\n", appliances_string[plug1.appliances], plug1.power_consumption);

    charts(plug1);

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

    plug.appliances = plug_type;
    plug.power_consumption = (rand() % 200) + (float)rand() / RAND_MAX;

    return plug;
}



/* general_power_consumption skal være den gennemsnitlige strømmængde brugt af danske køkkner*/

void charts (appliance plug){
    double general_power_consumption = 150; //kWh
    double your_total_consumption = total_consumption(plug);
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

    bar_chart(plug, &your_total_consumption);



}

double total_consumption (appliance plug){
    double tot_con = 0;
    for (int i = 0; i < APPLIANCE_MAX; i++){
        tot_con += plug.power_consumption;
    }

    return tot_con;
}

void bar_chart (appliance plug, double *ref){
    for (int j = 0; j < APPLIANCE_MAX; j++ ){
        printf("\n");
        for(double i = 0; i < (plug[j].power_consumption / *ref) * 100 ; i++){
            printf("|");
        }
    }
}
/* Stor på a, lille på b, hvis du går efter percent af
Og omvent hvis du går efter hvor meget b er større end a */
double percent (double a, double b){
    return (a / b) * 100;
}