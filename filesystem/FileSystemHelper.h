//
// Created by zhaohaibin on 1/27/18.
//

#ifndef IMAGEMATCHER_FILESYSTEMHELPER_H
#define IMAGEMATCHER_FILESYSTEMHELPER_H

#include <boost/filesystem.hpp>
#include <string>

using namespace boost;
using namespace std;

class FileSystemHelper
{

public:
    FileSystemHelper();
    ~FileSystemHelper();

public:
    template<typename T>
    static void GetFiles(const filesystem::path& dir, T& files, const string& extension)
    {
        filesystem::directory_iterator end_it;
        filesystem::directory_iterator it(dir);
        for (; it != end_it; ++it)
        {
            if (it->path().extension().generic_string() == extension)
                files.push_back(it->path().generic_string());
        }
    }

    static bool CreateDirectorys(const filesystem::path& dir, string& error);

    static string GetHomePath();

};


#endif //IMAGEMATCHER_FILESYSTEMHELPER_H
