#include "ServerBase.h"

int main(int argc, char *argv[])
{
    ServerBase* base = ServerBase::GetInstance();

    base->Run();
    return 0;
}
