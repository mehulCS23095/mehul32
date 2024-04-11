#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#define MAX_PRODUCTS 100
#define MAX_TRANSACTIONS 100
#define FILENAME "inventory.txt"
#define TRANSACTION_FILENAME "transactions.txt"

struct Product {
    char name[50];
    char brand[50];
    int id;
    int quantity;
    float price;
};

struct Transaction {
    int productId;
    int quantity;
    float totalAmount;
};

struct Product inventory[MAX_PRODUCTS];
struct Transaction transactions[MAX_TRANSACTIONS];
int totalProducts = 0;
int totalTransactions = 0;
float totalSales = 0.0;

// Function to find the index of a product by ID
int findProductIndexById(int productId) {
    int i;
    for (i = 0; i < totalProducts; i++) {
        if (inventory[i].id == productId) {
            return i;
        }
    }
    return -1;
}

// Function to find the index of a product by brand
int findProductIndexByBrand(const char *brand) {
    int i;
    for (i = 0; i < totalProducts; i++) {
        if (strcasecmp(inventory[i].brand, brand) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to save product data to a file
void saveDataToFile() {
    int i;
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%d\n", totalProducts);
    for (i = 0; i < totalProducts; i++) {
        fprintf(file, "%s %s %d %d %.2f\n", inventory[i].name, inventory[i].brand, inventory[i].id,
                inventory[i].quantity, inventory[i].price);
    }

    fprintf(file, "%.2f\n", totalSales);

    fclose(file);
}

// Function to save transaction data to a file
void saveTransactionToFile() {
    int i;
    FILE *file = fopen(TRANSACTION_FILENAME, "w");
    if (file == NULL) {
        printf("Error opening transaction file for writing.\n");
        return;
    }

    for (i = 0; i < totalTransactions; i++) {
        fprintf(file, "%d %d %.2f\n", transactions[i].productId, transactions[i].quantity, transactions[i].totalAmount);
    }

    fclose(file);
}

// Function to load data from files
void loadDataFromFile() {
    int i;
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No previous data found.\n");
        return;
    }

    fscanf(file, "%d", &totalProducts);
    for (i = 0; i < totalProducts; i++) {
        fscanf(file, "%s %s %d %d %f", inventory[i].name, inventory[i].brand, &inventory[i].id,
               &inventory[i].quantity, &inventory[i].price);
    }

    fscanf(file, "%f", &totalSales);  // Load total sales

    fclose(file);

    file = fopen(TRANSACTION_FILENAME, "r");
    if (file == NULL) {
        printf("No previous transaction data found.\n");
        return;
    }

    fscanf(file, "%d", &totalTransactions);
    for (i = 0; i < totalTransactions; i++) {
        fscanf(file, "%d %d %f", &transactions[i].productId, &transactions[i].quantity, &transactions[i].totalAmount);
    }

    fclose(file);
}

// Function to display transaction history
void displayTransactionHistory() {
    int i;
    printf("\n============================================\n");
    printf("           Grocery Store Management\n");
    printf("============================================\n");

    printf("All Transactions:\n");
    printf("-----------------------------------------------------------------------------\n");
    printf("| %-4s | %-20s | %-10s | %-6s |\n", "ID", "Product Name", "Quantity", "Total Amount");
    printf("-----------------------------------------------------------------------------\n");

    for (i = 0; i < totalTransactions; i++) {
        int productId = transactions[i].productId;
        int quantity = transactions[i].quantity;
        float totalAmount = transactions[i].totalAmount;

        int productIndex = findProductIndexById(productId);
        if (productIndex != -1) {
            printf("| %-4d | %-20s | %-10d | Rs.%-6.2f |\n", productId, inventory[productIndex].name, quantity, totalAmount);
        }
    }
}

// Function to add a new product to the inventory with ID uniqueness check
void addProduct() {
    int i, newProductId;
    if (totalProducts < MAX_PRODUCTS) {
        printf("Enter product name: ");
        scanf("%s", inventory[totalProducts].name);

        printf("Enter product brand: ");
        scanf("%s", inventory[totalProducts].brand);

        // Validate product ID uniqueness
        do {
            printf("Enter product ID: ");
            scanf("%d", &newProductId);

            for (i = 0; i < totalProducts; i++) {
                if (inventory[i].id == newProductId) {
                    printf("Product ID already exists. Please enter a unique ID.\n");
                    continue;
                }
            }
            break; // Exit the loop if the ID is unique
        } while (1);

        inventory[totalProducts].id = newProductId;

        printf("Enter quantity: ");
        scanf("%d", &inventory[totalProducts].quantity);

        printf("Enter price: ");
        scanf("%f", &inventory[totalProducts].price);

        totalProducts++;
        printf("Product added successfully!\n");
    } else {
        printf("Inventory is full. Cannot add more products.\n");
    }
}

// Function to display the current inventory with improved formatting
void displayInventory() {
    int i;
    printf("\n============================================\n");
    printf("           Grocery Store Management\n");
    printf("============================================\n");

    printf("Inventory:\n");
    printf("--------------------------------------------------------------------------\n");
    printf("| %-4s | %-20s | %-15s | %-8s | %-6s |\n", "ID", "Name", "Brand", "Quantity", "Price");
    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < totalProducts; i++) {
        printf("| %-4d | %-20s | %-15s | %-8d | Rs.%.2f |\n", inventory[i].id, inventory[i].name,
               inventory[i].brand, inventory[i].quantity, inventory[i].price);
    }
}

// Function to search for a product by ID, Name, or Brand
void searchProduct() {
    int choice;
    printf("\n============================================\n");
    printf("           Grocery Store Management\n");
    printf("============================================\n");

    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    printf("3. Search by Brand\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            searchProductByID();
            break;
        case 2:
            searchProductByName();
            break;
        case 3:
            searchProductByBrand();
            break;
        default:
            printf("Invalid choice.\n");
    }
}

// Function to search for a product by ID
void searchProductByID() {
    int productId;
    printf("Enter product ID to search: ");
    scanf("%d", &productId);

    int productIndex = findProductIndexById(productId);
    if (productIndex != -1) {
        displayProductDetails(productIndex);
    } else {
        printf("Product not found in the inventory.\n");
    }
}

// Function to search for a product by Name (case-insensitive)
void searchProductByName() {
    char searchName[50];
    int i, j, matchFound;
    printf("Enter product name to search: ");
    scanf("%s", searchName);

    for (i = 0; i < totalProducts; i++) {
        matchFound = 1; // Assume match until proven otherwise

        // Compare each character of searchName with the product name (case-insensitive)
        for (j = 0; searchName[j] != '\0' && inventory[i].name[j] != '\0'; j++) {
            if (tolower(searchName[j]) != tolower(inventory[i].name[j])) {
                matchFound = 0; // Characters do not match
                break;
            }
        }

        // If a match is found, display product details and return
        if (matchFound) {
            displayProductDetails(i);
            return;
        }
    }

    printf("Product not found in the inventory.\n");
}

// Function to search for a product by Brand (case-insensitive)
void searchProductByBrand() {
    char searchBrand[50];
    int i, j, matchFound;
    printf("Enter product brand to search: ");
    scanf("%s", searchBrand);

    for (i = 0; i < totalProducts; i++) {
        matchFound = 1; // Assume match until proven otherwise

        // Compare each character of searchBrand with the product brand (case-insensitive)
        for (j = 0; searchBrand[j] != '\0' && inventory[i].brand[j] != '\0'; j++) {
            if (tolower(searchBrand[j]) != tolower(inventory[i].brand[j])) {
                matchFound = 0; // Characters do not match
                break;
            }
        }

        // If a match is found, display product details and return
        if (matchFound) {
            displayProductDetails(i);
            return;
        }
    }

    printf("Product not found in the inventory.\n");
}

// Function to display details of a specific product
void displayProductDetails(int index) {
    printf("\n============================================\n");
    printf("           Grocery Store Management\n");
    printf("============================================\n");

    printf("Product found:\n");
    printf("ID:%d\n", inventory[index].id);
    printf("Name:%s\n", inventory[index].name);
    printf("Brand:%s\n", inventory[index].brand);
    printf("Quantity:%d\n", inventory[index].quantity);
    printf("Price: Rs.%.2f\n", inventory[index].price);
}

// Function to process a sale
void processSale() {
    int productId, quantity, productIndex;
    float totalAmount;

    printf("Enter product ID: ");
    scanf("%d", &productId);

    productIndex = findProductIndexById(productId);

    if (productIndex != -1) {
        printf("Enter quantity: ");
        scanf("%d", &quantity);

        if (quantity > 0 && quantity <= inventory[productIndex].quantity) {
            totalAmount = quantity * inventory[productIndex].price;
            transactions[totalTransactions].productId = productId;
            transactions[totalTransactions].quantity = quantity;
            transactions[totalTransactions].totalAmount = totalAmount;
            totalTransactions++;

            // Update inventory
            inventory[productIndex].quantity -= quantity;
            totalSales += totalAmount;

            printf("Sale processed successfully!\n");
        } else {
            printf("Invalid quantity. Sale not processed.\n");
        }
    } else {
        printf("Product not found. Sale not processed.\n");
    }
}

// Function to start the main program
void startProgram() {
    loadDataFromFile();

    int choice;

    do {
        printf("\n============================================\n");
        printf("           Grocery Store Management\n");
        printf("============================================\n");

        printf("1. Add Product\n");
        printf("2. Display Inventory\n");
        printf("3. Search Product\n");
        printf("4. Process Sale\n");
        printf("5. Display Transaction History\n");
        printf("6. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                displayInventory();
                break;
            case 3:
                searchProduct();
                break;
            case 4:
                processSale();
                break;
            case 5:
                displayTransactionHistory();
                break;
            case 6:
                saveDataToFile();
                saveTransactionToFile();
                printf("Exiting the program. Data saved. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 6);
}

int main() {
    startProgram();
    return 0;
}
