#ifndef MIDILAR_CORE_MOVE_H
#define MIDILAR_CORE_MOVE_H

/**
 * @file Move.h
 * @brief Provides lightweight move semantics utilities for MIDILAR.
 */

namespace MIDILAR::Core {

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

    } // namespace detail

    /// @endcond

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
    constexpr typename detail::RemoveReference<T>::Type&& Move(T&& value) {
        return static_cast<typename detail::RemoveReference<T>::Type&&>(value);
    }

} // namespace MIDILAR::Core

#endif // MIDILAR_CORE_MOVE_H