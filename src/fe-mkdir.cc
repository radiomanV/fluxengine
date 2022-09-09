#include "globals.h"
#include "flags.h"
#include "proto.h"
#include "fmt/format.h"
#include "fluxengine.h"
#include "lib/vfs/vfs.h"
#include "lib/utils.h"
#include "src/fileutils.h"
#include <google/protobuf/text_format.h>
#include <fstream>

static FlagGroup flags({&fileFlags});

static StringFlag filename({"-p", "--path"}, "directory to create", "");

int mainMkDir(int argc, const char* argv[])
{
    if (argc == 1)
        showProfiles("mkdir", formats);
    flags.parseFlagsWithConfigFiles(argc, argv, formats);

    try
    {
        auto filesystem = Filesystem::createFilesystemFromConfig();

        Path path(filename);
        if (path.size() == 0)
            Error() << "filename missing";

        filesystem->createDirectory(path);
        filesystem->flushChanges();
    }
    catch (const FilesystemException& e)
    {
        Error() << e.message;
    }

    return 0;
}
