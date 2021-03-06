#include <algorithm>
#include <numeric>
#include <shared/stack_panel.hpp>
#include <xaml/ui/controls/stack_panel.h>

using namespace std;

xaml_result xaml_stack_panel_internal::draw_impl(xaml_rectangle const& region, function<xaml_result(xaml_control*, xaml_rectangle const&)> const& func) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_layout_base_internal::draw_impl(region, func));
    xaml_rectangle real = region - m_margin;
    if (m_orientation == xaml_orientation_vertical)
    {
        double suby = real.y;
        XAML_FOREACH_START(xaml_control, cc, m_children);
        {
            xaml_margin cm;
            XAML_RETURN_IF_FAILED(cc->get_margin(&cm));
            xaml_size cs;
            XAML_RETURN_IF_FAILED(cc->get_size(&cs));
            double subh = cs.height + cm.top + cm.bottom;
            xaml_rectangle subrect = { real.x, suby, real.width, subh };
            XAML_RETURN_IF_FAILED(cc->draw(subrect));
            if (func) XAML_RETURN_IF_FAILED(func(cc, subrect));
            suby += subh;
        }
        XAML_FOREACH_END();
    }
    else
    {
        double subx = real.x;
        XAML_FOREACH_START(xaml_control, cc, m_children);
        {
            xaml_margin cm;
            XAML_RETURN_IF_FAILED(cc->get_margin(&cm));
            xaml_size cs;
            XAML_RETURN_IF_FAILED(cc->get_size(&cs));
            double subw = cs.width + cm.left + cm.right;
            xaml_rectangle subrect = { subx, real.y, subw, real.height };
            XAML_RETURN_IF_FAILED(cc->draw(subrect));
            if (func) XAML_RETURN_IF_FAILED(func(cc, subrect));
            subx += subw;
        }
        XAML_FOREACH_END();
    }
    return XAML_S_OK;
}

xaml_result xaml_stack_panel_internal::size_to_fit() noexcept
{
    if (m_orientation == xaml_orientation_vertical)
    {
        double height = 0;
        XAML_FOREACH_START(xaml_control, cc, m_children);
        {
            xaml_margin cm;
            XAML_RETURN_IF_FAILED(cc->get_margin(&cm));
            xaml_size cs;
            XAML_RETURN_IF_FAILED(cc->get_size(&cs));
            height += cs.height + cm.top + cm.bottom;
        }
        XAML_FOREACH_END();
        return set_size_noevent({ m_size.width, height });
    }
    else
    {
        double width = 0;
        XAML_FOREACH_START(xaml_control, cc, m_children);
        {
            xaml_margin cm;
            XAML_RETURN_IF_FAILED(cc->get_margin(&cm));
            xaml_size cs;
            XAML_RETURN_IF_FAILED(cc->get_size(&cs));
            width += cs.width + cm.left + cm.right;
        }
        XAML_FOREACH_END();
        return set_size_noevent({ width, m_size.height });
    }
}

xaml_result XAML_CALL xaml_stack_panel_new(xaml_stack_panel** ptr) noexcept
{
    return xaml_object_init<xaml_stack_panel_impl>(ptr);
}

xaml_result XAML_CALL xaml_stack_panel_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_stack_panel;
    XAML_RETURN_IF_FAILED(xaml_layout_base_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_stack_panel_new);
    XAML_TYPE_INFO_ADD_PROP(orientation, xaml_orientation);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_stack_panel_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_stack_panel, "xaml/ui/controls/stack_panel.h");
    XAML_RETURN_IF_FAILED(xaml_stack_panel_members(__info));
    return ctx->add_type(__info);
}
