#include <string_view>
#include <array>

#include "FieldCounter.hpp"

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#    define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

namespace valloside::rflct
{
    struct FieldNames
    {
        template <typename T>
            requires(std::is_aggregate_v<std::remove_cvref_t<T>>)
        static constexpr auto get()
        {
            using T_ = std::remove_cvref_t<T>;
            constexpr std::size_t count = FieldCounter::count<T_>();
            if constexpr (count > 0)
                return GetImpl<T, count>::get(std::make_index_sequence<count>{});
            else
                return std::array<std::string_view, 0>{};
        }

    private:
        template <typename T, auto ptr>
        static constexpr auto getOne()
        {
            std::string_view prettyName = __PRETTY_FUNCTION__;
#if defined(__clang__)
            return prettyName.substr(0, prettyName.size() - 1).substr(prettyName.find_last_of('.') + 1);
#elif defined(__GNUC__)
            return prettyName.substr(0, prettyName.size() - 2).substr(prettyName.find_last_of(':') + 1);
#elif defined(_MSC_VER)
            return prettyName.substr(0, prettyName.size() - 7).substr(prettyName.find_last_of('-') + 2);
#else
            static_assert(false, "Unsupported compiler.");
#endif
        }

        template <typename T>
        struct DeclVal
        {
            inline static std::remove_cvref_t<T> value{};
        };

        template <typename T, std::size_t N>
        struct GetImpl;

#define GEN_REFLECT_GET_FIELD_NAMES(N, ...)                                               \
    template <typename T>                                                                 \
    struct GetImpl<T, N>                                                                  \
    {                                                                                     \
        template <std::size_t... Idx>                                                     \
        static constexpr std::array<std::string_view, N> get(std::index_sequence<Idx...>) \
        {                                                                                 \
            constexpr auto members = []() {                                               \
                auto &[__VA_ARGS__] = DeclVal<T>::value;                                  \
                return std::tie(__VA_ARGS__);                                             \
            }();                                                                          \
            return {getOne<T, &std::get<Idx>(members)>()...};                             \
        };                                                                                \
    };

#include "macros/GEN_REFLECT_GET_FIELD_NAMES.inl"
    };

} // namespace valloside::rflct
