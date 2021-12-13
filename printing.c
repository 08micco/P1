#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include"Prototyper.h"

/* Print a breakline */
void print_break(void)
{
    int i;

    for (i = 0; i < PRINT_LINE_SIZE; i++)
        printf("-");
    printf("\n");
}

void print_title(char title[])
{
    int title_length = strlen(title);
    int line1_length = (double)ceil((PRINT_LINE_SIZE - title_length - 2) / 2);  /* Calculates the line size for the left side of title */
    int line2_length = (double)floor((PRINT_LINE_SIZE - title_length - 2) / 2); /* Calculates the line size for the right side of title */
    int i;

    printf("\n");
    for (i = 0; i < line1_length; i++)
        printf("=");
    printf(" %s ", title);
    /*     if (line_length % 2 != 0)
        line_length = line_length + 1; */
    for (i = 0; i < line2_length; i++)
        printf("=");
    printf("\n\n");
}

void print_section(char section[])
{
    printf("[ %s ]\n", section);
}