#include <win/webview_edge.hpp>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::UI;
using namespace Windows::Web::UI::Interop;

namespace xaml
{
    void webview_edge::create_async(HWND parent, rectangle const& rect, function<void()>&& callback)
    {
        try
        {
            WebViewControlProcess process;
            auto task = process.CreateWebViewControlAsync((int64_t)parent, { (float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height });
            task.Completed([this, callback](IAsyncOperation<WebViewControl> operation, AsyncStatus status) {
                if (status == AsyncStatus::Completed)
                {
                    m_view = operation.GetResults();
                    m_view.NavigationCompleted([this](IWebViewControl const&, WebViewControlNavigationCompletedEventArgs const& args) {
                        invoke_navigated(args.Uri().AbsoluteUri());
                    });
                }
                callback();
            });
        }
        catch (hresult_error const&)
        {
            m_view = nullptr;
            callback();
        }
    }

    void webview_edge::navigate(string_view_t uri)
    {
        m_view.Navigate(Uri{ uri });
    }

    void webview_edge::set_location(point p)
    {
        auto bounds = m_view.Bounds();
        m_view.Bounds({ (float)p.x, (float)p.y, bounds.Width, bounds.Height });
    }

    void webview_edge::set_size(size s)
    {
        auto bounds = m_view.Bounds();
        m_view.Bounds({ bounds.X, bounds.Y, (float)s.width, (float)s.height });
    }

    void webview_edge::set_rect(rectangle const& rect)
    {
        m_view.Bounds({ (float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height });
    }
} // namespace xaml