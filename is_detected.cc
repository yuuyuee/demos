// is_detected.cc

#include <iostream>
#include <type_traits>

namespace internal {
template <typename...>
struct make_void {
  using type = void;
};

template <typename... Args>
using void_t = typename make_void<Args...>::type;


template <
    typename Default,
    typename AlwaysVoid,
    template <typename...> class Op,
    typename... Args>
struct detector {
  using value_type = std::false_type;
  using type = Default;
};

template <
    typename Default,
    template <typename...> class Op,
    typename... Args>
struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
  using value_type = std::true_type;
  using type = Op<Args...>;
};

}  // namespace internal

template <template <typename...> class Op, typename... Args>
using is_detected =
    typename internal::detector<void, void, Op, Args...>::value_type;

template <template <typename...> class Op, typename... Args>
using detected_t =
    typename internal::detector<void, void, Op, Args...>::type;

template <template <typename...> class Op, typename... Args>
using detected_or = internal::detector<void, void, Op, Args...>;


template <typename Tp>
using copy_assign_t = decltype(std::declval<Tp&>() = std::declval<Tp const&>());

struct EnableCopyAssign {};
struct DisableCopyAssign {
  DisableCopyAssign(DisableCopyAssign const&) = delete;
  void operator=(DisableCopyAssign const&) = delete;
};

#define COUT(...) \
    std::cout << #__VA_ARGS__ << ": " << (__VA_ARGS__) << std::endl

int main() {
  COUT(is_detected<copy_assign_t, EnableCopyAssign>::value);
  COUT(is_detected<copy_assign_t, DisableCopyAssign>::value);
  return 0;
}
