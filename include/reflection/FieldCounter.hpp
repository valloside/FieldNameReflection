#pragma once

#include <utility>

namespace valloside::rflct
{
    struct FieldCounter
    {
        template <typename T>
            requires(std::is_aggregate_v<std::remove_cvref_t<T>>)
        static constexpr std::size_t count()
        {
            using T_ = std::remove_cvref_t<T>;
            constexpr std::size_t count = countImpl<T_>();
            if constexpr (count > 1)
                return count - countArray<T_, count>();
            else
                return count;
        }

    private:
        struct Count
        {
            size_t c;

            template <typename T>
                requires(!std::is_copy_constructible_v<T> && !std::is_move_constructible_v<T>)
            constexpr operator T() const;

            template <typename T>
                requires std::is_copy_constructible_v<T>
            constexpr operator T &() const;

            template <typename T>
                requires std::is_move_constructible_v<T>
            constexpr operator T &&() const;
        };

        template <typename T, bool Last = false>
        static constexpr std::size_t countImpl(auto &&...args)
        {
            constexpr bool isConstructible = requires { T{args...}; };
            if constexpr (!isConstructible && Last)
                return sizeof...(args) - 1;
            else if constexpr (sizeof...(args) < 128)
                return countImpl<T, isConstructible>(args..., Count{});
            else
                static_assert(false, "Too many fields");
        }

        template <typename T, std::size_t Total>
            requires(Total > 1)
        static constexpr std::size_t countArray()
        {
            return []<std::size_t... Before>(std::index_sequence<Before...>) {
                std::size_t count = 0;
                std::size_t nextPos = 0;
                ((count += testArrayAt<T, Total, Before>(nextPos)), ...);
                return count;
            }(std::make_index_sequence<Total - 1>{});
        }

        template <typename T, std::size_t Total, std::size_t Pos>
        static constexpr std::size_t testArrayAt(std::size_t &nextPos)
        {
            return testArrayAt<T, Total>(std::make_index_sequence<Pos>{},
                                         std::make_index_sequence<2>{},
                                         std::make_index_sequence<Total - 2 - Pos>{}, nextPos);
        }

        template <typename T, std::size_t Total, std::size_t... Before, std::size_t... Array, std::size_t... After>
        static constexpr std::size_t testArrayAt(std::index_sequence<Before...>,
                                                 std::index_sequence<Array...>,
                                                 std::index_sequence<After...>,
                                                 std::size_t &nextPos,
                                                 std::size_t  count = 0)
        {
            // clang-format off
        constexpr bool ctorAsArray = requires { T{Count{Before}..., {Count{Array}...}, Count{After}...}; };
        constexpr bool ctorAsStruct = requires { T{Count{Before}..., {Count{Array}...}, Count{}, Count{After}...}; };
            // clang-format on

            if (sizeof...(Before) != nextPos)
                return 0;
            if constexpr (ctorAsArray && !ctorAsStruct)
            {
                nextPos = sizeof...(Before) + sizeof...(Array) - 1;
                return count + sizeof...(Array) - 1;
            }
            else if constexpr (sizeof...(After) > 0)
            {
                return testArrayAt<T, Total>(std::make_index_sequence<sizeof...(Before)>{},
                                             std::make_index_sequence<sizeof...(Array) + 1>{},
                                             std::make_index_sequence<Total - 1 - sizeof...(Before) - sizeof...(Array)>{},
                                             nextPos, count);
            }
            else
            {
                ++nextPos;
                return count;
            }
        }
    };

} // namespace valloside::rflct