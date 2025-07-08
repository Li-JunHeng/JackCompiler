#include "Usage.h"
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;
namespace Usage_Helper {

Usage::Usage(int argc, char* argv[]) {
    parse(argc, argv);
}

const string& Usage::getInputPath() const noexcept { return m_input_path; }
const string& Usage::getOutputPath() const noexcept { return m_output_path; }
bool Usage::isHelp()    const noexcept { return m_help; }
bool Usage::isVersion() const noexcept { return m_version; }

void Usage::printHelp(string_view prog) {
    cout << "Usage: " << prog << " [options] source\n\n"
         << "Options:\n"
         << left
         << "  -i, --input <path>   "  << "Path to the Jack source (.jack file or directory). (required)\n"
         << "  -o, --output <path>  "  << "Path for output (.vm file or directory). Defaults to input path with .vm extension or same directory.\n"
         << "  -v, --version        "  << "Print program version and exit.\n"
         << "  -h, --help           "  << "Display this help message and exit.\n"
         << endl;
}

void Usage::printVersion() {
#ifdef PROJECT_VERSION
    cout << "JackCompiler version " << PROJECT_VERSION << '\n';
#else
    cout << "JackCompiler (version info not set at build time)\n";
#endif
}

void Usage::parse(int argc, char* argv[]) {
    static const option long_opts[] = {
        {"input",   required_argument, nullptr, 'i'},
        {"output",  required_argument, nullptr, 'o'},
        {"version", no_argument,       nullptr, 'v'},
        {"help",    no_argument,       nullptr, 'h'},
        {nullptr,   0,                 nullptr,  0 }
    };

    optind = 1;
    int opt;
    while((opt = ::getopt_long(argc, argv, "i:o:vh", long_opts, nullptr)) != -1) {
        switch(opt) {
            case 'i': m_input_path  = optarg; break;
            case 'o': m_output_path = optarg; break;
            case 'v': m_version     = true;  break;
            case 'h': m_help        = true;  break;
            case '?':
            default:
                Logger::ErrorLogger::usageError(runtime_error("Invalid command-line option. Use --help for usage."));
        }
    }
    if(m_input_path.empty() && optind < argc) {
        m_input_path = argv[optind++];
    }
    if(m_input_path.empty() && !m_help && !m_version) {
        m_help = true;
        m_version = true;
        return;
    }
    if(!m_help && !m_version) {
        if(!fs::exists(m_input_path)) {
            Logger::ErrorLogger::usageError(runtime_error("Input path does not exist: " + m_input_path));
        }
    }
    // If output not given, determine default
    if(m_output_path.empty() && !m_input_path.empty()) {
        m_output_path = defaultOutputPath(m_input_path);
    }
    // If input is a directory and output path is a file (not directory), that's an error
    if(fs::is_directory(m_input_path)) {
        if(!m_output_path.empty() && fs::exists(m_output_path) && !fs::is_directory(m_output_path)) {
            Logger::ErrorLogger::usageError(runtime_error("Output path must be a directory when input is a directory."));
        }
        // If output path does not exist and was provided, attempt to create directory
        if(!m_output_path.empty() && !fs::exists(m_output_path)) {
            fs::create_directories(m_output_path);
        }
    }
}

string Usage::defaultOutputPath(const string& input) {
    fs::path p(input);
    if(fs::is_directory(p)) {
        // Default to the same directory for output
        return p.string();
    }
    if(p.extension() == ".jack") {
        p.replace_extension(".vm");
    } else {
        p += ".vm";
    }
    return p.string();
}

} // namespace Usage_Helper
