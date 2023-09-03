#include "httpd.h"
#include <string.h>
void route()
{
    ROUTE_START()

    ROUTE_GET("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        // printf("Hello! You are using %s", request_header("User-Agent"));
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

    ROUTE_GET("../images/lion_awake.jpg") {
        
        printf("HTTP/1.1 200 OK\r\n");
    FILE *image = fopen("images/lion_awake.jpg", "rb");
    if (image == NULL) {
        perror("Unable to open image file");
    } else {
        fseek(image, 0, SEEK_END);
        long image_size = ftell(image);
        rewind(image);

        printf("Content-Type: image/jpg\r\n");
        printf("Content-Length: %ld\r\n\r\n", image_size);
        

        // Send the image data
        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), image)) > 0) {
            fwrite(buffer, 1, bytes_read, stdout);
        }

        fclose(image);
    }
}
 ROUTE_GET("/images/lion_sleeping.jpg") {
    printf("HTTP/1.1 200 OK\r\n");

    // Specify the correct path to the image file relative to your server's root
    FILE *image = fopen("images/lion_sleeping.jpg", "rb");

    if (image == NULL) {
        perror("Unable to open image file");
    } else {
        fseek(image, 0, SEEK_END);
        long image_size = ftell(image);
        rewind(image);

        printf("Content-Type: image/jpeg\r\n"); // Change to "image/jpeg" for JPEG images
        printf("Content-Length: %ld\r\n\r\n", image_size); // Send the content length

        // Send the image data
        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), image)) > 0) {
            fwrite(buffer, 1, bytes_read, stdout);
        }

        fclose(image);
    }
}


    ROUTE_GET("/css/style.css"){
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
        // printf("Wow, seems that you POSTed %d bytes. \r\n", payload_size);
        // printf("%s\n", payload);
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
            FILE *file = fopen("indexPost.html", "r");

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
    }

    ROUTE_END()
}
