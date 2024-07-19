
#pragma comment(lib, "winhttp.lib")

#include "config.h"
#include "configshare.h"

namespace cfgshare {

    std::wstring StringToWString(const std::string& s) {
        int len;
        int slength = (int)s.length() + 1;
        len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
        std::wstring r(len, L'\0');
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
        return r;
    }

    std::string UploadConfig(const std::string& filePath) {
        HINTERNET hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS, 0);

        if (!hSession) {
            std::cerr << "Failed to open WinHTTP session." << std::endl;
            return "";
        }

        HINTERNET hConnect = WinHttpConnect(hSession, L"0x0.st",
            INTERNET_DEFAULT_HTTPS_PORT, 0);

        if (!hConnect) {
            std::cerr << "Failed to connect to server." << std::endl;
            WinHttpCloseHandle(hSession);
            return "";
        }

        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", NULL,
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            WINHTTP_FLAG_SECURE);

        if (!hRequest) {
            std::cerr << "Failed to open HTTP request." << std::endl;
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return "";
        }

        // Prepare the file content to send
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return "";
        }

        std::ostringstream oss;
        oss << file.rdbuf();
        std::string fileContent = oss.str();

        std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
        std::string headers = "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n";
        std::string body = "--" + boundary + "\r\n";
        body += "Content-Disposition: form-data; name=\"file\"; filename=\"" + filePath + "\"\r\n";
        body += "Content-Type: application/octet-stream\r\n\r\n";
        body += fileContent + "\r\n";
        body += "--" + boundary + "--\r\n";

        WinHttpAddRequestHeaders(hRequest, StringToWString(headers).c_str(), (ULONG)-1L, WINHTTP_ADDREQ_FLAG_REPLACE | WINHTTP_ADDREQ_FLAG_ADD);

        BOOL result = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            (LPVOID)body.c_str(), body.size(),
            body.size(), 0);

        if (!result) {
            std::cerr << "Failed to send HTTP request." << std::endl;
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return "";
        }

        result = WinHttpReceiveResponse(hRequest, NULL);

        if (!result) {
            std::cerr << "Failed to receive HTTP response." << std::endl;
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return "";
        }

        DWORD dwSize = 0;
        DWORD dwDownloaded = 0;
        std::string response;

        do {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                std::cerr << "Error in WinHttpQueryDataAvailable: " << GetLastError() << std::endl;
            }

            if (dwSize == 0)
                break;

            char* pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer) {
                std::cerr << "Out of memory" << std::endl;
                dwSize = 0;
            }
            else {
                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                    std::cerr << "Error in WinHttpReadData: " << GetLastError() << std::endl;
                }
                else {
                    response.append(pszOutBuffer, dwDownloaded);
                }

                delete[] pszOutBuffer;
            }
        } while (dwSize > 0);

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);

        return response;
    }
}