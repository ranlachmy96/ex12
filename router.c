#include "httpd.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define FILENAME_SIZE 1024
#define MAX_LINE 2048
char *urlDecode(const char *url)
{
    int len = strlen(url);
    char *decoded = (char *)malloc(len + 1);
    if (decoded == NULL)
    {
        perror("Memory allocation error");
        exit(1);
    }

    int i, j = 0;
    for (i = 0; i < len; i++)
    {
        if (url[i] == '+')
        {
            decoded[j++] = ' ';
        }
        else if (url[i] == '%' && i + 2 < len)
        {
            char hex[3] = {url[i + 1], url[i + 2], '\0'};
            int decodedChar;

            if (strcmp(hex, "0D") == 0 && i + 5 < len && strncmp(url + i + 3, "0A", 2) == 0)
            {
                // Handle %0D%0A (carriage return and line feed)
                decodedChar = '\n'; // Replace with newline character
                i += 5;             // Skip the next two characters
            }
            else
            {
                sscanf(hex, "%x", &decodedChar);
                i += 2;
            }

            decoded[j++] = (char)decodedChar;
        }
        else
        {
            decoded[j++] = url[i];
        }
    }
    decoded[j] = '\0';
    return decoded;
}

void route()
{
    ROUTE_START()

    ROUTE_GET("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        FILE *file = fopen("index.html", "r");
        if (file == NULL)
        {
            perror("Unable to open file");
        }
        char character;
        while ((character = fgetc(file)) != EOF)
        {
            printf("%c", character);
        }
        fclose(file);
    }

    ROUTE_GET("/pass")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Hello! you got here %s , %s %s", method, uri, qs);
    }

    ROUTE_GET("/images/lion_awake.jpg")
    {
        printf("HTTP/1.1 200 OK\r\n");

        FILE *image = fopen("images/lion_awake.jpg", "rb");

        if (image == NULL)
        {
            perror("Unable to open image file");
        }
        else
        {
            fseek(image, 0, SEEK_END);
            long image_size = ftell(image);
            rewind(image);

            printf("Content-Type: image/jpeg\r\n");
            printf("Content-Length: %ld\r\n\r\n", image_size);

            char buffer[1024];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), image)) > 0)
            {
                fwrite(buffer, 1, bytes_read, stdout);
            }

            fclose(image);
        }
    }

    ROUTE_GET("/images/lion_sleeping.jpg")
    {
        printf("HTTP/1.1 200 OK\r\n");
        FILE *image = fopen("images/lion_sleeping.jpg", "rb");

        if (image == NULL)
        {
            perror("Unable to open image file");
        }
        else
        {
            fseek(image, 0, SEEK_END);
            long image_size = ftell(image);
            rewind(image);

            printf("Content-Type: image/jpeg\r\n");
            printf("Content-Length: %ld\r\n\r\n", image_size);

            char buffer[1024];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), image)) > 0)
            {
                fwrite(buffer, 1, bytes_read, stdout);
            }

            fclose(image);
        }
    }

    ROUTE_GET("/css/style.css")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        FILE *file = fopen("css/style.css", "r");
        if (file == NULL)
        {
            perror("Unable to open file");
        }
        char character;
        while ((character = fgetc(file)) != EOF)
        {
            printf("%c", character);
        }
        fclose(file);
    }

    ROUTE_POST("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        char *buffTmp[3];
        char *token = strtok(payload, "&");
        int i = 0;
        while (token != NULL)
        {
            char *equalSign = strchr(token, '=');

            if (equalSign != NULL)
            {
                char *value = equalSign + 1;
                buffTmp[i] = value;
                i++;
            }

            token = strtok(NULL, "&");
        }

        if (i == 3 && strcmp(buffTmp[2], "register") == 0)
        {
            // Open the file in append mode
            FILE *file = fopen("LoginInfo.txt", "a");
            if (file != NULL)
            {
                // Write the new user and password to the file
                fprintf(file, "%s\n%s\n", buffTmp[0], buffTmp[1]);
                fclose(file);
            }
            const char* extension = ".txt";

            // Calculate the length of the concatenated string
            int length = strlen(buffTmp[0]) + strlen(extension) + 1; // +1 for the null terminator

            // Allocate memory for the new string
            char* fileName = (char*)malloc(length);

            if (fileName == NULL) {
                perror("Memory allocation failed");
            }

            // Concatenate the strings
            strcpy(fileName, buffTmp[0]);
            strcat(fileName, extension);
            FILE *data_file = fopen(fileName, "a");
            if (data_file != NULL)
            {
                fclose(data_file);
            }
        }

        int validLogin = 0;
        char line_entry[256];
        FILE *Login = fopen("LoginInfo.txt", "r");

        if (Login == NULL)
        {
            perror("Failed to open the file");
        }
        while (fscanf(Login, "%s", line_entry) == 1)
        {
            if (strcmp(buffTmp[0], line_entry) == 0)
            {
                if (fscanf(Login, "%s", line_entry) == 1)
                {

                    if (strcmp(buffTmp[1], line_entry) == 0)
                    {
                        validLogin = 1;
                        break;
                    }
                }
            }
        }

        fclose(Login);

        if (validLogin)
        {
            
            const char* extension = ".txt";

            // Calculate the length of the concatenated string
            int length = strlen(buffTmp[0]) + strlen(extension) + 1; // +1 for the null terminator

            // Allocate memory for the new string
            char* fileName = (char*)malloc(length);

            if (fileName == NULL) {
                perror("Memory allocation failed");
            }

            // Concatenate the strings
            strcpy(fileName, buffTmp[0]);
            strcat(fileName, extension);
            FILE *fileData = fopen(fileName, "r");

            if (fileData == NULL)
            {
                perror("Error opening file");
            }
            
                char *file_content;
                long file_size;

                // Open the file in binary mode


                // Get the file size
                fseek(fileData, 0, SEEK_END);
                file_size = ftell(fileData);
                fseek(fileData, 0, SEEK_SET);

                // Allocate memory for the buffer
                file_content = (char *)malloc(file_size + 1);
                if (file_content == NULL) {
                    perror("Error allocating memory");
                    fclose(fileData);
                }

                // Read the file content into the buffer
                fread(file_content, 1, file_size, fileData);
                file_content[file_size] = '\0'; // Null-terminate the string

                // Close the file


                printf("<!DOCTYPE html>\n");
                printf("<html lang=\"en\">\n");
                printf("<head>\n");
                printf("    <meta charset=\"UTF-8\">\n");
                printf("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
                printf("    <title>index</title>\n");
                printf("    <link rel=\"stylesheet\" href=\"css/style.css\">\n");
                printf("</head>\n");
                printf("<body id=\"indexPost\">\n");
                printf("    <main id=\"indexMain\">\n");
                printf("<div>\n");
                printf("<a href=\"\" id=\"logout\">Logout</a>\n");
                printf("</div>\n");
                printf("    </main>\n");
                printf("    <form id=\"indexForm\" action=\"/data\" method=\"post\">\n");
                printf("         <input type=\"hidden\" name=\"user\" value=\"%s\">", buffTmp[0]);
                printf("        <textarea name=\"data\">%s</textarea>\n", file_content);
                printf("        <button type=\"submit\">Submit</button>\n");
                printf("    </form>\n");
                printf("</body>\n");
                printf("</html>\n");
                free(file_content);
                fclose(fileData);
            
        }
    }

    ROUTE_POST("/data")
    {
        //  char *decodedPayload = urlDecode(payload);
        printf("HTTP/1.1 200 OK\r\n\r\n");
        char *buffTmp[2];
        char *token = strtok(payload, "&");
        int i = 0;

        while (token != NULL)
        {
            char *equalSign = strchr(token, '=');

            if (equalSign != NULL)
            {
                char *value = equalSign + 1;
                buffTmp[i] = value;
                i++;
            }

            token = strtok(NULL, "&");
        }

        // Make sure you check if there are two values in buffTmp
        if (i != 2)
        {
            perror("Invalid payload");
            return;
        }

        // Extract the username and new data
        char *username = strdup(urlDecode(buffTmp[0])); // Extract the new data
        char *newData = strdup(urlDecode(buffTmp[1]));
        FILE *file, *temp;

        const char* extension = ".txt";

        // Calculate the length of the concatenated string
        int length = strlen(username) + strlen(extension) + 1; // +1 for the null terminator

        // Allocate memory for the new string
        char* fileName = (char*)malloc(length);

        if (fileName == NULL) {
            perror("Memory allocation failed");
        }

        // Concatenate the strings
        strcpy(fileName, username);
        strcat(fileName, extension);

        file = fopen(fileName,"wr");

        if(file == NULL){
            perror("Unable to open the file");     
        }
        
         fprintf(file, "%s", newData);

        // Close the file
        fclose(file);


        // Display the entire content of data.txt on the web page
        printf("<!DOCTYPE html>\n");
        printf("<html lang=\"en\">\n");
        printf("<head>\n");
        printf("    <meta charset=\"UTF-8\">\n");
        printf("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
        printf("    <title>index</title>\n");
        printf("    <link rel=\"stylesheet\" href=\"css/style.css\">\n");
        printf("</head>\n");
        printf("<body id=\"indexPost\">\n");
        printf("    <main id=\"indexMain\">\n");
        printf("    \n");
        printf("    </main>\n");
        printf("    <form id=\"indexForm\" action=\"/data\" method=\"post\">\n");
        printf("        <textarea name=\"data\">%s</textarea>\n", newData);
        printf("         <input type=\"hidden\" name=\"%s\">", username);
        printf("        <button type=\"submit\">Submit</button>\n");
        printf("    </form>\n");
        printf("</body>\n");
        printf("</html>\n");

        // Clean up dynamically allocated memory
        free(username);
        free(newData);
    
    }

    ROUTE_END()
}

