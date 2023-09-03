#include "httpd.h"

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

        if (!(fscanf(Login, "%s %s", userName, userPass) == 2))
        {
            printf("Failed to read data from the file.\n");
        }

        fclose(Login);
        printf("start1\n");
        printf("%s, %s\n", buffTmp[0], buffTmp[1]);
        printf("start2\n");
        printf("%s, %s\n", userName, userPass);

        if (userName == buffTmp[0] && userPass == buffTmp[1])
        {
            printf("good\n");
        }
        else
        {
            printf("no no good\n");
        }

        // FILE *file = fopen("indexPost.html", "r");

        // if (file == NULL)
        // {
        //     perror("Unable to open file");
        // }
        // char character;
        // while ((character = fgetc(file)) != EOF)
        // {
        //     printf("%c", character);
        // }
        // fclose(file);
    }

    ROUTE_END()
}
