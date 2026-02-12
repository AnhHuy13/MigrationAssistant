#include <sys/mount.h>
#include <iostream>
#include <cstdio>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <limits.h>

bool checkIsTheUserSudo(){
    return geteuid() == 0;
}

bool checkIsTheDirBlacklist(const char* target){
    char realPath[PATH_MAX];

    if (!realpath(target, realPath)){return true;}

    const char *blacklist[] = {"/","/etc","/bin","/sbin","/lib","/lib64","/usr","/boot","/dev",
        "/proc",
        "/sys",
        "/run"
    };

    for (int i = 0; i < sizeof(blacklist)/sizeof(blacklist[0]); i++) {
        int len = strlen(blacklist[i]);

        if (strncmp(realPath, blacklist[i], len) == 0 &&
            (realPath[len] == '/' || realPath[len] == '\0')) {
            return true;
        }
    }

    if (strcmp(realPath, "/home") == 0){return true;}

    return false;
}

void mountWindowsPartition(const char* partitionPath, const char* mountDir,const char* fileSystem) {
    if (checkIsTheDirBlacklist(mountDir)){
        std::cerr << "dangerous!";
        exit(1);
    }

    sync();
    int statusMkdir = mkdir(mountDir,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if (statusMkdir == -1){
        if (errno == EEXIST) {
            std::cout << "Directory was exist!";
        }
        else{
            std::perror("Error when make new directory!");
        }
    }
    else{
        std::cout << "sucessfully created dir";
    };
    int statusMount = mount(partitionPath, mountDir,fileSystem,MS_RDONLY,NULL);
    if (statusMount == -1){
        if (errno == EBUSY){
            std::cerr << "busy";
        }
        else if (errno == ENODEV){
            std::cerr << "isn't supported or existed";
        }
        else {
            std::cerr << "error! : " << std::strerror(errno);
        }
    }
    else{
        std::cout << "Mounted successfully";
    };
}
/*int main() {
    if (!checkIsTheUserSudo()){
        std::cerr << "Run this at sudo!";
        return 1;
    }
    else{
        mountWindowsPartition("/dev/nvme0n1p3","/tmp/test","ntfs3"); //test truoc
    }
}*/
