#include <shared/layout_base.hpp>
#include <xaml/ui/controls/layout_base.h>

using namespace std;

xaml_result xaml_layout_base_internal::draw_impl(xaml_rectangle const&, std::function<xaml_result(xaml_control*, xaml_rectangle const&)> const&) noexcept
{
    XAML_FOREACH_START(xaml_control, cc, m_children);
    {
        bool inited;
        XAML_RETURN_IF_FAILED(cc->get_is_initialized(&inited));
        if (!inited)
        {
            xaml_margin margin;
            XAML_RETURN_IF_FAILED(cc->get_margin(&margin));
            XAML_RETURN_IF_FAILED(cc->draw(xaml_rectangle{} + margin));
        }
        XAML_RETURN_IF_FAILED(cc->size_to_fit());
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_layout_base_members(xaml_type_info_registration* __info) noexcept
{
    return xaml_multicontainer_members(__info);
}

xaml_result XAML_CALL xaml_layout_base_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_layout_base, "xaml/ui/controls/layout_base.h");
    XAML_RETURN_IF_FAILED(xaml_layout_base_members(__info));
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_orientation_register(xaml_meta_context* ctx) noexcept
{
    XAML_ENUM_INFO_MAP_NEW();
    XAML_ENUM_INFO_ADD2(xaml_orientation, horizontal);
    XAML_ENUM_INFO_ADD2(xaml_orientation, vertical);
    XAML_ENUM_INFO_NEW(xaml_orientation, "xaml/ui/controls/layout_base.h");
    return ctx->add_type(__info);
}
