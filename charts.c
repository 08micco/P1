#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* general_power_consumption skal være den gennemsnitlige strømmængde brugt af danske køkkner*/

void charts (appliance plug){
    dobule your_total_consumption = total_consumption(plug);
    printf("Your total power consumtion %f:", your_total_consumption);

    printf("The general powerusaged in Denmark is %f", general_power_consumption);

    if(your_total_consumption > general_power_consumption){
        printf("You use %f procent more power then the general public", 
        (your_total_consumption / general_power_consumption) * 100);
    }
    else {
        printf("You use %f procent less power then the general public",
        (your_total_consumption / general_power_consumption) * 100);
    }

    bar_chart(your_total_consumption);
    bar_chart (general_power_consumption);



}

double total_consumption (appliance plug){
    double tot_con = 0;
    for (int i = 0; i < sizeof(appliance) / sizeof(enum) ; i++){
        tot_con += plug[i].power_consumption;
    }

    return tot_con;
}

void bar_chart (double *power){
    printf("____\n");
    for(double i = 0; i < power; i++ ){
        printf("|   |\n");
    }
}