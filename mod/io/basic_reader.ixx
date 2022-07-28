module;

#include <cstdint>
#include <system_error>
#include <utility>

export module tslpp.io:basic_reader;

import tslpp.error;
import tslpp.buffer;
import tslpp.converters;

export namespace tslpp::io {

class basic_reader {
public:
  constexpr basic_reader(tslpp::buffer& buf) : buf_{buf} {
  }

  template <std::integral T>
  constexpr auto operator()(T& out) -> std::error_code {
    return buf_.invoke_allocated<sizeof(T)>([&out](auto&& buf) {
      out = tslpp::converters::parse_intger<T>(std::move(buf));
      return std::error_code{};
    });
  }

private:
  tslpp::buffer& buf_;
};

}  // namespace tslpp::io