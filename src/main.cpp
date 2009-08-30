#include "ndssys/ndssys.h"

int main(int argc, char *argv[])
{

    NDSSYSTEM nds;
    if( nds.nds_loadfile("rom/simplest.nds")){

        nds.exec();
    }
    return 0;
}
