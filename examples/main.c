#include "../src/prettytable.h"

int main(int argc, char *argv[]) {
    // Create table
    Table *table = InitTable();

    // Append headers
    AppendHeader(table, "Country");
    AppendHeader(table, "Capital");
    AppendHeader(table, "Continent");

    // Create rows
    char *row1[3] = {"United States", "Washington D.C.", "North America"};
    char *row2[3] = {"France", "Paris", "Europe"};
    char *row3[3] = {"Australia", "Canberra", "Oceania"};
    char *row4[3] = {"Brazil", "Brasilia", "South America"};
    char *row5[3] = {"Japan", "Tokyo", "Asia"};
    
    // Append rows
    AppendRow(table, row1);
    AppendRow(table, row2);
    AppendRow(table, row3);
    AppendRow(table, row4);
    AppendRow(table, row5);

    // Print the table
    PrintTable(table);

    // Be a good boy and free the table :)
    FreeTable(table);

    return 0;
}