#include "axel/Exception.h"

namespace axel {
    Exception::Exception(const std::string& message) :
        std::runtime_error(message) {
        print_stack_trace();
    }

    void Exception::print_stack_trace() {
        using namespace backward;
        StackTrace st;
        st.load_here(8);
        Printer p;
        p.object = false;
        p.snippet = true;
        p.color_mode = ColorMode::always;
        p.address = true;
        p.print(st, stderr);
    }
}  // namespace axel
