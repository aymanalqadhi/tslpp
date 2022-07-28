module;

#include <span>
#include <system_error>
#include <utility>

export module tslpp.buffer;

import tslpp.error;

namespace tslpp {

export class buffer {
public:
  constexpr buffer(std::span<std::uint8_t> buf)
      : buf_{std::move(buf)},
        pos_{buf_.begin()} {
  }

  constexpr auto size() const noexcept -> std::size_t {
    return std::distance(std::begin(buf_), pos_);
  }

  constexpr auto remaining() const noexcept -> std::size_t {
    return std::distance(pos_, std::end(buf_));
  }

  template <std::size_t amount>
  constexpr auto take(std::span<std::uint8_t>& buf) noexcept
      -> std::error_code {
    if constexpr (remaining() < amount) {
      return make_error_code(too_small_buffer);
    }

    buf = {pos_, pos_ + amount};
    pos_ += amount;

    return {};
  }

  template <std::size_t amount>
  constexpr auto give_back() noexcept -> std::error_code {
    if constexpr (size() < amount) {
      return make_error_code(invalid_offset);
    }

    pos_ -= amount;

    return {};
  }

  template <std::size_t amount, typename Handler>
  constexpr auto invoke_allocated(Handler&& cb) -> std::error_code {
    std::error_code ec;
    std::span<std::uint8_t> buf{};

    if (ec = this->take<amount>(); !ec) {
      return ec;
    }

    if (ec = cb(std::move(buf))) {
      return give_back<amount>() || ec;
    }

    return {};
  }

private:
  std::span<std::uint8_t> buf_;
  decltype(buf_)::iterator pos_;
};

}  // namespace tslpp