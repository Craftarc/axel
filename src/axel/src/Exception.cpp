#include "axel/Exception.h"

const std::string GREEN = "\033[32m";
const std::string RED = "\033[31m";
const std::string RESET = "\033[0m";

namespace axel {
    Exception::Exception(const std::string& message) :
        std::runtime_error(message),
        snippet_(SnippetFactory()) {
        print_stack_trace();
    }

    void Exception::print_stack_trace() {
        size_t CUT = 4;  // Last 4 traces are from the Exception class itself
        using namespace backward;
        StackTrace st;
        st.load_here(8);

        // Remove useless traces
        TraceResolver tr;
        tr.load_stacktrace(st);
        size_t frame_count{ 1 };
        std::cout << RED << "Printing stacktrace (Most recent call last)"
                  << RESET << std::endl;
        for (size_t i = st.size() - 1; i >= CUT; --i) {
            ResolvedTrace trace = tr.resolve(st[i]);
            print_frame(trace, frame_count);
            frame_count += 1;
        }
    }

    void Exception::print_frame(ResolvedTrace& trace, size_t number) {
        // Print the number of this trace
        std::cout << GREEN << "#" << number << std::endl << RESET;

        // Print the filename
        std::cout << "From file: " << GREEN << trace.source.filename << RESET
                  << std::endl;

        // Print the function name
        std::cout << "In function call: " << GREEN << trace.source.function
                  << RESET << std::endl;

        // Print where the function was
        std::cout << "On line: " << trace.source.line
                  << ", column: " << trace.source.col << std::endl;

        std::cout << std::endl;

        // Print some context
        SnippetFactory::lines_t context_list =
        snippet_.get_snippet(trace.source.filename,
                             trace.source.line,
                             context_size_);
        for (auto& line : context_list) {
            std::cout << RED << line.first << RESET << " " << line.second
                      << std::endl;
        }

        std::cout << "---------------------------" << std::endl << std::endl;
    }
}  // namespace axel
