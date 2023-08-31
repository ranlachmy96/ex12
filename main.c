#include "httpd.h"

int main(int c, char** v)
{
    serve_forever("8888");
    return 0;
}

