#include "zink.h"

namespace zink
{

void actor_fn (zsock_t *pipe, void *a)
{
    ZinkRouter::Args *args = reinterpret_cast<ZinkRouter::Args*> (a);
    zyre_t *node = zyre_new ((char *) args->name.c_str());
    if (!node)
        return;                 //  Could not create new node

    zyre_start (node);
    zyre_join (node, "CHAT");
    zsock_signal (pipe, 0);     //  Signal "ready" to caller

    bool terminated = false;
    zpoller_t *poller = zpoller_new (pipe, zyre_socket (node), NULL);
    while (!terminated) {
        void *which = zpoller_wait (poller, -1);
        if (which == pipe) {
            zmsg_t *msg = zmsg_recv (which);
            if (!msg)
                break;              //  Interrupted

            char *command = zmsg_popstr (msg);
            if (streq (command, "$TERM"))
                terminated = true;
            else
            if (streq (command, "SHOUT")) {
                zyre_shout (node, "CHAT", &msg);
            }
            else {
                puts ("E: invalid message to actor");
                assert (false);
            }
            free (command);
            zmsg_destroy (&msg);
        }
        else
        if (which == zyre_socket (node)) {
            zmsg_t *msg = zmsg_recv (which);
            char *event = zmsg_popstr (msg);
            char *peer = zmsg_popstr (msg);
            char *name = zmsg_popstr (msg);
            char *group = zmsg_popstr (msg);
            char *message = zmsg_popstr (msg);

            if (streq (event, "ENTER"))
                printf ("%s has joined the chat\n", name);
            else
            if (streq (event, "EXIT"))
                printf ("%s has left the chat\n", name);
            else
            if (streq (event, "SHOUT"))
                if (args->zink.OnReceive)
                {
                    char *routeContent = zmsg_popstr (msg);
                    args->zink.OnReceive(std::string(name), std::string(message), std::string(routeContent));
                    free (routeContent);
                }
            else
            if (streq (event, "EVASIVE"))
                printf ("%s is being evasive\n", name);
            else
            if (streq (event, "SILENT"))
                printf ("%s is being silent\n", name);

            free (event);
            free (peer);
            free (name);
            free (group);
            free (message);
            zmsg_destroy (&msg);
        }
    }
    zpoller_destroy (&poller);
    zyre_stop (node);
    zclock_sleep (100);
    zyre_destroy (&node);
}

ZinkRouter::~ZinkRouter()
{
    stop();
}

void ZinkRouter::start()
{
    if (_actor)
    {
        return;
    }

    _actor = zactor_new (actor_fn, reinterpret_cast<void*>(&_args));
    assert (_actor);
}

void ZinkRouter::stop()
{
    if (_actor)
    {
        zactor_destroy (&_actor);
        _actor = nullptr;
    }
}

void ZinkRouter::sendRoute(const std::string& routeId, const std::string& routeContent)
{
    assert (_actor);
    zstr_sendx (_actor, "SHOUT", routeId.c_str(), routeContent.c_str(), NULL);
}

} // namespace zink
