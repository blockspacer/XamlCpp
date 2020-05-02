#ifndef XAML_UI_CONTROLS_SHARED_COMBO_BOX_HPP
#define XAML_UI_CONTROLS_SHARED_COMBO_BOX_HPP

#include <shared/items_base.hpp>
#include <xaml/ui/controls/combo_box.h>

struct xaml_combo_box_impl : xaml_items_base_implement<xaml_combo_box_impl, xaml_combo_box>
{
    XAML_EVENT_IMPL(text_changed)
    XAML_PROP_STRING_EVENT_IMPL(text)

    XAML_EVENT_IMPL(is_editable_changed)
    XAML_PROP_EVENT_IMPL(is_editable, bool, bool*, bool)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
    virtual xaml_result XAML_CALL draw_items() noexcept;
    virtual xaml_result XAML_CALL draw_text() noexcept;
    virtual xaml_result XAML_CALL draw_sel() noexcept;
    virtual xaml_result XAML_CALL draw_editable() noexcept;

    xaml_result XAML_CALL insert_item(std::int32_t index, xaml_ptr<xaml_object> const& value) noexcept override;
    xaml_result XAML_CALL remove_item(std::int32_t index) noexcept override;
    xaml_result XAML_CALL clear_items() noexcept override;
    xaml_result XAML_CALL replace_item(std::int32_t index, xaml_ptr<xaml_object> const& value) noexcept override;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;
    xaml_result XAML_CALL size_to_fit() noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_COMBO_BOX_HPP
