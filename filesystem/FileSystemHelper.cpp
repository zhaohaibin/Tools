//
// Created by zhaohaibin on 1/27/18.
//

#include "FileSystemHelper.h"

FileSystemHelper::FileSystemHelper()
{
}

FileSystemHelper::~FileSystemHelper()
{
}

bool FileSystemHelper::CreateDirectorys(const filesystem::path& dir, string& error)
{
    try
    {
        return boost::filesystem::create_directories(dir);
    }catch (std::exception& e)
    {
        error.append(e.what());
        return false;
    }

}

string FileSystemHelper::GetHomePath()
{
    string home(getenv("HOME"));
    return home;
}