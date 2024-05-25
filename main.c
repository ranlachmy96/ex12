//Ran Lachmy - 207029679
//Eido Peretz - 314652884

#include "httpd.h"
#include <stdio.h>
#include <stdlib.h>

void showAllUsersAndPasswords() {
    // Define the filename
    const char *filename = "LoginInfo.txt";
    
    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Buffer to hold each line read from the file
    char line[256];

    while (1) {
        // Read the username
        if (fgets(line, sizeof(line), file) == NULL) break;
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Check if the line is empty
        if (line[0] == '\0') continue;
        
        char username[128];
        snprintf(username, sizeof(username), "%s", line);

        // Read the password
        if (fgets(line, sizeof(line), file) == NULL) break;
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Check if the line is empty
        if (line[0] == '\0') continue;
        
        char password[128];
        snprintf(password, sizeof(password), "%s", line);

        // Print the username and password
        printf("Username: %s, Password: %s\n", username, password);
    }

    // Close the file
    fclose(file);
}
int main(int c, char** v)
{
    serve_forever("8888");
    return 0;
}

