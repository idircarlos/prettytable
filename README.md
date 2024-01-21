# prettytable - Beautiful Table Printing in C

#### Note: This project is a work in progress, and there may be areas that need improvement. Contributions are welcome!

## What is prettytable?

**prettytable** is a header C library for creating and printing well-formatted tables in the console. Inspired by the functionality of [Python's prettytable](https://pypi.org/project/prettytable/) library, this project aims to provide a simple and elegant way to represent tabular data in your C applications.

## Key Features of prettytable:

1. **Simple API:** With just a few function calls, you can create, populate, and print tables effortlessly.

2. **Dynamic Sizing:** Tables dynamically adjust column widths based on content, ensuring a clean and visually pleasing layout.


### Next features:

 1. **Customizable:** Adjust the appearance of your table by modifying formatting options such as alignment and colors.

 2. **Different Data Types:** Tables can store all primitive C types, and the library will handle their display appropriately.

## Getting Started with prettytable

To begin using **prettytable** in your C project, follow these steps:

1. Copy the *prettytable.h* file from the src folder and paste it into your project workspace.
2. Include the header file in your source code.
3. Start creating beautifully formatted tables with **prettytable**!

## Example

```c
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

    // Free the table memory
    FreeTable(table);

    return 0;
}
```
**Output**:
```
+---------------+-----------------+---------------+
| Country       | Capital         | Continent     |
+---------------+-----------------+---------------+
| United States | Washington D.C. | North America |
| France        | Paris           | Europe        |
| Australia     | Canberra        | Oceania       |
| Brazil        | Brasilia        | South America |
| Japan         | Tokyo           | Asia          |
+---------------+-----------------+---------------+
```

## Contributing to prettytable
prettytable is an open-source project that welcomes contributions from the community. Whether you're interested in improving documentation, adding new features, or fixing bugs, your efforts are highly appreciated.