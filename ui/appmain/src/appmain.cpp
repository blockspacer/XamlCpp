#include <nowide/args.hpp>
#include <nowide/iostream.hpp>
#include <sf/format.hpp>
#include <xaml/ui/application.h>

using namespace std;

static int xaml_main_end(xaml_result hr, int* pcode) noexcept
{
    if (XAML_SUCCEEDED(hr))
    {
        return *pcode;
    }
    else
    {
        sf::println(nowide::cerr, U("Unhandled exception: {}"), xaml_result_get_message(hr));
        return (int)hr;
    }
}

static xaml_result xaml_main_begin(int argc, char** argv, int* pcode) noexcept
{
    xaml_ptr<xaml_application> app;
    XAML_RETURN_IF_FAILED(xaml_application_init_with_args(argc, argv, &app));
    xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
    XAML_RETURN_IF_FAILED((xaml_delegate_new(
        [](xaml_object* sender, xaml_event_args*) noexcept -> xaml_result {
            xaml_ptr<xaml_application> app;
            XAML_RETURN_IF_FAILED(sender->query(&app));
            return xaml_main(app);
        },
        &callback)));
    int32_t token;
    XAML_RETURN_IF_FAILED(app->add_activate(callback, &token));
    return app->run(pcode);
}

int main(int argc, char** argv)
{
    nowide::args _(argc, argv);
    int code;
    return xaml_main_end(xaml_main_begin(argc, argv, &code), &code);
}
