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
double total_consumption (appliance plug[]);
void bar_chart(double *power, double *ref);
void charts (appliance plug []);

int main(void)
{
    srand(time(NULL));
    user_profile user_profile;
    appliance plug1;

    plug1 = add_plug(plug1);

    printf("\nPower usage of %s is %.2lf kWh.\n", appliances_string[plug1.appliances], plug1.power_consumption);

    charts(plug);

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
    plug.power_consumption = (rand() % 1700) + (float)rand() / RAND_MAX;

    return plug;
}



/* general_power_consumption skal være den gennemsnitlige strømmængde brugt af danske køkkner*/

void charts (appliance plug[]){
    double general_power_consumption = 150; //kWh
    double your_total_consumption = total_consumption(plug);
    printf("Your total power consumtion %f:\n", your_total_consumption);

    printf("The general powerusaged in Denmark is %f:\n", general_power_consumption);

    if(your_total_consumption > general_power_consumption){
        printf("You use %f procent more power then the general public\n", 
        (your_total_consumption / general_power_consumption) * 100);
    }
    else {
        printf("You use %f procent less power then the general public\n",
        (general_power_consumption / your_total_consumption) * 100);
    }

    bar_chart(&your_total_consumption, &general_power_consumption);



}

double total_consumption (appliance plug[]){
    double tot_con = 0;
    for (int i = 0; i < APPLIANCE_MAX; i++){
        tot_con += plug[i].power_consumption;
    }

    return tot_con;
}

void bar_chart (double *power, double *ref){
    int start = 0, start2 = 0;

    printf("Your usagde:      General power usagde:\n");

    for(double i = 0; i < (*power + *ref)/10; i++ ){

        if(*power >= *ref){
            if (start > 0){
                printf("|   |");
            }
            if (start2 > 0){
                printf("            |   |");   
            }
            if (start == 0){
                printf("_____");
                start++;
            }
            if(i >= (*power / 10) - (*ref / 10) && start2 == 0 ){
                printf("            _____");
                start2++;
            }
            printf("\n");
        }
        if(*power < *ref){
            if (start > 0){
                printf("|   |");
            }
            if (start2 > 0){
                printf("         |   |");
            }
            if (i >= (*ref / 10) - (*power / 10) && start == 0){
                printf("_____");
                start++;
            }

            if(start2 == 0 ){
                printf("           _____");
                start2++;
            }
            printf("\n");
        }
    }
}