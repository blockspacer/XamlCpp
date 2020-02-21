#include <xaml/ui/controls/check_box.hpp>

using namespace std;

namespace xaml
{
    void check_box::__draw(rectangle const& region)
    {
        button::__draw(region);
        NSMatrix* matrix = (NSMatrix*)get_handle();
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        [button setButtonType:NSButtonTypeSwitch];
        draw_checked();
    }

    void check_box::draw_checked()
    {
        NSMatrix* matrix = (NSMatrix*)get_handle();
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        button.state = m_is_checked ? NSControlStateValueOn : NSControlStateValueOff;
    }

    void check_box::__on_state_changed()
    {
        NSMatrix* matrix = (NSMatrix*)get_handle();
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        set_is_checked(button.state == NSControlStateValueOn);
    }
}
