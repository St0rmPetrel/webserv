#include "Options.hpp"

using namespace server;

Options::Options() : recv_buffer_size(4096), request_body_size_limit(3276800) { }
