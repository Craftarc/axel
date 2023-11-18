#ifndef AXEL_EXCEPTION_H
#define AXEL_EXCEPTION_H

#include <stdexcept>

#define BACKWARD_HAS_BFD 1
#include <backward.hpp>

using namespace backward;

namespace axel {
    class Exception : public std::runtime_error {
        public:
            Exception(const std::string& message);

        private:
            void print_stack_trace();

            /// @brief Prints a single trace (frame).
            /// @param A single resolved trace.
            /// @param count The number to give to this frame.
            void print_frame(ResolvedTrace& trace, size_t number);

            SnippetFactory snippet_;
            size_t context_size_ = 3;
    };
}  // namespace axel
#endif  // AXEL_EXCEPTION_H
