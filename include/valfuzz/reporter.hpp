/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#pragma once

#include <sstream>
#include <memory>
#include <vector>
#include <iostream> 

namespace valfuzz
{

struct report
{
  const std::string benchmark_name;
  long unsigned int input_size;
  double min;
  double max;
  double median;
  double mean;
  double standard_deviation;
  double q1;
  double q2;
};

/**
 * Abstract class for a reporter.
 */
class reporter
{
public:
  reporter() = default;
  virtual ~reporter() = 0;

  virtual std::string id() const = 0;
  virtual std::ostringstream output(struct report *rep) const = 0;
};

/**
  * This class stores and retrieves reporters
  */
class reporter_engine
{
public: 
  reporter_engine() = default;
  ~reporter_engine() = default;
  
  std::ostringstream report(struct report* rep, std::string id) const noexcept
  {
    for (auto& r : reporters)
      if (r->id() == id)
	  return r->output(rep);
    return std::ostringstream();
  }

  void add_reporter(std::shared_ptr<reporter> in_reporter) noexcept
  {
    this->reporters.push_back(in_reporter);
  }

  bool has([[maybe_unused]]std::string id)
  {
    for (auto& r : reporters)
      {
	if (r->id() == id)
	  return true;
      }
    return false;
  }
  
private:
  std::vector<std::shared_ptr<reporter>> reporters;
};

/**
 * Global reporter engine
 */
extern reporter_engine reporter_eg;

#define ADD_REPORTER(in_rep) \
    static struct in_rep##_reporter \
    {                                                                         \
        in_rep##_reporter()                                                   \
        {                                                                     \
	  reporter_eg.add_reporter( \
                      std::make_shared<in_rep>(in_rep()));	      	\
        }                                                                     \
    } in_rep##_reporter_instance;
  
class default_reporter : public reporter
{
public:
  std::string id() const override
  {
    return "default";
  }
  std::ostringstream output(struct report *rep) const override
  {
    std::ostringstream oss;
    oss << "benchmark: \"" << rep->benchmark_name
		<< "\"\n - space: " << rep->input_size
                << "\n - min: " << rep->min
		<< "s\n - max: " << rep->max
		<< "s\n - median: " << rep->median
		<< "s\n - mean: " << rep->mean
		<< "s\n - standard deviation: " << rep->standard_deviation
		<< "\n - Q1: " << rep->q1
		<< "s\n - Q3: " << rep->q2 << "\n";
    return oss;
  }
};

class csv_reporter : public reporter
{
public:
  std::string id() const override
  {
    return "csv";
  }
  std::ostringstream output(struct report *rep) const override
  {
    std::ostringstream oss;
    oss << "\"" << rep->benchmark_name
		<< "\"," << rep->input_size
                << "," << rep->min
		<< "," << rep->max
		<< "," << rep->median
		<< "," << rep->mean
		<< "," << rep->standard_deviation
		<< "," << rep->q1
		<< "," << rep->q2 << "\n";
    return oss;
  }
};

/**
 * Do not print anything
 */
class none_reporter : public reporter
{
public:
  std::string id() const override
  {
    return "none";
  }
  std::ostringstream output([[maybe_unused]]struct report *rep) const override
  {
    std::ostringstream oss;
    return oss;
  }
};

std::string &get_reporter();
void set_reporter(std::string rep);
  
} // valfuzz
