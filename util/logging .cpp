#pragma once

#include <golv/util/logging.hpp>

namespace golv {
void
set_log_level(log_level lvl)
{
    the_logger::instance().set_level(lvl);
}
}