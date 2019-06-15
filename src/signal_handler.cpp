//
// Created by IgVelasco on 6/3/19.
//

#include "signal_handler.h"



void signalHandler( int signum ) {
    //std::cout << "Interrupt signal (" << signum << ") received.\n";

    // cleanup and close up stuff here
    // terminate program
    if(signum != SIGPIPE)
        exit(signum);

}
