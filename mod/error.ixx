module;

#include <system_error>
#include <type_traits>

export module tslpp.error;

export namespace tslpp {

enum class [[nodiscard]] tslpp_error{
    too_small_buffer = 1,
    invalid_offset,
    invalid_value,
};

class tslpp_category : public std::error_category {
public:
  virtual auto name() const noexcept -> const char* override {
    return "deep.parser parse error";
  }

  virtual constexpr auto message(int e) const noexcept -> std::string override {
    using enum tslpp_error;

    switch (static_cast<tslpp_error>(e)) {
    case too_small_buffer:
      return "the buffer passed is too small";
    case invalid_value:
      return "invalid value";
    default:
      return "unknown error";
    }
  }

  virtual auto default_error_condition(int e) const noexcept
      -> std::error_condition override {
    using enum tslpp_error;

    switch (static_cast<tslpp_error>(e)) {
    case too_small_buffer:
      return std::errc::no_buffer_space;
    case invalid_value:
      return std::errc::bad_message;
    default:
      return std::error_condition(e, *this);
    }
  }

  static auto instance() -> tslpp_category& {
    static tslpp_category cat{};

    return cat;
  }
};

export std::error_code make_error_code(tslpp_error e) {
  return std::error_code(static_cast<int>(e), tslpp_category::instance());
}

export std::error_condition make_error_condition(tslpp_error e) {
  return std::error_condition(static_cast<int>(e), tslpp_category::instance());
}

}  // namespace tslpp

namespace std {

export template <>
struct is_error_code_enum<tslpp::tslpp_error> : std::true_type {};

}  // namespace std