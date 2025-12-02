// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/reporter.hpp>

namespace valfuzz
{
reporter_engine reporter_eg;

ADD_REPORTER(default_reporter);
ADD_REPORTER(csv_reporter);
ADD_REPORTER(none_reporter);

reporter::~reporter()
{
}

std::string &get_reporter()
{
  static std::string reporter_id = "default";
  return reporter_id;
}

void set_reporter(std::string rep)
{
  std::string &rep_id = get_reporter();
  rep_id = rep;
}

} // namespace valfuzz
