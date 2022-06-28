#ifndef SNAPSHOT_INTERNAL_TYPES_CHECK_HAS_CLASS_INTERNAL_TO_STRING_H_
#define SNAPSHOT_INTERNAL_TYPES_CHECK_HAS_CLASS_INTERNAL_TO_STRING_H_

#include <type_traits>

namespace snapshot::internal {

template <typename T>
class has_class_internal_to_string {
private:
    template <typename U>
    static auto check(int) -> decltype(std::declval<U>().ToString(), std::true_type());

    template <typename U>
    static std::false_type check(...);

public:
    enum { value = std::is_same<decltype(check<T>(0)), std::true_type>::value };
};

template <typename T>
inline constexpr bool has_class_internal_to_string_v = has_class_internal_to_string<T>::value;

}  // namespace snapshot::internal

#endif  // SNAPSHOT_INTERNAL_TYPES_CHECK_HAS_CLASS_INTERNAL_TO_STRING_H_
