#ifndef STRUCTS_H_
#define STRUCTS_H_

#define APPLIANCE_MAX 6                   /* Amount of appliances the system knows */
#define PLUGS_MAX 10                      /* Max amount of plugs in the system */
#define KWH_TO_DKK 2.3                    /* Price for 1 kWh in DKK */
#define KWH_TO_CO2_G 122                  /* CO2 g emmission pr 1 kWh production */
#define KWH_TO_CO2_KG KWH_TO_CO2_G / 1000 /* CO2 kg emmission pr 1 kWh production */
#define PRINT_LINE_SIZE 96                /* Length of line to seperate text (must be even when divided by 2)*/


enum appliances
{
    microwave = 1,
    kettle,
    oven,
    refrigerator,
    coffee
};
typedef enum appliances appliances;

char *appliances_string[APPLIANCE_MAX] = {"empty", "Microwave", "Kettle", "Oven", "Refrigerator", "Coffee Machine"};
char *appliances_string_lwr[APPLIANCE_MAX] = {"empty", "microwave", "kettle", "oven", "refrigerator", "coffee machine"};

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


#endif