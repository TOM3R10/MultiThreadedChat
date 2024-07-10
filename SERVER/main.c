#include "../HEADERS/server.h"
#include "../HEADERS/client.h"

int main(void) {
    if (start_server())
        return 1;
    return 0;
}
