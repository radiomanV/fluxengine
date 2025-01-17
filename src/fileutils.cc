#include "globals.h"
#include "flags.h"
#include "fluxmap.h"
#include "sector.h"
#include "proto.h"
#include "readerwriter.h"
#include "lib/fluxsource/fluxsource.h"
#include "lib/fluxsink/fluxsink.h"
#include "lib/imagereader/imagereader.h"
#include "lib/imagewriter/imagewriter.h"
#include "fluxengine.h"
#include "lib/vfs/sectorinterface.h"
#include "lib/vfs/vfs.h"
#include <google/protobuf/text_format.h>
#include <fstream>

FlagGroup fileFlags;

static StringFlag image({"-i", "--image"},
    "image to work on",
    "",
    [](const auto& value)
    {
        globalConfig().setImageReader(value);
        globalConfig().setImageWriter(value);
    });

static StringFlag flux({"-f", "--flux"},
    "flux source to work on",
    "",
    [](const auto& value)
    {
        globalConfig().setFluxSource(value);
        globalConfig().setFluxSink(value);
    });
