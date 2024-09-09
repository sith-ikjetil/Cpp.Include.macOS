///////////////////////////////////////////////////////////////////
//: Title        : itsoftware-macos-core.h
//: Product:     : Cpp.Include.macOS
//: Date         : 2021-09-21 
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of Cpp.Include.macOS.
//: Uses         : libuuid.a.
#pragma once
//
// #include
//
#include <string>
#include <memory>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <uuid/uuid.h>
#include <signal.h>
#include <thread>
#include <functional>
#include <CoreServices/CoreServices.h>
#include "itsoftware-macos.h"

//
// namespace
//
namespace ItSoftware::macOS::Core
{
    //
    // using
    //
    using std::make_unique;
    using std::string;
    using std::unique_ptr;
    using std::vector;
    using std::thread;
    using std::function;
    using ItSoftware::macOS::ItsString;

    //
    // #define
    //
    #define FILE_MONITOR_BUFFER_LENGTH (10 * (sizeof(inotify_event) + NAME_MAX + 1))

    //
    // struct: ItsTimer
    //
    struct ItsTimer
    {
        private:
        size_t m_start = 0;
        size_t m_end = 0;
        tms m_tms;
        bool m_isRunning = false;

        size_t GetTickCount()
        {
            struct timespec ts;
            if (clock_gettime(CLOCK_REALTIME,&ts) != 0)
            {
                return 0;
            }
            
            return (ts.tv_sec * 1'000'000'000) + ts.tv_nsec;
        }

        public:
        void Start()
        {
            if (!this->IsRunning())
            {
                this->m_start = this->GetTickCount();
                this->m_end = m_start;
                this->m_isRunning = true;
            }
        }

        void Stop()
        {
            this->m_end = this->GetTickCount();
            this->m_isRunning = false;
        }

        bool IsRunning()
        {
            return this->m_isRunning;
        }

        size_t LapSeconds()
        {
            if (!this->IsRunning())
            {
                return 0;
            }

            return (size_t)((this->GetTickCount() - this->m_start) / 1'000'000'000);
        }

        size_t LapMilliseconds()
        {
            if (!this->IsRunning())
            {
                return 0;
            }

            return (size_t)((this->GetTickCount() - this->m_start) / 1'000'000);
        }

        size_t LapMicroseconds()
        {
            if (!this->IsRunning())
            {
                return 0;
            }

            return (size_t)((this->GetTickCount() - this->m_start) / 1'000);
        }

        size_t LapNanoseconds()
        {
            if (!this->IsRunning())
            {
                return 0;
            }
            return (this->GetTickCount() - this->m_start);
        }

        size_t GetSeconds()
        {
            if (this->IsRunning())
            {
                return 0;
            }

            return (size_t)((this->m_end - this->m_start) / 1'000'000'000);
        }

        size_t GetMilliseconds()
        {
            if (this->IsRunning())
            {
                return 0;
            }

            return (size_t)((this->m_end - this->m_start) / 1'000'000);
        }

        size_t GetMicroseconds()
        {
            if (this->IsRunning())
            {
                return 0;
            }

            return (size_t)((this->m_end - this->m_start) / 1'000);
        }

        size_t GetNanoseconds()
        {
            if (this->IsRunning())
            {
                return 0;
            }
            return (this->m_end - this->m_start);
        }
    };

        //
    // struct: ItsGuidFormat
    //
    // (i): Container for premade Guid format strings.
    //
    struct ItsGuidFormat {
        inline static constexpr const char* MicrosoftRegistryFormat{"{%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"};
        inline static constexpr const char* MicrosoftRegistryFormatStripped{ "%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X" };
        inline static constexpr const char* MicrosoftConstFormat{ "{ 0x%lx, 0x%x, 0x%x, { 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x } }" };
        inline static constexpr const char* MicrosoftCompactFormat{ "%08lX%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X" };
        inline static constexpr const char* MicrosoftPrefixedCompactFormat{ "GUID%08lX%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X" };
    };

    //
    // struct: ItsGuid
    //
    // (i): Wrapper for Guid generation and rendering.
    //
    struct ItsGuid
    {
    public:
        static string CreateGuid()
        {
            uuid_t guid{0};
            uuid_generate(guid);
            return ItsGuid::ToString(guid);
        }
        static bool CreateGuid(uuid_t pGuid)
        {
            uuid_generate_random(pGuid);
            return true;
        }
        static string ToString(uuid_t guid) {
            return ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftRegistryFormat, true);
        }
        static string ToString(uuid_t guid, string format, bool isMicrosoftGuidFormat) {
            char szBuffer[100];
            memset(szBuffer, 0, 100);

            if (isMicrosoftGuidFormat) {
                sprintf(szBuffer, format.c_str(),
                    *reinterpret_cast<uint32_t*>(&guid[0]), *reinterpret_cast<uint16_t*>(&guid[4]), *reinterpret_cast<uint16_t*>(&guid[6]),
                    guid[8], guid[9], guid[10], guid[11], guid[12], guid[13], guid[14], guid[15]);
            }
            else {
                sprintf(szBuffer, format.c_str(),
                    guid[0], guid[1], guid[2], guid[3], guid[4], guid[5], guid[6], guid[7],
                    guid[8], guid[9], guid[10], guid[11], guid[12], guid[13], guid[14], guid[15]);
            }
            return string(szBuffer);
        }
    };

    //
    // struct: ItsError
    //
    // (i): macOS error messages
    //
    struct ItsError
    {
    public:
        static string GetErrorDescription(int err)
        {
            return strerror(err);
        }
        static string GetLastErrorDescription()
        {
            return strerror(errno);
        }
    };

    //
    // struct: ItsDirectory
    //
    // (i): macOS directory routines.
    //
    struct ItsDirectory
    {
    public:
        static bool Exists(string dirname)
        {
            struct stat sb;

            if (stat(dirname.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
                return true;
            }

            return false;
        }
        static bool CreateDirectory(string path, int mode)
        {
            return (mkdir(path.c_str(), mode) == 0);
        }
        static bool RemoveDirectory(string path)
        {
            return (rmdir(path.c_str()) == 0);
        }
        static bool SetCurrentDirectory(string path)
        {
            return (chdir(path.c_str()) == 0);
        }
        static vector<string> GetDirectories(string path) {
            if (path.size() == 0) {
                return vector<string>();
            }

            DIR *dir;
            struct dirent *entry;

            if ((dir = opendir(path.c_str())) == nullptr) {
                return vector<string>();
            }

            vector<string> directories;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type & DT_DIR) {
                    directories.push_back(entry->d_name);
                }
            }

            closedir(dir);
            
            return directories;
        }
        static vector<string> GetFiles(string path) {
            if (path.size() == 0) {
                return vector<string>();
            }

            DIR *dir;
            struct dirent *entry;

            if ((dir = opendir(path.c_str())) == nullptr) {
                return vector<string>();
            }

            vector<string> files;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type & DT_REG) {
                    files.push_back(entry->d_name);
                }
            }

            closedir(dir);
            
            return files;
        }
    };

    //
    // struct: unique_file_descriptor
    //
    struct unique_file_handle
    {
        private:
        int m_fd = -1;

        protected:
        public:
        explicit unique_file_handle()
        {
            this->m_fd = -1;
        }

        explicit unique_file_handle(int fd)
        {
            this->m_fd = fd;
        }

        ~unique_file_handle()
        {
            this->Close();
        }

        bool IsValid() const
        {
            return (this->m_fd >= 0);
        }

        bool IsInvalid() const
        {
            return (this->m_fd < 0);
        }

        void operator=(int fd)
        {
            this->Close();
            this->m_fd = fd;
        }

        operator int()
        {
            return this->m_fd;
        }

        bool Close()
        {
            if (this->IsInvalid())
            {
                return false;
            }
            close(this->m_fd);
            this->m_fd = -1;
            return true;
        }

        int p() const
        {
            return this->m_fd;
        }

        const int *GetAddressOf() const
        {
            return &this->m_fd;
        }
    };

    //
    // File IO Wrapper
    //
    class ItsFile
    {
        private:
        unique_file_handle m_fd;
        string m_filename;
        protected:
        public:
        ItsFile()
        {
        }

        //
        // GetFilename
        //
        string GetFilename()
        {
                return this->m_filename;
        }

        //
        // Method: OpenExisting
        //
        bool OpenExisting(string filename, string flags)
        {
            if (this->m_fd.IsValid())
            {
                return false;
            }

            if (filename.size() == 0)
            {
                return false;
            }

            if (flags.size() == 0)
            {
                return false;
            }

            if (!ItsFile::Exists(filename))
            {
                return false;
            }

            int i_flags = 0;
            if (flags.find("r") != string::npos &&
                flags.find("w") != string::npos)
            {
                i_flags |= O_RDWR;
            }
            else if (flags.find("r") != string::npos)
            {
                i_flags |= O_RDONLY;
            }
            else if (flags.find("w") != string::npos)
            {
                i_flags |= O_WRONLY;
            }
            else
            {
                return false;
            }

            i_flags |= O_CREAT;
            //i_flags |= O_LARGEFILE;

            if (flags.find("t") != string::npos)
            {
                i_flags |= O_TRUNC;
            }
            else if (flags.find("a") != string::npos)
            {
                i_flags |= O_APPEND;
            }

            this->m_fd = open(filename.c_str(), i_flags);
            if (this->m_fd.IsInvalid())
            {
                return false;
            }

            this->m_filename = filename;
            return true;
        }

        //
        // Method: OpenOrCreate
        //
        // (i) mode = "rwta" (read, write, trunc, append)
        //
        bool OpenOrCreate(string filename, string flags, int mode)
        {
            if (this->m_fd.IsValid())
            {
                return false;
            }

            if (filename.size() == 0)
            {
                return false;
            }

            if (flags.size() == 0)
            {
                return false;
            }

            int i_flags = 0;
            if (flags.find("r") != string::npos &&
                flags.find("w") != string::npos)
            {
                i_flags |= O_RDWR;
            }
            else if (flags.find("r") != string::npos)
            {
                i_flags |= O_RDONLY;
            }
            else if (flags.find("w") != string::npos)
            {
                i_flags |= O_WRONLY;
            }
            else
            {
                return false;
            }

            i_flags |= O_CREAT;
            //i_flags |= O_LARGEFILE;

            if (flags.find("t") != string::npos)
            {
                i_flags |= O_TRUNC;
            }
            else if (flags.find("a") != string::npos)
            {
                i_flags |= O_APPEND;
            }

            this->m_fd = open(filename.c_str(), i_flags, mode);
            if (this->m_fd.IsInvalid())
            {
                return false;
            }
            if (fchmod(this->m_fd, mode) != 0 )
            {
                this->Close();
                return false;
            }

            this->m_filename = filename;

            return true;
        }

        bool Write(const void *data, size_t bytesToWrite, size_t *bytesWritten)
        {
            if (this->IsInvalid())
            {
                return false;
            }

            if (bytesToWrite == 0)
            {
                return false;
            }

            *bytesWritten = write(this->m_fd.p(), data, bytesToWrite);

            return (*bytesWritten > 0);
        }

        bool Read(void *data, size_t bytesToRead, size_t *bytesRead)
        {
            if (this->IsInvalid())
            {
                return false;
            }

            if (bytesToRead == 0)
            {
                return false;
            }

            *bytesRead = read(this->m_fd.p(), data, bytesToRead);

            return true;
        }

        //
        // ReadAllText
        //
        bool ReadAllText(string& str)
        {
            if (this->IsInvalid())
            {
                return false;
            }

            this->SetPosFromBeg(0);

            std::stringstream ss;

            char data[2048];
            size_t bytesRead(0);
            while (this->Read((void*)data, 2048, &bytesRead) && bytesRead > 0)
            {
                ss.write(data, bytesRead);
            }

            str = ss.str();
            return true;
        }

        //
        // ReadAllTextLines
        //
        bool ReadAllTextLines(std::vector<string>& lines)
        {
            if (this->IsInvalid())
            {
                return false;
            }

            this->SetPosFromBeg(0);

            string str;
            if (!this->ReadAllText(str))
            {
                return false;
            }

            string split("\n");
            lines = ItsString::Split(str, split);
            return true;
        }

        bool SetPosFromBeg(off_t offset)
        {
            if (this->IsInvalid())
            {
                return false;
            }

            if (lseek(this->m_fd, offset, SEEK_SET) == -1)
            {
                return false;
            }

            return true;
        }

        bool SetPosFromEnd(off_t offset)
        {
            if (this->IsInvalid())
            {
                return false;
            }

            if (lseek(this->m_fd, offset, SEEK_END) == -1)
            {
                return false;
            }

            return true;
        }

        bool SetPosFromCur(off_t offset)
        {
            if (this->IsInvalid())
            {
                return false;
            }

            if (lseek(this->m_fd, offset, SEEK_CUR) == -1)
            {
                return false;
            }

            return true;
        }

        bool Close()
        {
            return this->m_fd.Close();
        }

        bool IsValid()
        {
            return this->m_fd.IsValid();
        }

        bool IsInvalid()
        {
            return this->m_fd.IsInvalid();
        }

        operator int()
        {
            return this->m_fd.p();
        }

        //
        // GetFileSize
        //
        static size_t GetFileSize(string filename)
        {
            struct stat statbuf;
            if ( stat(filename.c_str(), &statbuf) == -1 ) {
                return -1;
            }
            return statbuf.st_size;
        }

        static bool Delete(string filename)
        {
            return (!unlink(filename.c_str()));
        }

        static bool Exists(string filename)
        {
            struct stat sb;
            if(stat(filename.c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
                return true;
            }
            return false;
        }

        static bool Move(string sourceFilename, string targetFilename)
        {
            return (!rename(sourceFilename.c_str(), targetFilename.c_str()));
        }

        static bool Copy(string sourceFilename, string targetFilename, bool replaceIfExists)
        {
            if (!ItsFile::Exists(sourceFilename))
            {
                return false;
            }

            ItsFile source;
            source.OpenExisting(sourceFilename, "r");
            if (source.IsInvalid())
            {
                return false;
            }

            if (ItsFile::Exists(targetFilename) && !replaceIfExists)
            {
                return false;
            }

            ItsFile target;
            if (ItsFile::Exists(targetFilename))
            {
                target.OpenExisting(targetFilename, "w");
            }
            else
            {
                int mode = 0;
                ItsFile::GetMode(sourceFilename, &mode);
                target.OpenOrCreate(targetFilename, "wt", mode);
            }

            if (target.IsInvalid())
            {
                return false;
            }

            const int buff_size = 8192;
            unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(buff_size);
            size_t bytes_read{0};
            size_t bytes_written{0};
            while (source.Read(buffer.get(), buff_size, &bytes_read) && bytes_read > 0)
            {
                target.Write(buffer.get(), bytes_read, &bytes_written);
            }

            return true;
        }

        static bool GetMode(string filename, int *mode)
        {
            if (!ItsFile::Exists(filename))
            {
                return false;
            }

            struct stat stat1;
            if (stat(filename.c_str(), &stat1) != 0)
            {
                return false;
            }
            *mode = stat1.st_mode;
            return true;
        }

        static bool SetMode(string filename, int mode)
        {
            if (!ItsFile::Exists(filename))
            {
                return false;
            }

            struct stat stat1;
            if (chmod(filename.c_str(), mode) != 0)
            {
                return false;
            }
            return true;
        }


        static int CreateMode(string user, string group, string other)
        {
            int mode(0);

            if (user.size() > 0)
            {
                if (user.find('r',0) != string::npos) {
                    mode |= S_IRUSR;
                }
                if (user.find('w',0) != string::npos) {
                    mode |= S_IWUSR;
                }
                if (user.find('x',0) != string::npos) {
                    mode |= S_IXUSR;
                }
            }

            if (group.size() > 0)
            {
                    if (group.find('r',0) != string::npos) {
                        mode |= S_IRGRP;
                    }
                    if (group.find('w',0) != string::npos) {
                        mode |= S_IWGRP;
                    }
                    if (group.find('x',0) != string::npos) {
                        mode |= S_IXGRP;
                    }
            }

            if (other.size() > 0)
            {
                if (other.find('r',0) != string::npos) {
                        mode |= S_IROTH;
                }
                if (other.find('w',0) != string::npos) {
                        mode |= S_IWOTH;
                }
                if (other.find('x',0) != string::npos) {
                        mode |= S_IXOTH;
                }
            }

            return mode;
        }

        static bool Shred(string filename, bool alsoDelete) 
        {    
            if (!ItsFile::Exists(filename))
            {
                return false;
            }

            const size_t fileSize = ItsFile::GetFileSize(filename);
            if (fileSize == 0 ) {
                return false;
            }

            ItsFile f;
            f.OpenExisting(filename, "rw");
            if (f.IsInvalid())
            {
                return false;
            }

            const uint32_t bufferSize = 2048;
            size_t bytesWritten = 0;
            size_t totalWritten = 0;
            const unique_ptr<uint8_t[]> pdata = make_unique<uint8_t[]>(bufferSize);
            for (uint32_t i = 0; i < bufferSize; i++){
                pdata[i] = 0xFF;
            }
            f.SetPosFromBeg(0);
            while ( totalWritten < fileSize ) {
                f.Write(static_cast<const void*>(pdata.get()), ((fileSize-totalWritten) > bufferSize) ? bufferSize : (fileSize-totalWritten), &bytesWritten);
                totalWritten += bytesWritten;
            }
            f.Close();

            if (alsoDelete) {
                return ItsFile::Delete(filename);
            }

            return true;
        }

        static bool ShredAndDelete(string filename) 
        {
            return ItsFile::Shred(filename,true);
        }
    };

    //
    // struct: ItsPath
    //
    // (i): Path routines.
    //
    struct ItsPath
    {
    public:
        static const char PathSeparator = '/';
        static const char ExtensionSeparator = '.';
        
        static const vector<char> GetInvalidPathCharacters()
        {
            vector<char> chars {'\0'};
            return chars;
        }
        static const vector<char> GetInvalidFilenameCharacters()
        {
            vector<char> chars = ItsPath::GetInvalidPathCharacters();
            chars.push_back('/');
            return chars;
        }
        static string Combine(string path1, string path2)
        {
            if (path1.size() == 0 && path2.size() == 0) {
                return string("");
            }

            if (path1.size() == 0) {
                return path2;
            }

            if (path2.size() == 0) {
                return path1;
            }

            stringstream path;
            path << path1;
            if (path1[path1.size() - 1] != ItsPath::PathSeparator &&
                path2[0] != ItsPath::PathSeparator ) {
                path << ItsPath::PathSeparator;
            }
            path << path2;
            
            string retVal = path.str();
            return retVal;
        }
        static bool Exists(string path)
        {
            if (access(path.c_str(), F_OK) == 0) {
                return true;
            }
            return false;
        }
        static bool IsFile(string path)
        {
            return ItsFile::Exists(path);
        }
        static bool IsDirectory(string path)
        {
            return ItsDirectory::Exists(path);
        }
        static string GetDirectory(string path)
        {
            if (path.size() == 0) {
                return string("");
            }

            if (path.find(ItsPath::PathSeparator) == string::npos) {
                return string("");
            }

            auto i = path.find_last_of(ItsPath::PathSeparator);
            if (i == std::string::npos) {
                return string("");
            }
            return path.substr(0, i+1);
        }
        static string GetFilename(string path)
        {
            if (path.size() == 0) {
                return string("");
            }
            
            if (path.find(ItsPath::PathSeparator) == string::npos) {
                return string("");
            }

            auto i = path.rfind(ItsPath::PathSeparator);
            if (i == std::string::npos) {
                return string("");
            }
            return path.substr(i+1, path.size()-i-1);
        }
        static string GetExtension(string path)
        {
            if (path.size() == 0) {
                return string("");
            }

            if (path.find(ItsPath::PathSeparator) == string::npos) {
                return string("");
            }

            auto i = path.rfind(ItsPath::ExtensionSeparator);
            if (i == std::wstring::npos) {
                return string("");
            }
            return path.substr(i, path.size() - i);
        }
        static bool IsPathValid(string path)
        {
            if (path.size() == 0) {
                return false;
            }

            string directory = ItsPath::GetDirectory(path);
            string filename = ItsPath::GetFilename(path);
            
            auto invalidPathChars = ItsPath::GetInvalidPathCharacters();
            auto invalidFileChars = ItsPath::GetInvalidFilenameCharacters();
            
            
            if ( directory[directory.size()-1] != ItsPath::PathSeparator ) {
                return false;
            }

            return std::all_of(directory.begin(), directory.end(), [&](wchar_t d) {
                return std::none_of(invalidPathChars.begin(), invalidPathChars.end(), [&](wchar_t i) {
                    return d == i;
                    });
                });

            return std::all_of(filename.begin(), filename.end(), [&](wchar_t d) {
                return std::none_of(invalidFileChars.begin(), invalidFileChars.end(), [&](wchar_t i) {
                    return d == i;
                    });
                });

            return true;
        }
        static bool HasExtension(string path, string extension)
        {
            if (path.size() == 0) {
                return false;
            }

            string ext = ItsPath::GetExtension(path);
            if (ext.size() == 0) {
                return false;
            }

            return (strcmp(ext.c_str(), extension.c_str()) == 0);
        }
        static string ChangeExtension(string path, string newExtension)
        {
            if (path.size() == 0) {
                return string("");
            }

            if (newExtension.size() == 0) {
                return path;
            }

            if (path[path.size() - 1] == ItsPath::PathSeparator) {
                return path;
            }

            string ext = ItsPath::GetExtension(path);
            if (ext.size() == 0) {
                if (newExtension[0] == ItsPath::ExtensionSeparator) {
                    return path + newExtension;
                }
                else {
                    return path + ItsPath::ExtensionSeparator + newExtension;
                }
            }

            auto pe = path.rfind(ext);
            if (pe == string::npos) {
                return path;
            }

            string retVal = path.replace(pe, path.size()-pe, newExtension);
            return retVal;
        }
        
        static string GetParentDirectory(string path) 
        {
            if (path.size() == 0) {
                return string("");
            }

            path = ItsPath::GetDirectory(path);
            
            size_t pos1 = path.rfind(ItsPath::PathSeparator);
            if (pos1 == string::npos) {
                return string("");
            }

            size_t pos2 = pos1;
            if (pos1 == (path.size() - 1)) {
                pos2 = path.rfind(ItsPath::PathSeparator, pos1 - 1);
                if (pos2 == string::npos) {
                    pos2 = pos1;
                }
            }

            return path.substr(0, pos2+1);
        }
    };

    //
    // struct: ItsFileMonitorEvent
    //
    // (i): file monitor event object
    //
    struct ItsFileMonitorEvent {
        FSEventStreamEventId eventId;
        FSEventStreamEventFlags eventFlag;
        string path;
    };

    //
    // enum: ItsFileMonitorMask
    //
    // (i): mask to set what events you want
    //
    enum ItsFileMonitorMask : uint32_t {
        None = kFSEventStreamCreateFlagNone,
        UseCFTypes = kFSEventStreamCreateFlagUseCFTypes,
        NoDefer = kFSEventStreamCreateFlagNoDefer,
        WatchRoot = kFSEventStreamCreateFlagWatchRoot,
        IgnoreSelf = kFSEventStreamCreateFlagIgnoreSelf,
        FileEvents = kFSEventStreamCreateFlagFileEvents,
        MarkSelf = kFSEventStreamCreateFlagMarkSelf,
        UseExtendedData = kFSEventStreamCreateFlagUseExtendedData,
        FullHistory = kFSEventStreamCreateFlagFullHistory,
    };

    //
    // class: ItsFileMonitor
    //
    // (i): Monitors a given file folder
    //
    class ItsFileMonitor
    {
    private:
        function<void(ItsFileMonitorEvent&)> m_func;
        FSEventStreamRef m_stream;
        FSEventStreamCallback m_callback;
        FSEventStreamContext* m_callbackContext = nullptr; // put stream specific data here
        CFStringRef m_refPathname;
        CFArrayRef m_pathsToWatch;
        CFAbsoluteTime m_latency = 0.0; // latency in seconds
        thread m_thread;
        string m_pathname;
        bool m_bPaused;
        bool m_bStopped;
        uint32_t m_mask;
        inline static vector<ItsFileMonitor*> s_objects{};
    protected:
        void ExecuteDispatchThread() {
            /*
                1. done.
                2. The application schedules the stream on the run loop by calling FSEventStreamScheduleWithRunLoop.
            */
            FSEventStreamScheduleWithRunLoop(this->m_stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

            /*
                1. done.
                2. done.
                3. The application tells the file system events daemon to start sending events by calling FSEventStreamStart.
            */
            FSEventStreamStart(this->m_stream);

            CFRunLoopRef runLoop = CFRunLoopGetCurrent();
            CFRunLoopTimerContext context = {0, this, NULL, NULL, NULL};
            CFRunLoopTimerRef timer = CFRunLoopTimerCreate(kCFAllocatorDefault, 0.030, 0.030, 0, 0, &MyRunLoopCallback, &context); 
            CFRunLoopAddTimer(runLoop, timer, kCFRunLoopCommonModes);
            
            CFRunLoopRun();
            
            /*
                5. The application tells the daemon to stop sending events by calling FSEventStreamStop.
                6. If the application needs to restart the stream, go to step 3.
                7. The application unschedules the event from its run loop by calling FSEventStreamUnscheduleFromRunLoop.
                8. The application invalidates the stream by calling FSEventStreamInvalidate.
                9. The application releases its reference to the stream by calling FSEventStreamRelease.
            */
            FSEventStreamStop(this->m_stream);
            FSEventStreamUnscheduleFromRunLoop(this->m_stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
            FSEventStreamInvalidate(this->m_stream);
            FSEventStreamRelease(this->m_stream);
        }

        static void MyRunLoopCallback(CFRunLoopTimerRef timer, void *info)
        {
            ItsFileMonitor* pthis = reinterpret_cast<ItsFileMonitor*>(info);
            if ( pthis->m_bStopped ) {
                CFRunLoopStop(CFRunLoopGetCurrent());
            }
        }

        static void MonitorCallback(
            ConstFSEventStreamRef streamRef,
            void *clientCallBackInfo,
            size_t numEvents,
            void *eventPaths,
            const FSEventStreamEventFlags eventFlags[],
            const FSEventStreamEventId eventIds[])
        {
            for ( auto obj : ItsFileMonitor::s_objects ) {
                if ( obj->m_stream == streamRef ) {
                    if ( obj->m_bStopped ) {
                        CFRunLoopStop(CFRunLoopGetCurrent());
                        return;
                    }
                    if ( obj->m_bPaused ) {
                        return;
                    }

                    char **paths = reinterpret_cast<char **>(eventPaths);
                    for (int i = 0; i < numEvents; i++) {
                        ItsFileMonitorEvent event{0};
                        event.eventId = eventIds[i];
                        event.eventFlag = eventFlags[i];
                        event.path = paths[i];
                        
                        obj->m_func(event);
                    }
                    
                    break;
                }
            }
        }

    public:
        ItsFileMonitor(const string pathname, function<void(ItsFileMonitorEvent&)> func)
            : ItsFileMonitor(pathname, (ItsFileMonitorMask::FileEvents), func)
        {
            
        }
        ItsFileMonitor(const string pathname, uint32_t mask, function<void(ItsFileMonitorEvent&)> func)
            :   m_pathname(pathname),
                m_func(func),
                m_mask(mask),
                m_bPaused(false),
                m_bStopped(false)
        {
            if (ItsDirectory::Exists(this->m_pathname) ) {
                /*
                    1. The application creates a stream by calling FSEventStreamCreate or FSEventStreamCreateRelativeToDevice.
                */
                this->m_callback = &ItsFileMonitor::MonitorCallback;
                this->m_refPathname = CFStringCreateWithCString(kCFAllocatorDefault, pathname.c_str(), kCFStringEncodingUTF8);
                this->m_pathsToWatch = CFArrayCreate(nullptr, (const void**)&this->m_refPathname,1,nullptr);
                
                // 1.
                this->m_stream = FSEventStreamCreate(
                                            kCFAllocatorDefault,
                                            this->m_callback,
                                            this->m_callbackContext,
                                            this->m_pathsToWatch,
                                            kFSEventStreamEventIdSinceNow,
                                            this->m_latency,
                                            this->m_mask);

                if ( this->m_stream != nullptr ) {
                    ItsFileMonitor::s_objects.push_back(this);
                    this->m_thread = thread(&ItsFileMonitor::ExecuteDispatchThread, this);
                }
            }
        }
        void Pause() {
            this->m_bPaused = true;
        }
        void Resume() {
            this->m_bPaused = false;
        }
        bool IsPaused() {
            return this->m_bPaused;
        }
        void Stop() {
            this->m_bStopped = true;
        }
        bool IsStopped()
        {
            return this->m_bStopped;
        }
        ~ItsFileMonitor()
        {
            this->Stop();
            
            if ( this->m_thread.joinable() ) {
                this->m_thread.join();
            }

            auto itr = std::find(begin(ItsFileMonitor::s_objects), end(ItsFileMonitor::s_objects), this);
            if ( itr != end(ItsFileMonitor::s_objects) ) {
                ItsFileMonitor::s_objects.erase(itr);
            }
        }
    };
} // namespace ItSoftware::macOS::Core
