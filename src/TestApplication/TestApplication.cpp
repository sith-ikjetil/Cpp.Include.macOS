///////////////////////////////////////////////////////////////////
//: Title        : TestApplication.cpp
//: Product:     : Cpp.Include.Windows
//: Date         : 2020-05-01
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of Cpp.Include.Window Test Application.
//
// #include
//
#include <iostream>
#include <string>
#include "../include/itsoftware-macos.h"
#include "../include/itsoftware-macos-core.h"

namespace ItSoftware::CppIncludeMacOS::TestApplication
{
    //
    // using
    //
    using std::cout;
    using std::endl;
    using std::ends;
    using std::string;
    using std::stringstream;
    using std::vector;
    using std::unique_ptr;
    using std::make_unique;
    using ItSoftware::macOS::ItsString;
    using ItSoftware::macOS::ItsExpandDirection;
    using ItSoftware::macOS::ItsTime;
    using ItSoftware::macOS::ItsConvert;
    using ItSoftware::macOS::ItsRandom;
    using ItSoftware::macOS::ItsDateTime;
    using ItSoftware::macOS::ItsLog;
    using ItSoftware::macOS::ItsDataSizeStringType;
    using ItSoftware::macOS::ItsDateTime;
    using ItSoftware::macOS::ItsID;
    using ItSoftware::macOS::ItsCreateIDOptions;
    using ItSoftware::macOS::Core::ItsTimer;
    using ItSoftware::macOS::Core::ItsFile;
    using ItSoftware::macOS::Core::ItsGuid;
    using ItSoftware::macOS::Core::ItsGuidFormat;
    using ItSoftware::macOS::Core::ItsPath;
    using ItSoftware::macOS::Core::ItsDirectory;
    using ItSoftware::macOS::Core::ItsError;
    using ItSoftware::macOS::Core::ItsFileMonitor;
    using ItSoftware::macOS::Core::ItsFileMonitorEvent;
    using ItSoftware::macOS::Core::ItsFileMonitorMask;

    //
    // Function Prototypes
    //
    void TestItsConvert();
    void TestItsRandom();
    void TestItsTime();
    void TestItsString();
    void TestItsLog();
    void TestItsTimerStart();
    void TestItsTimerStop();
    void TestItsFile();
    void TestItsDateTime();
    void TestItsID();
    void TestItsGuid();
    void TestItsPath();
    void TestItsDirectory();
    void TestItsFileMonitorStart();
    void TestItsFileMonitorStop();
    void ExitFn();
    void PrintTestHeader(string txt);
    void PrintTestSubHeader(string txt);
    void PrintTestApplicationEvent(string event);
    void HandleFileEvent(ItsFileMonitorEvent& event);

    //
    // #define
    //
    #define CLR_GREEN      "\033[32m"
    #define CLR_WHITE      "\033[37;1m"
    #define CLR_RESET      "\033[0m"

    //
    // global data
    //
    ItsTimer g_timer;
    char g_filename[] = "/Users/kjetilso/test.txt";
    char g_copyToFilename[] = "/Users/kjetilso/test2.txt";
    char g_shredFilename[] = "/Users/kjetilso/test2shred.txt";
    string g_path1("/Users");
    string g_path2("/kjetilso/test.txt");
    string g_invalidPath("Users\0/kjetilso");
    string g_directoryRoot("/Users/kjetilso");
    string g_creatDir("/Users/kjetilso/testdir");
    vector<string> g_fileMonNames;
    unique_ptr<ItsFileMonitor> g_fm;

    //
    // Function: ExitFn
    //
    // (i): Print exit message.
    // 
    void ExitFn()
    {
        cout << endl;
        PrintTestApplicationEvent("Completed");
    }

    //
    // Function: main
    //
    // (i): Application entry point
    //
    int main(int argc, const char* argv[])
    {
        atexit(ExitFn);

        PrintTestApplicationEvent("Started");

        TestItsTimerStart();
        TestItsFileMonitorStart();
        TestItsConvert();
        TestItsRandom();
        TestItsTime();
        TestItsString();
        TestItsLog();
        TestItsFile();
        TestItsDateTime();
        TestItsID();
        TestItsGuid();
        TestItsPath();
        TestItsDirectory();
        TestItsFileMonitorStop();
        TestItsTimerStop();

        return EXIT_SUCCESS;
    }

    //
    // Function: PrintTestApplicationEvent
    //
    // (i): prints application event string.
    //
    void PrintTestApplicationEvent(string event)
    {
        cout << CLR_RESET << CLR_GREEN;
        
        cout << std::setw(80) << std::setfill('#') << std::left << "## Test Application " << endl;
        
        cout << CLR_RESET << CLR_WHITE;
        
        cout << "> "<< event << " <" << endl;
    }

    //
    // Function: PrintTestHeader
    //
    // (i): Prints a tests header.
    //
    void PrintTestHeader(string txt)
    {
        cout << CLR_RESET << CLR_GREEN;

        cout << endl;

        stringstream ss;
        ss << " " << txt << " ";
        cout << ItsString::WidthExpand(ss.str(), 80, '_', ItsExpandDirection::Middle) << endl;

        cout << CLR_RESET << CLR_WHITE;
    }

    //
    // Function: PrintTestSubHeader
    //
    // (i): Prints a tests sub header.
    //
    void PrintTestSubHeader(string txt)
    {
        cout << CLR_RESET << CLR_GREEN;

        cout << endl;
        cout << "__ " << txt << " __" << endl;

        cout << CLR_RESET << CLR_WHITE;
    }

    //
    // Function: TestToNumber
    //
    // (i): Test numerics as string converted to primitive data types.
    //
    void TestItsConvert()
    {
        PrintTestHeader("ItsConvert");

        PrintTestSubHeader("ToNumber");
        cout << R"(ItsConvert::ToNumber<int>("-1234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<int>("-1234") << endl;
        cout << R"(ItsConvert::ToNumber<unsigned int>("1234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<unsigned int>("1234") << endl;
        cout << R"(ItsConvert::ToNumber<long>("-1234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<long>("-1234") << endl;
        cout << R"(ItsConvert::ToNumber<unsigned long>("123456789"))" << endl;
        cout << "> " << ItsConvert::ToNumber<unsigned long>("12346789") << endl;
        cout << R"(ItsConvert::ToNumber<long long>("12345678900"))" << endl;
        cout << "> " << ItsConvert::ToNumber<long long>("12345678900") << endl;
        cout << R"(ItsConvert::ToNumber<unsigned long long>("12345678900"))" << endl;
        cout << "> " << ItsConvert::ToNumber<unsigned long long>("12345678900") << endl;
        cout << R"(ItsConvert::ToNumber<float>("-12.34"))" << endl;
        cout << "> " << ItsConvert::ToNumber<float>("-12.34") << endl;
        cout << R"(ItsConvert::ToNumber<double>("1.234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<double>("1.234") << endl;
        cout << R"(ItsConvert::ToNumber<long double>("1.234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<long double>("1.234") << endl;
        cout << R"(ItsConvert::ToNumber<short>("1234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<short>("1234") << endl;
        cout << R"(ItsConvert::ToNumber<unsigned short>("40001"))" << endl;
        cout << "> " << ItsConvert::ToNumber<unsigned short>("40001") << endl;

        PrintTestSubHeader("ToString");
        cout << R"(ItsConvert::ToString<int>(-1234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<int>(-1234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<unsigned int>(1234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<unsigned int>(1234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<long>(-1234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<long>(-1234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<unsigned long>(123456789))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<unsigned long>(12346789) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<long long>(12345678900))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<long long>(12345678900) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<unsigned long long>(12345678900))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<unsigned long long>(12345678900) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<float>(-12.34))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<float>(-12.34f) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<double>(1.234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<double>(1.234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<long double>(1.234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<long double>(1.234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<short>(1234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<short>(1234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<unsigned short>(40001))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<unsigned short>(40001) << R"(")" << endl;
        
        PrintTestSubHeader("ToStringFormatted");
        cout << R"(ItsConvert::ToStringFormatted(256810246))" << endl;
        cout << R"(> ")" << ItsConvert::ToStringFormatted(256810246) << R"(")" << endl;
        cout << R"(ItsConvert::ToStringFormatted(256810246, L' '))" << endl;
        cout << R"(> ")" << ItsConvert::ToStringFormatted(256810246, L' ') << R"(")" << endl;
        
        PrintTestSubHeader("ToDataSizeString");
        cout << R"(ItsConvert::ToDataSizeString(1024, 2))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(1024, 2) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(200100400, 0))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(200100400, 0) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(200100400, 2))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(200100400, 2) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(9807804606, 3, ItsDataSizeStringType::IEC))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(9807804606, 3, ItsDataSizeStringType::IEC) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(1000, 3, ItsDataSizeStringType::IEC))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(1000, 3, ItsDataSizeStringType::IEC) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(size_t max, 2))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(SIZE_MAX, 2) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(size_t max, 2, ItsDataSizeStringType::IEC))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(SIZE_MAX, 2, ItsDataSizeStringType::IEC) << R"(")" << endl;

        PrintTestSubHeader("ToLongFromHex");
        cout << R"(ItsConvert::ToLongFromHex("0xFF3333"))" << endl;
        cout << "> " << ItsConvert::ToLongFromHex("0xFF3333") << endl;

        cout << endl;
    }

    //
    // Function: TestRandom
    //
    // (i): Test random integers and floats.
    //
    void TestItsRandom()
    {
        PrintTestHeader("ItsRandom");

        cout << "ItsRandom<long>(10'000, 1'000'000)" << endl;
        cout << "> " << ItsRandom<long>(10'000, 1'000'000) << endl;
        cout << "ItsRandom<long>(10'000, 1'000'000)" << endl;
        cout << "> " << ItsRandom<long>(10'000, 1'000'000) << endl;
        cout << "ItsRandom<float>(1, 10)" << endl;
        cout << "> " << ItsRandom<float>(1, 10) << endl;
        cout << "ItsRandom<float>(1, 10)" << endl;
        cout << "> " << ItsRandom<float>(1, 10) << endl;
        cout << "ItsRandom<double>(1, 2)" << endl;
        cout << "> " << ItsRandom<double>(1, 2) << endl;
        cout << "ItsRandom<double>(1, 2)" << endl;
        cout << "> " << ItsRandom<double>(1, 2) << endl;

        cout << endl;
    }

    //
    // Function: TestTime
    //
    // (i): Test rendering of time from milliseconds.
    //
    void TestItsTime()
    {
        PrintTestHeader("ItsTime");

        PrintTestSubHeader("RenderMsToFullString");
        cout << "ItsTime::RenderMsToFullString(92481379, false)" << endl;
        cout << R"(> ")" << ItsTime::RenderMsToFullString(92481379, false) << R"(")" << endl;
        cout << "ItsTime::RenderMsToFullString(92481379, true)" << endl;
        cout << R"(> ")" << ItsTime::RenderMsToFullString(92481379, true) << R"(")" << endl;
        
        PrintTestSubHeader("Now + ToString");
        cout << "ItsDateTime::Now().ToString()" << endl;
        cout << R"(> ")" << ItsDateTime::Now().ToString() << R"(")" << endl;
        cout << R"(ItsDateTime.Now().ToString("s"))" << endl;
        cout << R"(> ")" << ItsDateTime::Now().ToString("s") << R"(")" << endl;

        cout << endl;
    }

    //
    // Function: TestString
    //
    // (i): Test string manipulation routines.
    //
    void TestItsString()
    {
        PrintTestHeader("ItsString");

        cout << R"(ItsString::Left("Ab12Cd",4))" << endl;
        cout << R"(> ")" << ItsString::Left("Ab12Cd", 4) << R"(")" << endl;
        cout << R"(ItsString::Right("Ab12Cd",4))" << endl;
        cout << R"(> ")" << ItsString::Right("Ab12Cd", 4) << R"(")" << endl;
        cout << R"(ItsString::Mid("Ab12Cd",2,2))" << endl;
        cout << R"(> ")" << ItsString::Mid("Ab12Cd", 2, 2) << R"(")" << endl;
        cout << R"(ItsString::ToLowerCase("Ab12Cd"))" << endl;
        cout << R"(> ")" << ItsString::ToLowerCase("Ab12Cd") << R"(")" << endl;
        cout << R"(ItsString::ToUpperCase("Ab12Cd"))" << endl;
        cout << R"(> ")" << ItsString::ToUpperCase("Ab12Cd") << R"(")" << endl;
        cout << R"(ItsString::Trim("  Ab12Cd  "))" << endl;
        cout << R"(> ")" << ItsString::Trim("  Ab12Cd  ") << R"(")" << endl;
        cout << R"(ItsString::Replace("__ ABCDEF __"))" << endl;
        cout << R"(> ")" << ItsString::Replace("__ ABCDEF __", "__", "##") << R"(")" << endl;
        cout << R"(ItsString::Split("A;BC;DEF;G",";"))" << endl;
        auto vs = ItsString::Split("A;BC;DEF;G", ";");
        stringstream ss;
        ss << "{";
        bool bFirst{true};
        for (auto s : vs) {
            if ( !bFirst ) {
                ss << ",";
            }
            ss << R"(")" << s << R"(")";
            
            bFirst = false;            
        }
        ss << "}";
        ss << ends;
        cout << "> " << ss.str() << endl;
        cout << R"(ItsString::WidthExpand ("Kjetil", 30, L'_', ItsExpandDirection:Left))" << endl;
        cout << R"(> ")" << ItsString::WidthExpand("Kjetil", 30, L'_', ItsExpandDirection::Left) << R"(")" << endl;
        cout << R"(ItsString::WidthExpand ("Kjetil", 30, L'_', ItsExpandDirection:Middle))" << endl;
        cout << R"(> ")" << ItsString::WidthExpand("Kjetil", 30, L'_', ItsExpandDirection::Middle) << R"(")" << endl;
        cout << R"(ItsString::WidthExpand ("Kjetil", 30, L'_', ItsExpandDirection:Right))" << endl;
        cout << R"(> ")" << ItsString::WidthExpand("Kjetil", 30, L'_', ItsExpandDirection::Right) << R"(")" << endl;

        cout << endl;
    }

    //
    // Function: TestLog
    //
    // (i): Test ItsLog.
    //
    void TestItsLog()
    {
        PrintTestHeader("ItsLog");

        ItsLog log("ItsTestApp",true);
        log.LogInformation("This is an information log item");
        log.LogWarning("This is an warning log item");
        log.LogError("This is an error log item");
        log.LogOther("This is an other log item");
        log.LogDebug("This is an debug log item");

        PrintTestSubHeader("ToString");
        cout << log.ToString() << endl;
        
        cout << endl;
    }

    //
    // Function: TestFile
    //
    // (i): Test of ItsFile.
    //
    void TestItsFile()
    {
        PrintTestHeader("ItsFile");

        ItsFile file;
        cout << R"(file.OpenOrCreate(g_filename,"rwt",ItsFile::CreateMode("rw","rw","rw")))" << endl;
        if (!file.OpenOrCreate(g_filename,"rwt",ItsFile::CreateMode("rw","rw","rw")))
        {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success" << endl;

        char text[] = "Test Line 1\nTest Line 2\n";
        size_t written(0);
        cout << "file.Write((void*)text, strlen(text), &written)" << endl;
        if ( !file.Write(reinterpret_cast<void*>(text), strlen(text), &written) )
        {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. " << written << " bytes written to " << g_filename << endl;
        

        string str;
        cout << "file.ReadAllText(str)" << endl;
        if (!file.ReadAllText(str)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. Read all text from file " << g_filename << endl;
        

        cout << "file.Close()" << endl;
        if (!file.Close() ) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;;
        }
        cout << "> Success" << endl;

        cout << "ItsFile::Copy(g_filename, g_shredFilename, true)" << endl;
        if (!ItsFile::Copy(g_filename, g_shredFilename, true)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> SUCCESS. File " << g_filename << " successfully copied to " << g_shredFilename << endl;

        cout << "ItsFile::Copy(g_filename, g_copyToFilename, true)" << endl;
        if (!ItsFile::Copy(g_filename, g_copyToFilename, true)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. File " << g_filename << " successfully copied to " << g_copyToFilename << endl;
        
        cout << "ItsFile::Exists(g_copyToFilename)" << endl;
        if (!ItsFile::Exists(g_copyToFilename)) {
            cout << "> File " << g_copyToFilename << " does not exist" << endl;
        }
        else {
            cout << "> File " << g_copyToFilename << " exists" << endl;
        }

        cout << "ItsFile::Delete(g_copyToFilename)" << endl;
        if (!ItsFile::Delete(g_copyToFilename)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. File " << g_copyToFilename << " deleted" << endl;

        cout << "ItsFile::Shred(g_shredFilename,true)" << endl;
        if (!ItsFile::Shred(g_shredFilename, true)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. File " << g_shredFilename << " was shreded" << endl;

        cout << endl;
    }

    //
    // Function: TestStartTimer
    //
    // (i): Test of ItsTimer.
    //
    void TestItsTimerStart()
    {
        PrintTestHeader("ItsTimer Start");

        g_timer.Start();
        cout << "Timer started..." << endl;

        cout << endl;
    }

    //
    // Function: TestStopTimer
    //
    // (i): Test of ItsTimer.
    //
    void TestItsTimerStop()
    {
        PrintTestHeader("ItsTimer Stop");

        g_timer.Stop();
        cout << "Elapsed Time: " << ItsTime::RenderMsToFullString(g_timer.GetMilliseconds(),true) << endl;

        cout << endl;
    }

    //
    // Function: TestDateTime
    //
    // (i): Test ItsDateTime
    //
    void TestItsDateTime()
    {
        PrintTestHeader("ItsDateTime");

        auto now = ItsDateTime::Now();
        cout << "ItsDateTime.Now(): " << now.ToString() << endl;

        now.AddDays(7);
        now.AddHours(1);
        now.AddMinutes(1);
        now.AddSeconds(1);
        cout << "Added Days(7), Hours(1), Minutes(1) and Seconds(1): " << now.ToString() << endl;

        now.SubtractDays(7);
        now.SubtractHours(1);
        now.SubtractMinutes(1);
        now.SubtractSeconds(1);
        cout << "Subtracted Days(7), Hours(1), Minutes(1) and Seconds(1): " << now.ToString() << endl;

        cout << "Custom: " << now.ToString("dd.MM.yyyy HH:mm:ss") << endl;

        cout << endl;
    }

    //
    // Function: TestID
    //
    // (i): Tests ItsID.
    //
    void TestItsID()
    {
        PrintTestHeader("ItsID");

        cout << "ItsID::CreateID(12, ItsCreateIDOptions::LowerAndUpperCase, false)" << endl;
        cout << R"(> ")" << ItsID::CreateID(12, ItsCreateIDOptions::LowerAndUpperCase, false) << R"(")" << endl;
        cout << "ItsID::CreateID(10, ItsCreateIDOptions::LowerAndUpperCase, true)" << endl;
        cout << R"(> ")" << ItsID::CreateID(10, ItsCreateIDOptions::LowerAndUpperCase, true) << R"(")" << endl;
        cout << "ItsID::CreateID(8, ItsCreateIDOptions::UpperCase, false)" << endl;
        cout << R"(> ")" << ItsID::CreateID(8, ItsCreateIDOptions::UpperCase, false) << R"(")" << endl;
        cout << "ItsID::CreateID(6, ItsCreateIDOptions::UpperCase, true)" << endl;
        cout << R"(> ")" << ItsID::CreateID(6, ItsCreateIDOptions::UpperCase, true) << R"(")" << endl;
        cout << "ItsID::CreateID(14, ItsCreateIDOptions::LowerCase, false)" << endl;
        cout << R"(> ")" << ItsID::CreateID(14, ItsCreateIDOptions::LowerCase, false) << R"(")" << endl;
        cout << "ItsID::CreateID(16, ItsCreateIDOptions::LowerCase, true)" << endl;
        cout << R"(> ")" << ItsID::CreateID(16, ItsCreateIDOptions::LowerCase, true) << R"(")" << endl;
        cout << "ItsID::CreateID(64, ItsCreateIDOptions::LowerAndUpperCase, false)" << endl;
        cout << R"(> ")" << ItsID::CreateID(64, ItsCreateIDOptions::LowerAndUpperCase, false) << R"(")" << endl;
        cout << "ItsID::CreateID(64, ItsCreateIDOptions::LowerAndUpperCase, true)" << endl;
        cout << R"(> ")" << ItsID::CreateID(64, ItsCreateIDOptions::LowerAndUpperCase, true) << R"(")" << endl;

        cout << endl;
    }

    //
    // Function: TestItsGuid
    //
    // (i): Tests ItsGuid.
    //
    void TestItsGuid()
    {
        PrintTestHeader("ItsGuid");

        cout << "ItsGuid::CreateGuid()" << endl;
        cout << R"(> ")" << ItsGuid::CreateGuid() << R"(")" << endl;
        
        uuid_t guid{ 0 };
        cout << "ItsGuid::CreateGuid(guid)" << endl;
        ItsGuid::CreateGuid(guid);
        {
            cout << "> Success" << endl;
            
            cout << "ItsGuid::ToString(guid,ItsGuidFormat::MicrosoftRegistryFormat, true)" << endl;
            cout << R"(> ")" << ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftRegistryFormat, true) << R"(")" << endl;
            cout << "ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftRegistryFormatStripped, true)" << endl;
            cout << R"(> ")" << ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftRegistryFormatStripped, true) << R"(")" << endl;
            cout << "ItsGuid::ToString(guid,ItsGuidFormat::MicrosoftConstFormat, true)" << endl;
            cout << R"(> ")" << ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftConstFormat, true) << R"(")" << endl;
            cout << "ItsGuid::ToString(guid,ItsGuidFormat::MicrosoftCompactFormat, true)" << endl;
            cout << R"(> ")" << ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftCompactFormat, true) << R"(")" << endl;
            cout << "ItsGuid::ToString(guid,ItsGuidFormat::MicrosoftPrefixedCompactFormat, true)" << endl;
            cout << R"(> ")" << ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftPrefixedCompactFormat, true) << R"(")" << endl;
        }        

        cout << endl;
    }

    //
    // Function: TestItsPath
    //
    // (i): Test ItsPath
    //
    void TestItsPath()
    {
        PrintTestHeader("ItsPath");

        string path = ItsPath::Combine(g_path1, g_path2);
        cout << R"(ItsPath::Exists(path))" << endl;
        if (ItsPath::Exists(path)) {
            cout << "> Path: " << path << " exists" << endl;
        }
        else {
            cout << "> Path: " << path << " does not exist" << endl;
        }

        cout << R"(ItsPath::GetDirectory(path))" << endl; 
        cout << R"(> ")" << ItsPath::GetDirectory(path) << R"(")" << endl;
        cout << R"(ItsPath::GetFilename(path))" << endl; 
        cout << R"(> ")" << ItsPath::GetFilename(path) << R"(")" << endl;
        cout << R"(ItsPath::GetExtension(path))" << endl; 
        cout << R"(> ")" << ItsPath::GetExtension(path) << R"(")" << endl;
        cout << R"(ItsPath::HasExtension(path, ".txt"))" << endl; 
        cout << R"(> )" << ((ItsPath::HasExtension(path, ".txt")) ? "true" : "false") << endl;
        cout << R"(ItsPath::HasExtension(path, ".js"))" << endl; 
        cout << R"(> )" << ((ItsPath::HasExtension(path, ".js")) ? "true" : "false") << endl;
        cout << R"(ItsPath::ChangeExtension(path,".js"))" << endl; 
        cout << R"(> ")" << ItsPath::ChangeExtension(path, ".js") << endl;
        cout << R"(ItsPath::IsPathValid(path))" << endl; 
        cout << R"(> )" << ((ItsPath::IsPathValid(path)) ? "true" : "false") << endl;
        cout << R"(ItsPath::IsPathValid(g_invalidPath))" << endl; 
        cout << R"(> )" << ((ItsPath::IsPathValid(g_invalidPath)) ? "true" : "false") << endl;
        cout << R"(ItsPath::GetParentDirectory(path))" << endl;
        cout << R"(> )" << ItsPath::GetParentDirectory(path) << endl;

        cout << endl;
    }

    //
    // Function: TestItsDirectory
    //
    // (i): Tests ItsDirectory.
    //
    void TestItsDirectory()
    {
        PrintTestHeader("ItsDirectory");

        cout << R"(ItsDirectory::Exists(g_directoryRoot))" << endl;
        if (!ItsDirectory::Exists(g_directoryRoot)) {
            cout << "> Directory " << g_directoryRoot << " does NOT exist" << endl;
        }
        else {
            cout << "> Directory " << g_directoryRoot << " EXISTS" << endl;
        }

        cout << R"(ItsDirectory::GetDirectories(g_directoryRoot))" << endl;
        auto result = ItsDirectory::GetDirectories(g_directoryRoot);
        if (result.size() > 0) {
            cout << "> Success. Found " << result.size() << " sub-directories under " << g_directoryRoot << endl;
            for (auto r : result) {
                cout << ">> " << r << endl;
            }
        }
        else {
            cout << "> FAILED. No directories found under " << g_directoryRoot << endl;
        }

        cout << R"(ItsDirectory::GetFiles(g_directoryRoot))" << endl;
        auto result2 = ItsDirectory::GetFiles(g_directoryRoot);
        if (result2.size() > 0) {
            cout << "> Success. Found " << result2.size() << " files under " << g_directoryRoot << endl;
            for (auto r : result2) {
                cout << ">> " << r << endl;
            }
        }

        auto cdir = g_creatDir;
        cout << R"(ItsDirectory::CreateDirectory(cdir))" << endl;
        bool bResult = ItsDirectory::CreateDirectory(cdir, ItsFile::CreateMode("rw","rw","rw"));
        if (!bResult) {
            cout << "> FAILED. Error: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success creating " << cdir << endl;

        cout << R"(ItsDirectory::RemoveDirectory(cdir))" << endl;
        bResult = ItsDirectory::RemoveDirectory(cdir);
        if (!bResult) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;;
            cout << endl;
            return;
        }
        cout << "> Success removing directory " << cdir << endl;

        cout << endl;
    }

    //
    // Function: TestItsFileMonitor
    //
    // (i): Tests ItsFileMonitor.
    //
    void TestItsFileMonitorStart()
    {
        g_fm = make_unique<ItsFileMonitor>(g_directoryRoot, (ItsFileMonitorMask::FileEvents), HandleFileEvent);  
        
        PrintTestHeader("ItsFileMonitor Start");
        cout << "File monitor monitoring directory '" << g_directoryRoot << "' with mask 'ItsFileMonitorMask::FileEvents'" << endl;
        
        cout << endl;
    }

    //
    // Function: TestItsFileMonitor
    //
    // (i): Tests ItsFileMonitor.
    //
    void TestItsFileMonitorStop()
    {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // allow for delay in receiving file data
        g_fm->Stop();

        PrintTestHeader("ItsFileMonitor Stop");
        cout << "File monitor monitoring directory '" << g_directoryRoot << "' with mask 'ItsFileMonitorMask::FileEvents'" << endl;
        cout << "Items found:" << endl;
        for ( auto i : g_fileMonNames ) {
            cout << ">> " << i << endl;
        }

        cout << endl;
    }

    //
    // Function: HandleFileEvent
    //
    // (i): copy file event names
    //
    void HandleFileEvent(ItsFileMonitorEvent& event)
    {
        stringstream ss;
        ss << "Name: " << event.path << ", Flag: ";
        if (event.eventFlag & kFSEventStreamEventFlagNone) {
            ss << "[kFSEventStreamEventFlagNone] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagMustScanSubDirs) {
            ss << "[kFSEventStreamEventFlagMustScanSubDirs] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagUserDropped) {
            ss << "[kFSEventStreamEventFlagUserDropped] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagKernelDropped) {
            ss << "[kFSEventStreamEventFlagKernelDropped] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagEventIdsWrapped) {
            ss << "[kFSEventStreamEventFlagEventIdsWrapped] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagHistoryDone) {
            ss << "[kFSEventStreamEventFlagHistoryDone] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagRootChanged) {
            ss << "[kFSEventStreamEventFlagRootChanged] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagMount) {
            ss << "[kFSEventStreamEventFlagMount] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagUnmount) {
            ss << "[kFSEventStreamEventFlagUnmount] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemChangeOwner) {
            ss << "[kFSEventStreamEventFlagItemChangeOwner] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemCreated) {
            ss << "[kFSEventStreamEventFlagItemCreated] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemFinderInfoMod) {
            ss << "[kFSEventStreamEventFlagItemFinderInfoMod] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemInodeMetaMod) {
            ss << "[kFSEventStreamEventFlagItemInodeMetaMod] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemIsDir) {
            ss << "[kFSEventStreamEventFlagItemIsDir] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemIsFile) {
            ss << "[kFSEventStreamEventFlagItemIsFile] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemIsHardlink) {
            ss << "[kFSEventStreamEventFlagItemIsHardlink] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemIsLastHardlink) {
            ss << "[kFSEventStreamEventFlagItemIsLastHardlink] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemIsSymlink) {
            ss << "[kFSEventStreamEventFlagItemIsSymlink] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemModified) {
            ss << "[kFSEventStreamEventFlagItemModified] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemRemoved) {
            ss << "[kFSEventStreamEventFlagItemRemoved] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemRenamed) {
            ss << "[kFSEventStreamEventFlagItemRenamed] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemXattrMod) {
            ss << "[kFSEventStreamEventFlagItemXattrMod] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagOwnEvent) {
            ss << "[kFSEventStreamEventFlagOwnEvent] ";
        }
        if (event.eventFlag & kFSEventStreamEventFlagItemCloned) {
            ss << "[kFSEventStreamEventFlagItemCloned] ";
        }

        g_fileMonNames.push_back(ss.str());
    }
}

//
// Function: main
//
// (i): Application entry point.
//      Redirects to ItSoftware::CppIncludeMacOS::TestApplication::main.
//
int main(int argc, const char* argv[])
{
    return ItSoftware::CppIncludeMacOS::TestApplication::main(argc, argv);
}