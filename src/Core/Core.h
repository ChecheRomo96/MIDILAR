#ifndef MIDILAR_CORE_H
#define MIDILAR_CORE_H

/**
 * @file Core.h
 * @brief Provides lightweight core utilities for MIDILAR.
 */

namespace MIDILAR {

    struct TrueType {
        static constexpr bool value = true;
    };

    struct FalseType {
        static constexpr bool value = false;
    };

    template <bool Condition, typename T = void>
    struct EnableIf {
    };

    template <typename T>
    struct EnableIf<true, T> {
        typedef T Type;
    };

    /// @cond INTERNAL

    namespace detail {

        template <typename T>
        struct RemoveReference {
            typedef T Type;
        };

        template <typename T>
        struct RemoveReference<T&> {
            typedef T Type;
        };

        template <typename T>
        struct RemoveReference<T&&> {
            typedef T Type;
        };

        template <typename T>
        T&& DeclVal();

        template <typename T>
        struct IsCopyAssignableHelper {
            template <typename U>
            static TrueType Test(decltype(DeclVal<U&>() = DeclVal<const U&>())*);

            template <typename>
            static FalseType Test(...);

            static constexpr bool value = decltype(Test<T>(nullptr))::value;
        };

    } // namespace detail

    /// @endcond

    template <typename T>
    struct IsCopyAssignable {
        static constexpr bool value =
            detail::IsCopyAssignableHelper<T>::value;
    };

    /**
     * @brief Casts an object to an rvalue reference.
     *
     * This enables move semantics without requiring <utility>.
     *
     * @tparam T Type of the object.
     * @param value Object to cast.
     * @return Rvalue reference to the object.
     *
     * @note Equivalent to std::move.
     */
    template <typename T>
    constexpr typename detail::RemoveReference<T>::Type&& move(T&& value) noexcept {
        return static_cast<typename detail::RemoveReference<T>::Type&&>(value);
    }
} // namespace MIDILAR

#endif // MIDILAR_CORE_H