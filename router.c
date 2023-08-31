#include "httpd.h"


void route()
{
    ROUTE_START()

    ROUTE_GET("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Hello! You are using %s", request_header("User-Agent"));
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
