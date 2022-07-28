module;

#include <bit>
#include <cassert>
#include <concepts>
#include <span>
#include <system_error>

export module tslpp.converters:integer;

import tslpp.error;

export namespace tslpp::converters {

template <std::integral T, std::endian endianness = std::endian::native>
constexpr auto parse_intger(std::span<std::uint8_t> buf) noexcept -> T {
  assert(sizeof(T) == buf.size());

  if constexpr (sizeof(T) == 1U) {
    return static_cast<T>(buf[0]);
  }

  T ret{};

  for (std::size_t i{0}; i < sizeof(T); ++i) {
    if constexpr (endianness == std::endian::big) {
      ret <<= 8U;
      ret |= buf[i];
    } else {
      ret >>= 8U;
      ret |= static_cast<T>(buf[i]) << (8U * (sizeof(T) - 1U));
    }
  }

  return ret;
}

template <typename T, std::endian endianness = std::endian::native>
constexpr void write_integer(T value, std::span<std::uint8_t> buf) noexcept {
  for (std::size_t i{0}; i < sizeof(T); ++i) {
    if constexpr (endianness == std::endian::big) {
      constexpr auto offset = (sizeof(T) - i - 1) * 8U;

      buf[i] = static_cast<std::uint8_t>((value >> offset) & 0xFF);
    } else {
      buf[i] = static_cast<std::uint8_t>(value & 0xFF);
      value >>= 8U;
    }
  }
}

}  // namespace tslpp::converters