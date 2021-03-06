#include <assert.h>
#include <helper.h>
#include <stdio.h>
#include <xaml/observable_vector.h>
#include <xaml/string.h>
#include <xaml/vector.h>

xaml_result XAML_CALL print_string(xaml_string* str)
{
    char const* data;
    XAML_RETURN_IF_FAILED(str->vtbl->get_data(str, &data));
    int32_t length;
    XAML_RETURN_IF_FAILED(str->vtbl->get_length(str, &length));
    printf(U("%*s "), length, data);
    return XAML_S_OK;
}

xaml_result XAML_CALL observable_vector_changed_callback(xaml_object* sender, xaml_vector_changed_args_1__xaml_string* e)
{
    (void)sender;
    xaml_result hr;
    xaml_vector_changed_action action;
    XAML_GOTO_IF_FAILED(e->vtbl->get_action(e, &action), exit);
    switch (action)
    {
    case xaml_vector_changed_add:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), exit);
        printf(U("Add item at %d: "), index);
        xaml_vector_view_1__xaml_string* new_items;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_items(e, &new_items), exit);
        int32_t size;
        XAML_GOTO_IF_FAILED(new_items->vtbl->get_size(new_items, &size), clean_new_items_1);
        for (int32_t i = 0; i < size; i++)
        {
            xaml_string* str;
            XAML_GOTO_IF_FAILED(new_items->vtbl->get_at(new_items, i, &str), clean_new_items_1);
            XAML_GOTO_IF_FAILED(print_string(str), clean_str_1);
            str->vtbl->release(str);
            continue;
        clean_str_1:
            str->vtbl->release(str);
            goto clean_new_items_1;
        }
        printf(U("\n"));
        new_items->vtbl->release(new_items);
        break;
    clean_new_items_1:
        new_items->vtbl->release(new_items);
        goto exit;
    }
    case xaml_vector_changed_erase:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), exit);
        printf(U("Erase item at %d: "), index);
        xaml_vector_view_1__xaml_string* old_items;
        XAML_GOTO_IF_FAILED(e->vtbl->get_old_items(e, &old_items), exit);
        int32_t size;
        XAML_GOTO_IF_FAILED(old_items->vtbl->get_size(old_items, &size), clean_old_items_2);
        for (int32_t i = 0; i < size; i++)
        {
            xaml_string* str;
            XAML_GOTO_IF_FAILED(old_items->vtbl->get_at(old_items, i, &str), clean_old_items_2);
            XAML_GOTO_IF_FAILED(print_string(str), clean_str_2);
            str->vtbl->release(str);
            continue;
        clean_str_2:
            str->vtbl->release(str);
            goto clean_old_items_2;
        }
        printf(U("\n"));
        old_items->vtbl->release(old_items);
        break;
    clean_old_items_2:
        old_items->vtbl->release(old_items);
        goto exit;
    }
    case xaml_vector_changed_move:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), exit);
        printf(U("Move item at %d\n"), index);
        break;
    }
    case xaml_vector_changed_replace:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), exit);
        printf(U("Replace item at %d\n"), index);
        break;
    }
    case xaml_vector_changed_reset:
        printf(U("Reset.\n"));
        break;
    }
exit:
    return hr;
}

xaml_result main_impl()
{
    xaml_result hr;
    xaml_string* str;
    XAML_GOTO_IF_FAILED(xaml_string_new_view(U("Hello world!"), &str), exit);
    xaml_observable_vector_1__xaml_string* vec;
    XAML_GOTO_IF_FAILED(xaml_observable_vector_1__xaml_string_new(&vec), clean_str);
    xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_string* callback;
    XAML_GOTO_IF_FAILED(xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_string_new(observable_vector_changed_callback, &callback), clean_vec);
    int32_t token;
    XAML_GOTO_IF_FAILED(vec->vtbl->add_vector_changed(vec, callback, &token), clean_callback);
    XAML_GOTO_IF_FAILED(vec->vtbl->append(vec, str), clean_callback);
    XAML_GOTO_IF_FAILED(vec->vtbl->remove_at(vec, 0), clean_callback);

    xaml_map_2__int32_t__xaml_object* map;
    XAML_GOTO_IF_FAILED(xaml_map_2__int32_t__xaml_object_new(&map), clean_callback);
    XAML_GOTO_IF_FAILED(map->vtbl->insert(map, 1, (xaml_object*)str, NULL), clean_map);
    XAML_GOTO_IF_FAILED(map->vtbl->insert(map, 2, (xaml_object*)vec, NULL), clean_map);
    xaml_object* obj1;
    XAML_GOTO_IF_FAILED(map->vtbl->lookup(map, 1, &obj1), clean_map);
    xaml_string* str1;
    XAML_GOTO_IF_FAILED(obj1->vtbl->query(obj1, &xaml_guid_xaml_string, (void**)&str1), clean_obj1);
    XAML_GOTO_IF_FAILED(print_string(str1), clean_str1);
clean_str1:
    str1->vtbl->release(str1);
clean_obj1:
    obj1->vtbl->release(obj1);
clean_map:
    map->vtbl->release(map);

clean_callback:
    callback->vtbl->release(callback);
clean_vec:
    vec->vtbl->release(vec);
clean_str:
    str->vtbl->release(str);
exit:
    return hr;
}

int main()
{
    XAML_ASSERT_SUCCEEDED(main_impl());
}
