#include <valfuzz/reporter.hpp>

namespace valfuzz
{
  ADD_REPORTER(default_reporter);
  ADD_REPORTER(csv_reporter);
  ADD_REPORTER(none_reporter);
  
  reporter_engine reporter_eg;

  reporter::~reporter() {}

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

} // valfuzz
