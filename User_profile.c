#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "Prototyper.h"

user_profile initialize_user_profile(user_profile user, int *plug_index)
{
    int run = 1;
    int scan_input;

    printf("What is the size of the household?\nNumber of people: "); /* Scan for household size to the user*/
    scanf(" %d", &user.household_size);
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

    *plug_index += 1;                                                                             /* Plug_index starts on 0. To get the right amount of plug further in the program, it is raised by one. */
    printf("Added %d plug(s) to your household of size %d.\n", *plug_index, user.household_size); /* Skal slettes senere */
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