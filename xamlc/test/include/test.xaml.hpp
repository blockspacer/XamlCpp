#ifndef XAML_TEST_TEST_WINDOW_HPP
#define XAML_TEST_TEST_WINDOW_HPP

#include <xaml/ui/controls/button.hpp>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/timer.hpp>
#include <xaml/ui/window.hpp>

namespace xaml::test
{
    class test_window : public window
    {
    private:
        timer tmr;
        int count{ 0 };

    public:
        test_window();

        ~test_window() override {}

#ifdef XAML_TEST_GEN_FAKE
        void init_components(meta_context& ctx);
#else
        void init_components();
#endif // XAML_TEST_GEN_FAKE

        void on_timer_tick(std::reference_wrapper<timer>);

        void on_button_click(std::reference_wrapper<button> btn);

        void on_canvas_redraw(std::reference_wrapper<canvas> cv, std::reference_wrapper<drawing_context> dc);

        REGISTER_CLASS_DECL_FILE(xaml::test, test_window, "test.xaml.hpp")
        {
            ADD_CTOR_DEF();
            ADD_WINDOW_MEMBERS();
            ADD_METHOD(on_button_click);
            ADD_METHOD(on_canvas_redraw);
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml::test

#endif // !XAML_TEST_TEST_WINDOW_HPP