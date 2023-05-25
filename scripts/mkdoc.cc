#include "lib/globals.h"
#include "lib/proto.h"
#include "lib/flags.h"
#include <fmt/format.h>

extern const std::map<std::string, const ConfigProto*> formats;

static const ConfigProto& findConfig(std::string name)
{
    return *formats.at(name);
}

static void addExample(std::vector<std::string>& examples, bool write, std::string name, ConfigProto config, const OptionProto* format)
{
    std::string r;

    if (format)
        config.MergeFrom(format->config());

    if (write && config.has_encoder())
        r += fmt::format("fluxengine write");
    else if (!write && config.has_decoder())
        r += fmt::format("fluxengine read");
    else
        return;

    r += fmt::format(" {}", name);
    if (format)
        r += fmt::format(" --{}", format->name());

    std::string filename = write ?
        config.image_reader().filename() : 
        config.image_writer().filename();
    if (filename.empty())
        filename = fmt::format("{}.img", name);

    if (write)
        r += fmt::format(" -d drive:0 -i {}", filename);
    else
        r += fmt::format(" -s drive:0 -o {}", filename);

    examples.push_back(r);
}

int main(int argc, const char* argv[])
{
    auto config = findConfig(argv[1]);

    fmt::print("{}\n====\n", argv[1]);
    fmt::print("## {}\n", config.comment());
    fmt::print("<!-- This file is automatically generated. Do not edit. -->\n\n");

    const auto& documentation = config.documentation();
    auto it = documentation.begin();
    if (it != documentation.end())
    {
        fmt::print("{}\n", *it++);

        fmt::print("## Options\n\n");

        const OptionGroupProto* formatGroup = nullptr;
        if (!config.option().empty() || !config.option_group().empty())
        {
            for (const auto& option_group : config.option_group())
            {
                auto groupName = option_group.comment();
                if (groupName == "$formats")
                {
                    groupName = "Format variants";
                    formatGroup = &option_group;
                }

                fmt::print("  - {}:\n", groupName);
                for (const auto& option : option_group.option())
                    fmt::print(
                        "      - `{}`: {}\n", option.name(), option.comment());
            }

            for (const auto& option : config.option())
                fmt::print("  - `{}`: {}\n", option.name(), option.comment());
        }
        else
            fmt::print("(no options)\n");
        fmt::print("\n");

        fmt::print("## Examples\n\n");

        std::vector<std::string> examples;
        if (formatGroup)
        {
            for (const auto& option : formatGroup->option())
                addExample(examples, false, argv[1], config, &option);
        }
        else
            addExample(examples, false, argv[1], config, nullptr);

        if (!examples.empty())
        {
            fmt::print("To read:\n\n");
            for (const std::string& e : examples)
                fmt::print("  - `{}`\n", e);
            fmt::print("\n");
        }

        examples.clear();
        if (formatGroup)
        {
            for (const auto& option : formatGroup->option())
                addExample(examples, true, argv[1], config, &option);
        }
        else
            addExample(examples, true, argv[1], config, nullptr);

        if (!examples.empty())
        {
            fmt::print("To write:\n\n");
            for (const std::string& e : examples)
                fmt::print("  - `{}`\n", e);
            fmt::print("\n");
        }

        while (it != documentation.end())
            fmt::print("{}\n", *it++);
    }
    else
        fmt::print("(This format has no documentation. Please file a bug.)\n");

    return 0;
}
