#include <xaml/ui/controls/button.hpp>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/entry.hpp>
#include <xaml/ui/controls/grid.hpp>
#include <xaml/ui/controls/label.hpp>
#include <xaml/ui/controls/meta.hpp>
#include <xaml/ui/controls/password_entry.hpp>

using namespace std;
using namespace xaml;

static ostream& compile_grid_length(ostream& stream, grid_length length)
{
    stream << "{ " << length.value << ", ";
    stream << enum_meta<grid_layout, char>{}(length.layout);
    return stream << " }";
}

extern "C"
{
    XAML_UI_CONTROLS_META_API void init_meta(void* ctx) noexcept
    {
        shared_ptr<meta_context>* pctx = (shared_ptr<meta_context>*)ctx;
        ui_controls_init_traits::init_meta(pctx ? *pctx : nullptr);
    }

    XAML_UI_CONTROLS_META_API int can_compile(void* t) noexcept
    {
        type_index& type = *(type_index*)t;
        return (type == type_index(typeid(std::vector<grid_length> const&))) ? 1 : 0;
    }

    XAML_UI_CONTROLS_META_API void compile(void* t, const char* code, void* res) noexcept
    {
        auto lengths = value_converter_traits<std::vector<grid_length> const&>::convert(code);
        ostringstream stream;
        stream << "{ ";
        auto bit = lengths.begin();
        auto eit = lengths.end();
        if (bit != eit)
        {
            compile_grid_length(stream, *bit);
            for (++bit; bit != eit; ++bit)
            {
                compile_grid_length(stream << ", ", *bit);
            }
        }
        stream << " }";
        *(string*)res = stream.str();
    }
}

namespace xaml
{
    void ui_controls_init_traits::init_meta(shared_ptr<meta_context> const& ctx) noexcept
    {
        init_context(ctx);
        register_class<
            grid,
            canvas,
            button_base,
            button,
            label,
            entry,
            password_entry>();
        REGISTER_ENUM(xaml, grid_layout);
    }
} // namespace xaml