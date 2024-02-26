#include "portlog.hxx"

int main (void) {

    Portlog::Outputs::File *file = new Portlog::Outputs::File("test.log");
    Portlog::Outputs::StdOut *std_out = new Portlog::Outputs::StdOut();
    Portlog::Extensions::ThreadId thread_id;
    Portlog::Extensions::Timestamp time_stamp;

    Portlog::Logging log(Portlog::LOG_LEVEL::DEBUG,
                         std::vector<Portlog::Output*>{file, std_out},
                         std::vector<Portlog::Extension*>{&thread_id, &time_stamp});

    log.writeLog(Portlog::LOG_LEVEL::DEBUG, "Hello log!");

    return 0;
}
