#include <stdlib.h>
#include <iostream>
#include <vector>
#include "dbconnector.h"
#include "producerstatetable.h"
#include "logger.h"
#include "orch_zmq_config.h"

using namespace std;
using namespace swss;

void usage(char **argv)
{
    cout << "Usage: " << argv[0] << " [start|stop]" << endl;
}

int main(int argc, char **argv)
{
    swss::Logger::getInstance().setMinPrio(swss::Logger::SWSS_INFO);

    SWSS_LOG_ENTER();
    DBConnector db("APPL_DB", 0);

    // When the feature ORCH_NORTHBOND_ROUTE_ZMQ_ENABLED is enabled, route events must be sent to orchagent via the ZMQ channel.
    std::shared_ptr<ZmqClient> zmqClient = create_local_zmq_client(ORCH_NORTHBOND_ROUTE_ZMQ_ENABLED, false);
    std::shared_ptr<ProducerStateTable> r = createProducerStateTable(&db, APP_ROUTE_TABLE_NAME, zmqClient);

    if (argc != 2)
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    std::string op = std::string(argv[1]);
    if (op == "stop")
    {
        r->del("resync");
    }
    else if (op == "start")
    {
        FieldValueTuple fv("nexthop", "0.0.0.0");
        std::vector<FieldValueTuple> fvVector = { fv };
        r->set("resync", fvVector);
    }
    else
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
