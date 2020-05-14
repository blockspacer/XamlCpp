#include <filesystem>
#include <iostream>
#include <options.h>
#include <unordered_map>
#include <xaml/cmdline/deserializer.h>
#include <xaml/cmdline/option.h>
#include <xaml/result_handler.h>
#include <xaml/version.h>

#ifdef UNICODE
#define _tmain wmain
#define _tcout ::std::wcout
#define _tcerr ::std::wcerr
#else
#define _tmain main
#define _tcout ::std::cout
#define _tcerr ::std::cerr
#endif // UNICODE

using namespace std;
using namespace std::filesystem;

xaml_std_string_t get_type_name(xaml_ptr<xaml_meta_context> const& ctx, xaml_guid const& type)
{
    xaml_ptr<xaml_reflection_info> info;
    if (XAML_SUCCEEDED(ctx->get_type(type, &info)))
    {
        xaml_ptr<xaml_string> name;
        XAML_THROW_IF_FAILED(info->get_name(&name));
        return to_string_t(name);
    }
    else
    {
        xaml_ptr<xaml_string> name;
        if (XAML_SUCCEEDED(ctx->get_basic_type(type, &name)))
        {
            return to_string_t(name);
        }
        else
        {
            return U("Unknown-type");
        }
    }
}

int _tmain(int argc, xaml_char_t** argv)
{
    xaml_ptr<xaml_detector_options> options;
    xaml_ptr<xaml_meta_context> cmdline_ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&cmdline_ctx));
    XAML_THROW_IF_FAILED(xaml_detector_options_register(cmdline_ctx.get()));
    xaml_ptr<xaml_reflection_info> info;
    XAML_THROW_IF_FAILED(cmdline_ctx->get_type<xaml_detector_options>(&info));
    xaml_ptr<xaml_type_info> t;
    XAML_THROW_IF_FAILED(info->query(&t));
    xaml_ptr<xaml_cmdline_options> opts;

    XAML_THROW_IF_FAILED(xaml_cmdline_parse_argv(t.get(), argc, argv, &opts));
    xaml_ptr<xaml_object> obj;
    XAML_THROW_IF_FAILED(xaml_cmdline_deserialize(t.get(), opts.get(), &obj));
    XAML_THROW_IF_FAILED(obj->query(&options));

    bool verbose;
    XAML_THROW_IF_FAILED(options->get_verbose(&verbose));

    if (verbose)
    {
        int32_t token;
        XAML_THROW_IF_FAILED(xaml_result_handler_add(
            [](xaml_result hr, xaml_char_t const* msg) {
                _tcerr << U("0x") << hex << hr << U(": ") << msg << endl;
            },
            &token));
    }

    path exe{ argv[0] };
    bool no_logo;
    XAML_THROW_IF_FAILED(options->get_no_logo(&no_logo));
    if (!no_logo)
    {
        _tcout << exe.filename().string<xaml_char_t>() << U(" ") XAML_VERSION;
#ifdef XAML_COMMIT_HASH
        constexpr xaml_std_string_view_t hash{ U("") XAML_COMMIT_HASH };
        _tcout << U('-') << hash.substr(0, 8);
#endif // XAML_COMMIT_HASH
        _tcout << U("\nCopyright (c) 2019-2020 Berrysoft\n") << endl;
    }

    bool help;
    XAML_THROW_IF_FAILED(options->get_help(&help));
    if (help || argc <= 1)
    {
        xaml_ptr<xaml_cmdline_option> opt;
        XAML_THROW_IF_FAILED(t->get_attribute(&opt));
        XAML_THROW_IF_FAILED(xaml_cmdline_option_print(_tcout, opt.get()));
        return 1;
    }

    xaml_ptr<xaml_string> path_str;
    XAML_THROW_IF_FAILED(options->get_path(&path_str));

    xaml_ptr<xaml_module> module;
    XAML_THROW_IF_FAILED(xaml_module_new(&module));
    XAML_THROW_IF_FAILED(module->open(path_str.get()));

    xaml_ptr<xaml_meta_context> ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&ctx));

    bool recursive;
    XAML_THROW_IF_FAILED(options->get_recursive(&recursive));
    if (recursive)
        XAML_THROW_IF_FAILED(ctx->add_module_recursive(module.get()));
    else
        XAML_THROW_IF_FAILED(ctx->add_module(module.get()));

    xaml_result (*pget_version)(xaml_version*);
    XAML_THROW_IF_FAILED(module->get_method("xaml_module_version", (void**)&pget_version));

    xaml_version ver;
    XAML_THROW_IF_FAILED(pget_version(&ver));

    _tcout << U("Module ") << quoted(to_string_view_t(path_str)) << U(" (") << ver << U(')') << endl;

    xaml_ptr<xaml_map_view> types;
    XAML_THROW_IF_FAILED(ctx->get_types(&types));

    for (auto item : types)
    {
        xaml_ptr<xaml_key_value_pair> pair = item.query<xaml_key_value_pair>();
        xaml_ptr<xaml_object> second;
        XAML_THROW_IF_FAILED(pair->get_value(&second));
        xaml_ptr<xaml_reflection_info> info = second.query<xaml_reflection_info>();
        xaml_ptr<xaml_string> name;
        XAML_THROW_IF_FAILED(info->get_name(&name));
        xaml_ptr<xaml_string> include_file;
        XAML_THROW_IF_FAILED(info->get_include_file(&include_file));
        if (auto t = info.query<xaml_type_info>())
        {
            _tcout << U("class ") << to_string_view_t(name) << endl;
            _tcout << U("  included in <") << to_string_view_t(include_file) << U('>') << endl;
            {
                xaml_ptr<xaml_map_view> props;
                XAML_THROW_IF_FAILED(t->get_properties(&props));
                for (auto item2 : props)
                {
                    xaml_ptr<xaml_key_value_pair> pair = item2.query<xaml_key_value_pair>();
                    xaml_ptr<xaml_object> key;
                    XAML_THROW_IF_FAILED(pair->get_key(&key));
                    xaml_ptr<xaml_object> value;
                    XAML_THROW_IF_FAILED(pair->get_value(&value));
                    xaml_ptr<xaml_property_info> info = value.query<xaml_property_info>();
                    xaml_guid type;
                    XAML_THROW_IF_FAILED(info->get_type(&type));
                    _tcout << U("    P: ") << to_string_view_t(key.query<xaml_string>()) << U('\t') << get_type_name(ctx, type) << endl;
                }
            }
            {
                xaml_ptr<xaml_map_view> props;
                XAML_THROW_IF_FAILED(t->get_collection_properties(&props));
                for (auto item2 : props)
                {
                    xaml_ptr<xaml_key_value_pair> pair = item2.query<xaml_key_value_pair>();
                    xaml_ptr<xaml_object> key;
                    XAML_THROW_IF_FAILED(pair->get_key(&key));
                    xaml_ptr<xaml_object> value;
                    XAML_THROW_IF_FAILED(pair->get_value(&value));
                    xaml_ptr<xaml_collection_property_info> info = value.query<xaml_collection_property_info>();
                    xaml_guid type;
                    XAML_THROW_IF_FAILED(info->get_type(&type));
                    _tcout << U("    C: ") << to_string_view_t(key.query<xaml_string>()) << U('\t') << get_type_name(ctx, type) << endl;
                }
            }
            {
                xaml_ptr<xaml_map_view> props;
                XAML_THROW_IF_FAILED(t->get_events(&props));
                for (auto item2 : props)
                {
                    xaml_ptr<xaml_key_value_pair> pair = item2.query<xaml_key_value_pair>();
                    xaml_ptr<xaml_object> key;
                    XAML_THROW_IF_FAILED(pair->get_key(&key));
                    _tcout << U("    E: ") << to_string_view_t(key.query<xaml_string>()) << endl;
                }
            }
        }
        else if (auto t = info.query<xaml_enum_info>())
        {
            _tcout << U("enum ") << to_string_view_t(name) << endl;
            _tcout << U("  included in <") << to_string_view_t(include_file) << U('>') << endl;
            xaml_ptr<xaml_map_view> values;
            XAML_THROW_IF_FAILED(t->get_values(&values));
            for (auto item2 : values)
            {
                xaml_ptr<xaml_key_value_pair> pair = item2.query<xaml_key_value_pair>();
                xaml_ptr<xaml_object> key;
                XAML_THROW_IF_FAILED(pair->get_key(&key));
                xaml_ptr<xaml_object> value;
                XAML_THROW_IF_FAILED(pair->get_value(&value));
                xaml_ptr<xaml_box> box = value.query<xaml_box>();
                int const* pvalue;
                XAML_THROW_IF_FAILED(box->get_data((void const**)&pvalue));
                _tcout << U("    ") << to_string_view_t(key.query<xaml_string>()) << U(" = ") << *pvalue << endl;
            }
        }
    }

    return 0;
}