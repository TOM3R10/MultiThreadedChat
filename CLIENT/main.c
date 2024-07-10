#include "../HEADERS/client.h"
#include "../HEADERS/server.h"

int main() {
    if (start_server())
        return 1;
    return 0;
}