#define ShowMessages 1

#include <iostream>

#if ShowMessages
#define DEBUG(msg) std::cout << msg << std::endl;
#else
#define DEBUG(msg)
#endif