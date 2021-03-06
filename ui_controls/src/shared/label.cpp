#include <shared/label.hpp>
#include <xaml/ui/controls/label.h>

using namespace std;

xaml_result xaml_label_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_text_changed));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_text_changed->add(
        [this](xaml_object*, xaml_string*) noexcept -> xaml_result {
            if (m_handle)
            {
                XAML_RETURN_IF_FAILED(draw_text());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_label_new(xaml_label** ptr) noexcept
{
    return xaml_object_init<xaml_label_impl>(ptr);
}

xaml_result XAML_CALL xaml_label_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_label;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_label_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(text, xaml_string);
    XAML_TYPE_INFO_ADD_PROP(text_halignment, xaml_halignment);
    XAML_TYPE_INFO_ADD_DEF_PROP(text);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_label_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_label, "xaml/ui/controls/label.h");
    XAML_RETURN_IF_FAILED(xaml_label_members(__info));
    return ctx->add_type(__info);
}
