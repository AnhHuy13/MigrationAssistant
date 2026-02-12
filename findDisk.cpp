#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <blkid/blkid.h>

struct partition
{
    std::string name;
    std::string type;
    std::string label;
};
std::vector<partition> findWin()
{
    std::vector<partition> ntfs_list;
    blkid_cache cache;

    if (blkid_get_cache(&cache, NULL) < 0)
    {
        return ntfs_list;
    }
    blkid_probe_all(cache); // scan partition

    blkid_dev_iterate iter = blkid_dev_iterate_begin(cache);
    blkid_dev dev;
    while (blkid_dev_next(iter, &dev) == 0)
    {
        const char *devName = blkid_dev_devname(dev);
        const char *type = blkid_get_tag_value(cache, "TYPE", devName);
        const char *label = blkid_get_tag_value(cache, "LABEL", devName);
        if (type && strstr(type, "ntfs")) {
            ntfs_list.push_back({devName, type, label ? label : "No label"});
        }
    };

    // free memory
    blkid_dev_iterate_end(iter);
    blkid_put_cache(cache);
    return ntfs_list;
}
/*int main()
{
    auto drives = findWin();
    for (const auto &d : drives)
    {
        std::cout << "Device: " << d.name << "|Type: " << d.type << " | Label: " << d.label << std::endl;
    }
    return 0;
}*/
