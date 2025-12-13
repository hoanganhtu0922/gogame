#include "katago.h"
void KataGoController::start() {
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // 1. Create Pipes
    if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0)) throw std::runtime_error("Pipe Error");
    SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0);
    if (!CreatePipe(&hChildStd_IN_Rd, &hChildStd_IN_Wr, &saAttr, 0)) throw std::runtime_error("Pipe Error");
    SetHandleInformation(hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0);

    // 2. Setup Startup Info
    STARTUPINFOA siStartInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = hChildStd_OUT_Wr;  // IMPORTANT: We want to see Errors too!
    siStartInfo.hStdOutput = hChildStd_OUT_Wr;
    siStartInfo.hStdInput = hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    std::string fullCmd = "..\\..\\src\\katago\\katago.exe gtp -model ..\\..\\src\\katago\\model.gz -config ..\\..\\src\\katago\\default_gtp.cfg";
    std::vector<char> cmdVec(fullCmd.begin(), fullCmd.end());
    cmdVec.push_back(0);

    // 3. Create Process (Current Directory)
    BOOL success = CreateProcessA(
        NULL,
        cmdVec.data(),
        NULL, NULL, TRUE, 0, NULL,
        NULL,
        &siStartInfo, &piProcInfo
    );

    CloseHandle(hChildStd_OUT_Wr);
    CloseHandle(hChildStd_IN_Rd);

    if (!success) {
        std::cout << "Failed to start KataGo. Is katago.exe in this folder?" << std::endl;
    }
}

int totalCount(const std::string &str) {
    int cnt = 0;
    for (char c : str) {
        if (c == '\n')
            cnt++;
    }

    return cnt;
}

std::string KataGoController::ReadResponse() {
    const int BUFSIZE = 4096;
    char buffer[BUFSIZE];
    DWORD dwRead;
    DWORD dwAvail;
    std::string totalOutput = "";
    bool foundStart = false;

    //Sleep(500);// wait for the data
    while (true) {
        DWORD exitCode;
        if (GetExitCodeProcess(piProcInfo.hProcess, &exitCode)) {
            if (exitCode != STILL_ACTIVE) return "Error: KataGo died.";
        }

        // 2. Kiểm tra xem có dữ liệu trong ống không
        if (!PeekNamedPipe(hChildStd_OUT_Rd, NULL, 0, NULL, &dwAvail, NULL)) break;

        if (dwAvail > 0) {
            BOOL success = ReadFile(hChildStd_OUT_Rd, buffer, BUFSIZE - 1, &dwRead, NULL);
            if (!success || dwRead == 0) continue;

            buffer[dwRead] = '\0';
            std::string chunk(buffer);
            totalOutput += chunk;

            if ((totalOutput.find("\n\n") != std::string::npos) || (totalOutput.find("\r\n\r\n") != std::string::npos)) {
                break;
            }
        }
    }
    return totalOutput;
}

KataGoController::~KataGoController() {
    Send("quit");
    CloseHandle(hChildStd_IN_Wr);
    CloseHandle(hChildStd_OUT_Rd);
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
}

void KataGoController::Send(std::string cmd) {
    DWORD dwWritten;
    std::string packet = cmd + "\n";
    WriteFile(hChildStd_IN_Wr, packet.c_str(), packet.size(), &dwWritten, NULL);
    FlushFileBuffers(hChildStd_IN_Wr); // Push it immediately
}

std::string KataGoController::transPoint(std::vector <int> v) {
    std::string turn = "b";
    if (v.back() == 0)
        turn = "w";

    int c = 0;
    if ('A' + v[0] >= 'I')
        c = 1;

    return turn + " " + std::string(1, 'A' + v[0] + c) + std::to_string(gridSize - v[1]) + " ";
}

int KataGoController::to_int(std::string s) {
    int res = 0;
    for (char c : s) {
        if (c > '9' || c < '0')
            continue;

        res = res * 10 + (c - '0');
    }

    return res;
}

std::pair<int, int> KataGoController::coord(std::string str) {
    while (str.length() && str[0] != '=')
        str.erase(0, 1);

    if (str.empty())
        return {-2, -2};

    //std::cout << "here "  << str;
    if (str.find("resign") !=  std::string::npos)
        return {-1, -1};

    if (str.find("pass") !=  std::string::npos)
        return {-2, -2};

    int j = str[2] - 'A';
    if (str[2] > 'I')
        j--;

    int i = to_int(str.substr(3));
    return {j, gridSize - i};
}

std::pair<int, int> KataGoController::getAImove(std::vector<std::vector<int>> points, int b) {
    // Play all moves so far
    std::string board = "set_position ";
    for (auto &p : points) {
        board += transPoint(p);
    }

    Send(board);
    std::string ans = ReadResponse();

    std::cout << board << "\n";
    // Ask for AI move
    std::string turn = "w";
    if (b == 0)
        turn = "b";

    Send("genmove " + turn);

    ans = ReadResponse();
    std::cout << "position " << ans;
    std::pair <int, int> pp = coord(ans);
    return pp;
}

