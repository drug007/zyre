//  --------------------------------------------------------------------------
//  Example Zyre distributed chat application
//
//  Copyright (c) 2010-2014 The Authors
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//  --------------------------------------------------------------------------

#include "test_data.h"

#include "zink.h"

#include <iostream>

int
main (int argc, char *argv [])
{
    if (argc < 2) {
        printf ("usage:\n%s name\n", argv[0]);
        exit (0);
    }

    zink::ZinkRouter zinkRouter(argv[1]);
    zinkRouter.start();

    zinkRouter.OnReceive = [](const std::string& sender, const std::string& routeId, const std::string& routeContent)
    {
        std::cout << sender << " " << routeId << " " << routeContent << std::endl;
    };

    std::string buf;
    buf.resize (1024);
    while (!zsys_interrupted) {
        if (!fgets (&buf[0], buf.size(), stdin))
            break;
        buf [strlen (&buf[0]) - 1] = 0;     // Drop the trailing linefeed

        char *routeId = strtok(&buf[0], " ,.");
        if (!routeId || strlen (routeId) == 0)
        {
            printf ("Wrong route id\n");
            continue;
        }
        char *routeContent = routeId + strlen (routeId) + 1;
        if (!routeContent || strlen (routeContent) == 0)
        {
            printf ("Wrong route content\n");
            continue;
        }

        if (streq (routeContent, "test route"))
            routeContent = const_cast<char*>(testRouteContent);

        if (!routeId || ! routeContent)
        {
            printf ("Error\nid '%s'\nroute '%s'\n", routeId, routeContent);
            continue;
        }
        zinkRouter.sendRoute(std::string(routeId), std::string(routeContent));
    }

    zinkRouter.stop();

    return 0;
}
