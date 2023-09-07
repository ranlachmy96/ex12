#include "httpd.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

char* urlDecode(const char* url) {
    int len = strlen(url);
    char* decoded = (char*)malloc(len + 1); 
    if (decoded == NULL) {
        perror("Memory allocation error");
        exit(1);
    }

    int i, j = 0;
    for (i = 0; i < len; i++) {
        if (url[i] == '+') {
            decoded[j++] = ' ';
        } else if (url[i] == '%' && i + 2 < len) {
            char hex[3] = { url[i + 1], url[i + 2], '\0' };
            int decodedChar;
            sscanf(hex, "%x", &decodedChar);
            decoded[j++] = (char)decodedChar;
            i += 2;
        } else {
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

ROUTE_GET("/images/lion_awake.jpg") {
    printf("HTTP/1.1 200 OK\r\n");

    FILE *image = fopen("images/lion_awake.jpg", "rb");

    if (image == NULL) {
        perror("Unable to open image file");
    } else {
        fseek(image, 0, SEEK_END);
        long image_size = ftell(image);
        rewind(image);

        printf("Content-Type: image/jpeg\r\n");
        printf("Content-Length: %ld\r\n\r\n", image_size); 

        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), image)) > 0) {
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
        FILE *Login = fopen("LoginInfo.txt", "r");

        if (Login == NULL)
        {
            perror("Failed to open the file");
        }

        char userName[100];
        char userPass[100];

        fscanf(Login, "%s %s", userName, userPass);

        fclose(Login);
        if (strcmp(userName, buffTmp[0]) == 0 && strcmp(userPass, buffTmp[1]) == 0)
        {
            FILE *fileData = fopen("data.txt", "r");

            if (fileData == NULL) {
                perror("Error opening file");
            }

            fseek(fileData, 0, SEEK_END);
            long file_size = ftell(fileData);
            fseek(fileData, 0, SEEK_SET);

            char *file_content = (char *)malloc(file_size + 1);

            if (file_content == NULL) {
                perror("Memory allocation error");
                fclose(fileData);
            }

            fread(file_content, 1, file_size, fileData);
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
            printf("        <textarea name=\"data\">%s</textarea>\n",file_content);
            printf("        <button type=\"submit\">Submit</button>\n");
            printf("    </form>\n");
            printf("</body>\n");
            printf("</html>\n");
            fclose(fileData);
        }
        
    }

    ROUTE_POST("/data")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        char *buffTmp[1];
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
        char* data=strdup(urlDecode(buffTmp[0]));
FILE *textData = fopen("data.txt", "w");

    if (textData == NULL) {
        perror("Failed to open the file");
    }

    if (data != NULL) {
        fputs(data, textData);
        fclose(textData);
    } else {
        perror("URL decoding failed");
        fclose(textData);
    }
    
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
            printf("        <textarea name=\"data\">%s</textarea>\n",data);
            printf("        <button type=\"submit\">Submit</button>\n");
            printf("    </form>\n");
            printf("</body>\n");
            printf("</html>\n");
    }


    ROUTE_END()
}
