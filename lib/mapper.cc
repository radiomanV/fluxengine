#include "globals.h"
#include "sector.h"
#include "image.h"
#include "fmt/format.h"
#include "logger.h"
#include "proto.h"
#include "mapper.h"
#include "flux.h"
#include "lib/mapper.pb.h"

typedef std::function<void(
    std::map<int, int>&, const SectorMappingProto::MappingProto&)>
    insertercb_t;

static void getTrackFormat(const SectorMappingProto& proto,
    SectorMappingProto::TrackdataProto& trackdata,
    unsigned track,
    unsigned side)
{
    trackdata.Clear();
    for (const SectorMappingProto::TrackdataProto& f : proto.trackdata())
    {
        if (f.has_track() && f.has_up_to_track() &&
            ((track < f.track()) || (track > f.up_to_track())))
            continue;
        if (f.has_track() && !f.has_up_to_track() && (track != f.track()))
            continue;
        if (f.has_side() && (f.side() != side))
            continue;

        trackdata.MergeFrom(f);
    }
}

static std::unique_ptr<Image> remapImpl(const Image& source,
    const SectorMappingProto& mapping,
    insertercb_t inserter_cb)
{
    typedef std::pair<int, int> tracksidekey_t;
    std::map<tracksidekey_t, std::map<int, int>> cache;

    auto getTrackdata =
        [&](const tracksidekey_t& key) -> const std::map<int, int>&
    {
        auto it = cache.find(key);
        if (it != cache.end())
            return it->second;

        SectorMappingProto::TrackdataProto trackdata;
        getTrackFormat(mapping, trackdata, key.first, key.second);

        auto& map = cache[key];
        for (const auto mappingsit : trackdata.mapping())
            inserter_cb(map, mappingsit);

        return map;
    };

    std::set<std::shared_ptr<const Sector>> destSectors;
    for (const auto& sector : source)
    {
        tracksidekey_t key = {sector->logicalTrack, sector->logicalSide};
        const auto& trackdata = getTrackdata(key);
        if (trackdata.empty())
            destSectors.insert(sector);
        else
        {
            auto it = trackdata.find(sector->logicalSector);
            if (it == trackdata.end())
                Error() << fmt::format(
                    "mapping requested but mapping table has no entry for "
                    "sector {}",
                    sector->logicalSector);

            auto newSector = std::make_shared<Sector>(*sector);
            newSector->logicalSector = it->second;
            destSectors.insert(newSector);
        }
    }

    return std::make_unique<Image>(destSectors);
}

std::unique_ptr<const Image> Mapper::remapSectorsPhysicalToLogical(
    const Image& source, const SectorMappingProto& mapping)
{
    Logger() << "remapping sectors from physical IDs to logical IDs";
    return remapImpl(source,
        mapping,
        [](auto& map, const auto& pair)
        {
            map.insert({pair.physical(), pair.logical()});
        });
}

std::unique_ptr<const Image> Mapper::remapSectorsLogicalToPhysical(
    const Image& source, const SectorMappingProto& mapping)
{
    Logger() << "remapping sectors from logical IDs to physical IDs";
    return remapImpl(source,
        mapping,
        [](auto& map, const auto& pair)
        {
            map.insert({pair.logical(), pair.physical()});
        });
}

unsigned Mapper::remapCylinderPhysicalToLogical(unsigned pcylinder)
{
    return (pcylinder - config.drive().head_bias()) /
           config.drive().head_width();
}

unsigned Mapper::remapCylinderLogicalToPhysical(unsigned lcylinder)
{
	Error() << "not working yet";
    return config.drive().head_bias() + lcylinder * config.drive().head_width();
}

std::set<Location> Mapper::computeLocations()
{
    std::set<Location> locations;

    unsigned track_step =
		(config.tpi() == 0) ? 1 : (config.drive().tpi() / config.tpi());

    if (track_step == 0)
        Error()
            << "this drive can't write this image, because the head is too big";

    for (unsigned logicalCylinder : iterate(config.cylinders()))
    {
        for (unsigned head : iterate(config.heads()))
        {
			unsigned physicalCylinder = config.drive().head_bias() + logicalCylinder * track_step;

            locations.insert({.physicalCylinder = physicalCylinder,
                .logicalCylinder = logicalCylinder,
                .head = head,
                .groupSize = track_step});
        }
    }

    return locations;
}
