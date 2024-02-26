#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>



namespace Portlog {

    enum class LOG_LEVEL {
            DEBUG = 0,
            INFO,
            WARN,
            ERROR,
            FATAL
    };

    class Output {
        public:
            virtual ~Output() {}
            virtual void writeOutput(const char *) = 0;
    };

    class Extension {
        public:
            virtual ~Extension() {}
            virtual std::string getExtensionValue() = 0;
    };

    namespace Outputs {
        class StdOut: public Output {
            public:
                StdOut() {}
                ~StdOut() {}
                void writeOutput(const char *out) override {
                    std::cout << out << std::endl;
                }
        };

        class File: public Output {
            private:
                std::ofstream ofile;

            public:
                File(std::string file) {
                    ofile.open(file, std::ofstream::out);
                }

                ~File() {}

                void writeOutput(const char *out) override {
                    ofile << out << std::endl;
                }
        };
    }

    namespace Extensions {
        class Timestamp: public Extension {
            public:
                Timestamp() {}
                ~Timestamp() {}
                std::string getExtensionValue() override {
                    return "[time_stamp]";
                }
        };

        class ThreadId: public Extension {
            public:
                ThreadId() {}
                ~ThreadId() {}
                std::string getExtensionValue() override {
                    return "[thread_id]";
                }
        };
    }

    class Logging {
        private:
            LOG_LEVEL global_level;
            std::vector<Output*> outputs_;
            std::vector<Extension*> extensions_;
            Outputs::StdOut *std_out;

        public:
            Logging(LOG_LEVEL level): global_level(level) {
                std_out = new Outputs::StdOut();
                outputs_.push_back(std_out);
            }

            Logging(LOG_LEVEL level,
                    const std::vector<Output*> outputs,
                    const std::vector<Extension*> extensions): global_level(level), outputs_(outputs), extensions_(extensions) {
            }

            Logging(const Logging&) {}

            ~Logging() {
                for (auto& it: outputs_) {
                    Output *output = it;
                    delete output;
                }

                outputs_.clear();            
            }

            void setLogLevel(LOG_LEVEL level) {
                global_level = level;
            }

            void writeLog(LOG_LEVEL level, const char *log) {
                std::string message;

                if (global_level > level) {
                    return;
                }

                for (auto& ext: extensions_) { 
                    message.append(ext->getExtensionValue());
                }

                message.append(log);

                for (auto& output: outputs_) {
                    output->writeOutput(message.c_str());
                }
            }
    };
}
