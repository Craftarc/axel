#ifndef AXEL_EXCEPTION_H
#define AXEL_EXCEPTION_H

#include <stdexcept>

#define BACKWARD_HAS_BFD 1
#include <backward.hpp>

namespace axel {
    class Exception : public std::runtime_error {
        public:
            Exception(const std::string& message);

        private:
            void print_stack_trace();
    };
}  // namespace axel
#endif  // AXEL_EXCEPTION_H
