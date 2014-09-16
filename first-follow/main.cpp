//
// Find first&follow set.
//
// Copyright 2014 Towry Wang <http://towry.me>
// MIT License
//
// $2014/9/15$
//

#include "ff.h"
#include <iostream>

void usage(const char *arg)
{
    std::cout << "Copyright 2014 Towry Wang\nUsage:\n";
    std::cout << arg << " [file]\n";
}

int main(int argc, char **argv)
{
    FF f;

#ifdef _DEBUG
    f.input("etc/input");
#else
    if (argc < 2)
    {
        usage(argv[0]);
        return -1;
    }

    f.input(argv[1]);
#endif /* _DEBUG */

    if (f.error())
    {
        std::cout << f.getMessage() << "\n";
        return -1;
    }
    f.parse();

    if (f.error())
    {
        std::cout << f.getMessage() << "\n";
        return -1;
    }
    f.output("hi");

    std::cin.get();
    return 0;
}