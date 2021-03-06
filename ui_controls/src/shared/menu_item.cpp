#include <shared/menu_item.hpp>
#include <xaml/ui/controls/menu_item.h>
#include <xaml/ui/menu_bar.h>

using namespace std;

xaml_result xaml_menu_item_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_click));
    return XAML_S_OK;
}

xaml_result xaml_popup_menu_item_internal::add_submenu(xaml_menu_item* child) noexcept
{
    if (child)
    {
        child->set_parent(static_cast<xaml_control*>(m_outer_this));
        XAML_RETURN_IF_FAILED(m_submenu->append(child));
        XAML_RETURN_IF_FAILED(parent_redraw());
    }
    return XAML_S_OK;
}

xaml_result xaml_popup_menu_item_internal::remove_submenu(xaml_menu_item* child) noexcept
{
    int32_t index;
    XAML_RETURN_IF_FAILED(m_submenu->index_of(child, &index));
    if (index >= 0)
    {
        XAML_RETURN_IF_FAILED(m_submenu->remove_at(index));
    }
    return XAML_S_OK;
}

xaml_result xaml_popup_menu_item_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_menu_item_internal::init());

    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_submenu));
    return XAML_S_OK;
}

xaml_result xaml_check_menu_item_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_menu_item_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_checked_changed));

    int32_t token;
    return m_is_checked_changed->add(
        [this](xaml_object*, bool) noexcept -> xaml_result {
#ifdef XAML_UI_WINDOWS
            if (m_menu_id)
#elif defined(XAML_UI_COCOA)
            if (m_menu)
#elif defined(XAML_UI_GTK3)
            if (m_handle)
#endif // XAML_UI_WINDOWS
                XAML_RETURN_IF_FAILED(draw_checked());
            return XAML_S_OK;
        },
        &token);
}

xaml_result xaml_radio_menu_item_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_menu_item_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_checked_changed));

    int32_t token;
    return m_is_checked_changed->add(
        [this](xaml_object*, bool) noexcept -> xaml_result {
#ifdef XAML_UI_WINDOWS
            if (m_menu_id)
#elif defined(XAML_UI_COCOA)
            if (m_menu)
#elif defined(XAML_UI_GTK3)
            if (m_handle)
#endif // XAML_UI_WINDOWS
            {
                XAML_RETURN_IF_FAILED(draw_checked());
                XAML_RETURN_IF_FAILED(draw_group());
            }
            return XAML_S_OK;
        },
        &token);
}

#ifndef XAML_UI_GTK3
xaml_result xaml_radio_menu_item_internal::draw_group() noexcept
{
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent && m_is_checked)
    {
        xaml_ptr<xaml_menu_bar> multic;
        if (XAML_SUCCEEDED(parent->query(&multic)))
        {
            xaml_ptr<xaml_vector_view<xaml_control>> children;
            XAML_RETURN_IF_FAILED(multic->get_children(&children));
            XAML_FOREACH_START(xaml_control, c, children);
            {
                if (auto rc = c.query<xaml_radio_menu_item>())
                {
                    if (rc.get() != static_cast<xaml_control*>(m_outer_this))
                    {
                        xaml_ptr<xaml_string> group;
                        XAML_RETURN_IF_FAILED(rc->get_group(&group));
                        bool equals;
                        XAML_RETURN_IF_FAILED(xaml_string_equals(group, m_group, &equals));
                        if (equals)
                        {
                            XAML_RETURN_IF_FAILED(rc->set_is_checked(false));
                        }
                    }
                }
            }
            XAML_FOREACH_END();
        }
    }
    return XAML_S_OK;
}
#endif // !XAML_UI_GTK3

xaml_result XAML_CALL xaml_menu_item_new(xaml_menu_item** ptr) noexcept
{
    return xaml_object_init<xaml_menu_item_impl>(ptr);
}

xaml_result XAML_CALL xaml_menu_item_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_menu_item;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_menu_item_new);
    XAML_TYPE_INFO_ADD_PROP(text, xaml_string);
    XAML_TYPE_INFO_ADD_EVENT(click);
    XAML_TYPE_INFO_ADD_DEF_PROP(text);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_menu_item_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_menu_item, "xaml/ui/controls/menu_item.h");
    XAML_RETURN_IF_FAILED(xaml_menu_item_members(__info));
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_popup_menu_item_new(xaml_popup_menu_item** ptr) noexcept
{
    return xaml_object_init<xaml_popup_menu_item_impl>(ptr);
}

xaml_result XAML_CALL xaml_popup_menu_item_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_popup_menu_item;
    XAML_RETURN_IF_FAILED(xaml_menu_item_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_popup_menu_item_new);
    XAML_TYPE_INFO_ADD_CPROP(submenu, xaml_menu_item);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_popup_menu_item_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_popup_menu_item, "xaml/ui/controls/menu_item.h");
    XAML_RETURN_IF_FAILED(xaml_popup_menu_item_members(__info));
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_check_menu_item_new(xaml_check_menu_item** ptr) noexcept
{
    return xaml_object_init<xaml_check_menu_item_impl>(ptr);
}

xaml_result XAML_CALL xaml_check_menu_item_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_check_menu_item;
    XAML_RETURN_IF_FAILED(xaml_menu_item_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_check_menu_item_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_checked, bool);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_check_menu_item_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_check_menu_item, "xaml/ui/controls/menu_item.h");
    XAML_RETURN_IF_FAILED(xaml_check_menu_item_members(__info));
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_radio_menu_item_new(xaml_radio_menu_item** ptr) noexcept
{
    return xaml_object_init<xaml_radio_menu_item_impl>(ptr);
}

xaml_result XAML_CALL xaml_radio_menu_item_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_radio_menu_item;
    XAML_RETURN_IF_FAILED(xaml_menu_item_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_radio_menu_item_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_checked, bool);
    XAML_TYPE_INFO_ADD_PROP(group, xaml_string);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_radio_menu_item_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_radio_menu_item, "xaml/ui/controls/menu_item.h");
    XAML_RETURN_IF_FAILED(xaml_radio_menu_item_members(__info));
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_separator_menu_item_new(xaml_separator_menu_item** ptr) noexcept
{
    return xaml_object_init<xaml_separator_menu_item_impl>(ptr);
}

xaml_result XAML_CALL xaml_separator_menu_item_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_separator_menu_item;
    XAML_RETURN_IF_FAILED(xaml_menu_item_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_separator_menu_item_new);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_separator_menu_item_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_separator_menu_item, "xaml/ui/controls/menu_item.h");
    XAML_RETURN_IF_FAILED(xaml_separator_menu_item_members(__info));
    return ctx->add_type(__info);
}
