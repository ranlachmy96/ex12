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
        int validLogin = 0;
        char line_entry[256];
        FILE *Login = fopen("LoginInfo.txt", "r");

        if (Login == NULL)
        {
            perror("Failed to open the file");
        }
        while (fscanf(Login,"%s",line_entry) == 1)
        {
            if (strcmp(buffTmp[0], line_entry) == 0)
            {
                // Username match found, now read the next line (password).
                if (fscanf(Login,"%s",line_entry) == 1)
                {
                    
                    if (strcmp(buffTmp[1], line_entry) == 0)
                    {
                        validLogin = 1; // Password match found.
                        break;
                    }
                }
            }
        }

        fclose(Login);

        if (validLogin)
        {
            FILE *fileData = fopen("data.txt", "r");

            if (fileData == NULL)
            {
                perror("Error opening file");
            }
            else
            {
                char line[100];
                char *file_content = NULL;
                size_t file_size = 0;
                int read_mode = 0; // 0 for name, 1 for message

                // Read lines until we find the start marker "@"
                while (fgets(line, sizeof(line), fileData))
                {
                    if (read_mode == 0 && strstr(line, buffTmp[0]) != NULL)
                    {
                        read_mode = 1; // Start reading messages
                        continue;      // Skip the name line
                    }
                    else if (read_mode == 1 && strstr(line, "@") != NULL)
                    {
                        // Found the end marker, stop reading
                        read_mode = 0;
                        break;
                    }

                    if (read_mode == 1)
                    {
                        // Append the line to the content
                        size_t line_len = strlen(line);
                        char *temp = realloc(file_content, file_size + line_len + 1);
                        if (temp == NULL)
                        {
                            perror("Memory allocation error");
                            fclose(fileData);
                            if (file_content != NULL)
                            {
                                free(file_content);
                            }
                            return;
                        }

                        file_content = temp;
                        memcpy(file_content + file_size, line, line_len);
                        file_size += line_len;
                    }
                }

                file_content[file_size] = '\0';
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
    }

    //     ROUTE_POST("/data")
    //     {
    //         printf("HTTP/1.1 200 OK\r\n\r\n");
    //         char *buffTmp[1];
    //         char *token = strtok(payload, "&");
    //         int i = 0;
    //         while (token != NULL)
    //         {
    //             char *equalSign = strchr(token, '=');

    //             if (equalSign != NULL)
    //             {
    //                 char *value = equalSign + 1;
    //                 buffTmp[i] = value;
    //                 i++;
    //             }

    //             token = strtok(NULL, "&");
    //         }
    //         char* data=strdup(urlDecode(buffTmp[0]));
    // FILE *textData = fopen("data.txt", "a");

    //     if (textData == NULL) {
    //         perror("Failed to open the file");
    //     }

    //     if (data != NULL) {
    //         fputs(data, textData);
    //         fclose(textData);
    //     } else {
    //         perror("URL decoding failed");
    //         fclose(textData);
    //     }

    //         printf("<!DOCTYPE html>\n");
    //             printf("<html lang=\"en\">\n");
    //             printf("<head>\n");
    //             printf("    <meta charset=\"UTF-8\">\n");
    //             printf("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    //             printf("    <title>index</title>\n");
    //             printf("    <link rel=\"stylesheet\" href=\"css/style.css\">\n");
    //             printf("</head>\n");
    //             printf("<body id=\"indexPost\">\n");
    //             printf("    <main id=\"indexMain\">\n");
    //             printf("    \n");
    //             printf("    </main>\n");
    //             printf("    <form id=\"indexForm\" action=\"/data\" method=\"post\">\n");
    //             printf("        <textarea name=\"data\">%s</textarea>\n",data);
    //             printf("        <button type=\"submit\">Submit</button>\n");
    //             printf("    </form>\n");
    //             printf("</body>\n");
    //             printf("</html>\n");
    //     }

    ROUTE_POST("/data")
    {
        // char *decodedPayload = urlDecode(payload);
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
        char filename[] = "data.txt";           // Specify the filename
        char temp_filename[] = "temp_data.txt"; // Temporary filename

        char buffer[MAX_LINE];
        char newline[MAX_LINE];
        int flag = 0;

        file = fopen(filename, "r");
        temp = fopen(temp_filename, "w");

        // Check if either file failed to open, if either did, exit with an error status
        if (file == NULL || temp == NULL)
        {
            perror("Error creating temporary file");
            free(username);
            free(newData);
            return;
        }

        bool keep_reading = true;

        do
        {
            fgets(buffer, MAX_LINE, file);
            if (feof(file))
            {
                keep_reading = false;
            }
            else if (strcmp(buffer, "@") == 0)
            {
                flag = 0;
            }
            else if (flag == 1)
            {
                fputs(newData, temp);
                fputs(buffer, temp);
            }
            else if (strcmp(buffer, username) == 0)
            {
                flag = 1;
            }
            else
            {
                fputs(buffer, temp);
            }
        } while (keep_reading);

        // Close our access to both files as we are done with them
        fclose(file);
        fclose(temp);

        // Remove the original file
        remove(filename);

        // Rename the temp file to the original file's name
        rename(temp_filename, filename);

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
