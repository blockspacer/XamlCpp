#include <xaml/event.h>
#include <xaml/object.hpp>
#include <xaml/observable_vector.h>
#include <xaml/vector.hpp>
#include <xaml/xaml_ptr.hpp>

using namespace std;

struct xaml_observable_vector_impl : xaml_implement<xaml_observable_vector_impl, xaml_observable_vector, xaml_vector, xaml_vector_view, xaml_enumerable, xaml_object>
{
private:
    xaml_ptr<xaml_vector> m_vec;
    xaml_ptr<xaml_event> m_collection_changed;

    xaml_result on_collection_changed(xaml_vector_changed_action action, xaml_vector_view* new_items, size_t new_index, xaml_vector_view* old_items, size_t old_index) noexcept
    {
        xaml_ptr<xaml_vector_changed_args> args;
        XAML_RETURN_IF_FAILED(xaml_vector_changed_args_new(action, new_items, new_index, old_items, old_index, &args));
        return m_collection_changed->invoke(this, args.get());
    }

public:
    xaml_observable_vector_impl(xaml_ptr<xaml_vector>&& vec, xaml_ptr<xaml_event>&& ccevent) noexcept : m_vec(move(vec)), m_collection_changed(move(ccevent)) {}

    xaml_result XAML_CALL get_size(size_t* psize) const noexcept override
    {
        return m_vec->get_size(psize);
    }

    xaml_result XAML_CALL get_at(size_t index, xaml_object** ptr) const noexcept override
    {
        return m_vec->get_at(index, ptr);
    }

    xaml_result XAML_CALL set_at(size_t index, xaml_object* obj) noexcept override
    {
        xaml_ptr<xaml_object> old_item;
        XAML_RETURN_IF_FAILED(get_at(index, &old_item));
        XAML_RETURN_IF_FAILED(m_vec->set_at(index, obj));
        xaml_ptr<xaml_vector> old_items, new_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ old_item }, &old_items));
        XAML_RETURN_IF_FAILED(xaml_vector_new({ obj }, &new_items));
        return on_collection_changed(xaml_vector_changed_replace, new_items.get(), index, old_items.get(), index);
    }

    xaml_result XAML_CALL append(xaml_object* obj) noexcept override
    {
        size_t index;
        XAML_RETURN_IF_FAILED(m_vec->get_size(&index));
        XAML_RETURN_IF_FAILED(m_vec->append(obj));
        xaml_ptr<xaml_vector> new_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ obj }, &new_items));
        return on_collection_changed(xaml_vector_changed_add, new_items.get(), index, nullptr, index);
    }

    xaml_result XAML_CALL remove_at(size_t index) noexcept override
    {
        xaml_ptr<xaml_object> old_item;
        XAML_RETURN_IF_FAILED(get_at(index, &old_item));
        XAML_RETURN_IF_FAILED(m_vec->remove_at(index));
        xaml_ptr<xaml_vector> old_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ old_item }, &old_items));
        return on_collection_changed(xaml_vector_changed_erase, nullptr, index, old_items.get(), index);
    }

    xaml_result XAML_CALL clear() noexcept override
    {
        xaml_ptr<xaml_vector> old_items = m_vec;
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_vec));
        return on_collection_changed(xaml_vector_changed_erase, nullptr, 0, old_items.get(), 0);
    }

    xaml_result XAML_CALL get_enumerator(xaml_enumerator** ptr) const noexcept override
    {
        return m_vec->get_enumerator(ptr);
    }

    xaml_result XAML_CALL add_vector_changed(xaml_callback* handler, size_t* ptoken) noexcept override
    {
        return m_collection_changed->add(handler, ptoken);
    }

    xaml_result XAML_CALL remove_vector_changed(size_t token) noexcept override
    {
        return m_collection_changed->remove(token);
    }
};

xaml_result xaml_observable_vector_new(xaml_observable_vector** ptr) noexcept
{
    xaml_ptr<xaml_vector> vec;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&vec));
    xaml_ptr<xaml_event> ccevent;
    XAML_RETURN_IF_FAILED(xaml_event_new(&ccevent));
    return xaml_object_new<xaml_observable_vector_impl>(ptr, move(vec), move(ccevent));
}

struct xaml_vector_changed_args_impl : xaml_implement<xaml_vector_changed_args_impl, xaml_vector_changed_args, xaml_object>
{
private:
    xaml_vector_changed_action m_action;
    xaml_ptr<xaml_vector_view> m_new_items;
    size_t m_new_index;
    xaml_ptr<xaml_vector_view> m_old_items;
    size_t m_old_index;

public:
    xaml_vector_changed_args_impl(xaml_vector_changed_action action, xaml_vector_view* new_items, size_t new_index, xaml_vector_view* old_items, size_t old_index)
        : m_action(action), m_new_items(new_items), m_new_index(new_index), m_old_items(old_items), m_old_index(old_index) {}

    xaml_result XAML_CALL get_action(xaml_vector_changed_action* paction) const noexcept override
    {
        *paction = m_action;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_new_items(xaml_vector_view** ptr) const noexcept override
    {
        m_new_items->add_ref();
        *ptr = m_new_items.get();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_new_index(size_t* pindex) const noexcept override
    {
        *pindex = m_new_index;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_old_items(xaml_vector_view** ptr) const noexcept override
    {
        m_old_items->add_ref();
        *ptr = m_old_items.get();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_old_index(size_t* pindex) const noexcept override
    {
        *pindex = m_old_index;
        return XAML_S_OK;
    }
};

xaml_result xaml_vector_changed_args_new(xaml_vector_changed_action action, xaml_vector_view* new_items, size_t new_index, xaml_vector_view* old_items, size_t old_index, xaml_vector_changed_args** ptr) noexcept
{
    return xaml_object_new<xaml_vector_changed_args_impl>(ptr, action, new_items, new_index, old_items, old_index);
}
