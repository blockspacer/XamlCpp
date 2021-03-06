#include <shared/webview.hpp>
#include <xaml/ui/controls/webview.h>

using namespace std;

xaml_result xaml_webview_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_uri_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_resource_requested));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_uri_changed->add(
        [this](xaml_object*, xaml_string*) noexcept -> xaml_result {
            if (m_handle && !m_navigating)
            {
                XAML_RETURN_IF_FAILED(draw_uri());
            }
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_webview_new(xaml_webview** ptr) noexcept
{
    return xaml_object_init<xaml_webview_impl>(ptr);
}

xaml_result XAML_CALL xaml_webview_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_webview;
    XAML_TYPE_INFO_ADD_PROP_EVENT(uri, xaml_string);
    XAML_TYPE_INFO_ADD_EVENT(resource_requested);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_webview_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_webview, "xaml/ui/controls/webview.h");
    XAML_RETURN_IF_FAILED(xaml_webview_members(__info));
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_webview_web_request_new(xaml_webview_web_request** ptr) noexcept
{
    return xaml_object_new<xaml_webview_web_request_impl>(ptr);
}

xaml_result XAML_CALL xaml_webview_web_response_new(xaml_webview_web_response** ptr) noexcept
{
    return xaml_object_new<xaml_webview_web_response_impl>(ptr);
}

xaml_result XAML_CALL xaml_webview_resource_requested_args_new(xaml_webview_resource_requested_args** ptr) noexcept
{
    return xaml_object_new<xaml_webview_resource_requested_args_impl>(ptr);
}
