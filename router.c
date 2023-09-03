#include "httpd.h"


void route()
{
    ROUTE_START()

    ROUTE_GET("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        // printf("Hello! You are using %s", request_header("User-Agent"));
    FILE *file = fopen("index.html", "r");

    // Check if the file was opened successfully
    if (file == NULL) {
        perror("Unable to open file");
    }

    // Read and print the content of the file
    char character;
    while ((character = fgetc(file)) != EOF) {
        printf("%c", character);
    }

    // Close the file
    fclose(file);
    }

    ROUTE_GET("/pass")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Hello! you got here %s , %s %s",method,uri,qs);
    }

    ROUTE_POST("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Wow, seems that you POSTed %d bytes. \r\n", payload_size);
        printf("Fetch the data using `payload` variable.");
    }
  
    ROUTE_END()
}
