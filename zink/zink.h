#pragma once

#include "zyre.h"

#include <string>
#include <functional>

#ifndef freen
#define freen(x) do {free(x); x = NULL;} while(0)
#endif // freen

namespace zink
{

struct ZinkRouter final
{
    ZinkRouter (const std::string& name)
        : _args (name, *this)
        , _actor (nullptr)
    {}

    ~ZinkRouter();

    void start();
    void stop();

    void sendRoute(const std::string& routeId, const std::string& routeContent);

    std::function<void(const std::string& sender, const std::string& routeId, const std::string& routeContent)> OnReceive;

private:

    friend void actor_fn (zsock_t *pipe, void *a);

    struct Args
    {
        std::string name;
        ZinkRouter& zink;

        explicit Args(const std::string& n, ZinkRouter& z)
        : name (n)
        , zink (z)
        {}
    };

    Args _args;
    zactor_t *_actor;
};

} // namespace zink