#include "executer.h"
#include "message_processor.h"
#include "server.h"
#include <signal.h>

#define PORT 3000

int main()
{
    auto handler = (struct sigaction) { SIG_IGN };
    sigaction(SIGPIPE, &handler, NULL);

    Server::TRequestHandler request_handler = MessageProcessor<BoostExecuter>();
    auto server = Server(PORT, request_handler);
    server.start_processing();

    return 0;
}
