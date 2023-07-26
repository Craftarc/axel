#include "axel/Handler.h"
#include "axel/HandlerManager.h"

int main() {
    axel::HandlerManager("production").run_handler();
    return 0;
}