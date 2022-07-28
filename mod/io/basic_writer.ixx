module;

#include <cstdint>
#include <system_error>
#include <utility>

export module tslpp.io:basic_writer;

import tslpp.error;
import tslpp.buffer;
import tslpp.converters;

export namespace tslpp::io {

class basic_writer {
public:
  constexpr basic_writer(tslpp::buffer& buf) : buf_{buf} {
  }

  template <std::integral T>
  constexpr auto operator()(T value) -> std::error_code {
    return buf_.invoke_allocated<sizeof(T)>([](auto&& buf) {
      tslpp::converters::write_integer<T>(value, buf);
      return std::error_code{};
    });
  }

private:
  tslpp::buffer& buf_;
};

}  // namespace tslpp::io