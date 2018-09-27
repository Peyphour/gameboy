#include "main.h"

int main(int argc, char *argv[]) {

    core core;
    core_init(&core);
    core_loop(&core);

    exit(EXIT_SUCCESS);
}