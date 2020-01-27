#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <xaml/compiler.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/parser.hpp>

#ifndef __APPLE__
#include <filesystem>
#else
#include <boost/filesystem.hpp>
#endif // !__APPLE__

using namespace std;
using namespace xaml;
using namespace boost::program_options;

#ifndef __APPLE__
using namespace std::filesystem;
#else
using boost::filesystem::path;
using boost::filesystem::directory_iterator;
#endif // !__APPLE__

int main(int argc, char** argv)
{
    options_description desc{ "Allowed options" };
    desc.add_options()(
        "help,h", "Print help message")(
        "input-file,i", value<string>(), "Input XAML file")(
        "output-file,o", value<string>(), "Output C++ file")(
        "library-path,L", value<vector<string>>(), "Search library path")(
        "fake,f", "Generate deserialize code")(
        "no-logo", "Cancellation to show copyright infomation");
    positional_options_description p;
    p.add("input-file", -1);

    variables_map vm;
    store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    notify(vm);

    path exe{ argv[0] };
    if (!vm.count("no-logo"))
    {
        cout << exe.filename().string() << " 1.0.0" << endl
             << "Copyright (c) 2019-2020 Berrysoft" << endl
             << endl;
    }

    if (vm.empty() || vm.count("help"))
    {
        cout << desc << endl;
        return 1;
    }

    if (vm.count("input-file"))
    {
        string inf = vm["input-file"].as<string>();
        path ouf_path = vm.count("output-file") ? vm["output-file"].as<string>() : inf + ".g.cpp";
        auto lib_dirs = vm.count("library-path") ? vm["library-path"].as<vector<string>>() : vector<string>{ exe.parent_path().string() };
        for (auto& dir : lib_dirs)
        {
            for (auto& en : directory_iterator{ dir })
            {
                auto p = en.path();
                if (p.has_extension() && p.extension().native() == module_extension)
                {
                    add_compiler_module(p.string());
                }
            }
        }
        init_parser();
        parser p{ inf };
        if (p.is_open())
        {
            auto node = p.parse();
            compiler c{};
            ofstream stream{ ouf_path.native() };
            if (vm.count("fake"))
            {
                c.compile_fake(stream, node, inf);
            }
            else
            {
                c.compile(stream, node);
            }
        }
        else
        {
            cerr << "Cannot open " << inf << endl;
            return 1;
        }
    }
    else
    {
        cerr << "Input file must be specified" << endl;
        return 1;
    }
}