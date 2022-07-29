#include <iostream>
#include <string>
#include <regex>
#include <array>

using namespace std;

string exec(const char* cmd)
{
    string result;
    array<char, 128> buffer;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += regex_replace(buffer.data(), std::regex("\n"), "");
    }

    return result;
}

string get_host()
{
    string host;
    if(!exec("kextstat | grep -F -e 'FakeSMC' -e 'VirtualSMC'").empty())
    {
        host = "Hackintosh (SMBIOS: " + exec("sysctl -n hw.model") + ")";
    }
    else
    {
        host = exec("sysctl -n hw.model");
    }
    return host;
}

// string get_uptime()
// {
//     // TODO: Parse it into hrs, mins, and sec and retrieve uptime in a better way
//     string uptime;
//     uptime = exec("uptime | awk '{print $3}'");
//     return uptime;
// }

// string get_packages()
// {
//     // TODO: add support for other package managers
//     string packages;
//     packages = exec("brew list | wc -l | xargs");
//     return packages;
// }

string get_shell()
{
    string shell;
    shell = exec("echo $SHELL");
    if(shell == "/bin/zsh")
    {
        return "zsh";
    }
    else if(shell == "/bin/fish")
    {
        return "fish";
    }
    else if(shell == "/bin/sh")
    {
        return "bash";
    }
    else
    {
        return shell;
    }
}

string get_wm()
{
    return "Quartz Compositor";
}

string get_term()
{
    string term;
    term = exec("echo $TERM_PROGRAM");
    if(term == "iTerm.app")
    {
        return "iTerm2";
    }
    else if(term == "Apple_Terminal")
    {
        return "Apple Terminal";
    }
    else
    {
        return term;
    }
}

int main()
{
    cout << "   " << exec("whoami") + "@" + exec("hostname") << endl;
    cout << "   ----------------------" << endl;
    cout << "   • Host:        " << get_host() << endl;
    cout << "   • Kernel:      " << exec("uname -s") << " " << exec("uname -r") << endl;
    cout << "   • Uptime:      " << exec("uptime | awk '{print $3}'") << endl;
    cout << "   • Packages:    " << exec("brew list | wc -l | xargs") << " (brew)" << endl;
    cout << "   • Shell:       " << get_shell() << endl;
    cout << "   • Resolution:  " << exec("system_profiler SPDisplaysDataType | grep UI | xargs | awk '{print $4, $5, $6, $7, $8}'") << endl;
    cout << "   • DE:          " << "Aqua" << endl;
    cout << "   • WM:          " << get_wm() << endl;
    cout << "   • Terminal:    " << get_term() << endl;
    cout << "   • CPU:         " << exec("sysctl -n machdep.cpu.brand_string") << endl;
    cout << "   • GPU:         " << exec("system_profiler SPDisplaysDataType | grep 'Chipset' | awk '{print $3, $4, $5, $6}'") << endl;
}

// TBA: 
// 1. Above comments
// 2. Public and Private IP
// 3. Battery (retrieve percentage and status)
// 4. Fix uptime
// 5. Properly retrieve WM
// 6. Memory
// 7. Fix Hackintosh check