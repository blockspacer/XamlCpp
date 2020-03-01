#include <xaml/ui/controls/entry.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

namespace xaml
{
    void entry::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = gtk_entry_new();
            set_handle(h);
            g_signal_connect(G_OBJECT(get_handle()->handle), "changed", G_CALLBACK(entry::on_changed), this);
        }
        rectangle real = region - get_margin();
        __set_size_noevent({ real.width, real.height });
        draw_size();
        draw_text();
        draw_alignment();
    }

    void entry::draw_size()
    {
        auto [rw, rh] = to_native<tuple<gint, gint>>(get_size());
        gtk_widget_set_size_request(get_handle()->handle, rw, rh);
    }

    void entry::draw_text()
    {
        gtk_entry_set_text(GTK_ENTRY(get_handle()->handle), m_text.c_str());
    }

    void entry::draw_alignment()
    {
        gfloat align;
        switch (m_text_halignment)
        {
        case halignment_t::center:
            align = 0.5;
            break;
        case halignment_t::right:
            align = 1.0;
            break;
        default:
            align = 0;
            break;
        }
        gtk_entry_set_alignment(GTK_ENTRY(get_handle()->handle), align);
    }

    void entry::on_changed(void*, void* data)
    {
        entry* self = (entry*)data;
        self->set_text(gtk_entry_get_text(GTK_ENTRY(self->get_handle()->handle)));
    }

    void entry::__size_to_fit()
    {
        gtk_entry_set_width_chars(GTK_ENTRY(get_handle()->handle), (gint)m_text.length());
        control::__size_to_fit();
    }
} // namespace xaml
