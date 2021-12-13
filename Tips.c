#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "Prototyper.h"

/* Prints tips on areas, where the users consumption is higher than average */
void print_tips(appliance above_average_consumption[], appliance below_average_consumption[], int index_above, int index_below)
{
    int user_want_extra;

    /* Function for printing out all the instances where the appliances are above average in consumption. */
    print_switch(above_average_consumption, index_above);

    print_break();

    printf("Do you wish to get tips on appliances you didn't overuse as well?:\n1 | Yes\n2 | No\nSelect option: ");
    scanf("%d", &user_want_extra);
    print_break();

    if (user_want_extra == 1)
        print_switch(below_average_consumption, index_below);
}

void print_switch(appliance consumption[], int amount)
{
    int i, micro_yes = 0, kettle_yes = 0, oven_yes = 0, refridgerator_yes = 0, coffee_yes = 0;
    char tips_microwave[500] = "Microwave:\nOnly use the microwave for smaller meals.\n";
    char tips_kettle[500] = "Kettle:\nMake sure you don't boil more water than needed.\n";
    char tips_oven[500] = "Oven:\nMake use of the ovens pre- and postheat to cook your meals.\nOnly use the oven for bigger meals.\n";
    char tips_refrigerator[500] = "Refrigerator:\nThaw frozen food in the refridgerator to help it keep a cold temperature.\n";
    char tips_coffee[500] = "Coffee Machine:\nDon't make more coffee than you are going to drink.\nRemember to remove calcium from your machine.\n";
    char tips_general[500] = "General Tips:\nMake sure appliances, pots and more is properly sealed, as to not waste the heat or cold.\n";
    /* xd */
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
                printf("%s\n", tips_refrigerator);
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
            printf("Error (Print Switch): ID not found (ID: %d).\n", consumption[i].id);
        }
    }
    printf("%s\n", tips_general);
}