#include "ServerBase.h"

int main(int argc, char *argv[])
{
    ServerBase base;

    base.Init();
    base.Run();
    return 0;
}
