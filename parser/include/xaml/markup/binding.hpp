#ifndef XAML_BINDING_EXTENSION_HPP
#define XAML_BINDING_EXTENSION_HPP

#include <xaml/markup/markup_extension.hpp>
#include <xaml/meta/default_property.hpp>
#include <xaml/meta/enum_meta.hpp>
#include <xaml/strings.hpp>

namespace xaml
{
    STRING_CONST(__binding_mode_one_time, "one_time")
    STRING_CONST(__binding_mode_one_way, "one_way")
    STRING_CONST(__binding_mode_one_way_to_source, "one_way_to_source")
    STRING_CONST(__binding_mode_two_way, "two_way")

    template <typename TChar>
    struct enum_meta<binding_mode, TChar> : __enum_meta_helper<enum_meta<binding_mode, TChar>>
    {
        typename __enum_meta_helper<enum_meta<binding_mode, TChar>>::map_type enum_map{
            { __binding_mode_one_time<TChar>, binding_mode::one_time },
            { __binding_mode_one_way<TChar>, binding_mode::one_way },
            { __binding_mode_one_way_to_source<TChar>, binding_mode::one_way_to_source },
            { __binding_mode_two_way<TChar>, binding_mode::two_way }
        };
    };

    template <>
    struct type_guid<meta_box<binding_mode>>
    {
        static constexpr guid value{ 0x01f3d35e, 0x73cc, 0x4abd, { 0xbc, 0xa9, 0xe5, 0x07, 0x70, 0x9e, 0x06, 0x9a } };
    };

    class binding;

    template <>
    struct type_guid<binding>
    {
        static constexpr guid value{ 0x9b0c4424, 0x93f5, 0x45e7, { 0xb8, 0xcf, 0x60, 0x24, 0x27, 0x2d, 0x03, 0x4e } };
    };

    class binding : public markup_extension
    {
    public:
        META_CLASS_IMPL(markup_extension)

    private:
        std::string m_element;

    public:
        std::string_view get_element() const noexcept { return m_element; }
        void set_element(std::string_view value) { m_element = value; }

    private:
        std::string m_path;

    public:
        std::string_view get_path() const noexcept { return m_path; }
        void set_path(std::string_view value) { m_path = value; }

        PROP(mode, binding_mode)

    public:
        ~binding() override
        {
        }

        XAML_API void provide(meta_context& ctx, markup_context& context) override;

#define ADD_BINDING_EXTENSION_MEMBERS() \
    ADD_MARKUP_EXTENSION_MEMBERS();     \
    ADD_PROP(element);                  \
    ADD_PROP(path);                     \
    ADD_PROP(mode);                     \
    ADD_DEF_PROP(path)

    public:
        REGISTER_CLASS_DECL_NOFILE(xaml, binding)
        {
            ADD_CTOR();
            ADD_BINDING_EXTENSION_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_BINDING_EXTENSION_HPP
