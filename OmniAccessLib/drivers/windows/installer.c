// installer.c - Placeholder content
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[]) {
    SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!scm) {
        _tprintf(_T("Failed to open Service Control Manager\n"));
        return 1;
    }

    TCHAR path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    TCHAR* last_slash = _tcsrchr(path, _T('\\'));
    if (last_slash) *(last_slash + 1) = _T('\0');
    _tcscat_s(path, MAX_PATH, _T("OmniAccessDriver.sys"));

    SC_HANDLE service = CreateService(
        scm,
        _T("OmniAccessDriver"),
        _T("OmniAccess System Driver"),
        SERVICE_ALL_ACCESS,
        SERVICE_KERNEL_DRIVER,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        path,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (!service) {
        DWORD err = GetLastError();
        if (err == ERROR_SERVICE_EXISTS) {
            service = OpenService(scm, _T("OmniAccessDriver"), SERVICE_ALL_ACCESS);
        } else {
            _tprintf(_T("CreateService failed (%d)\n"), err);
            CloseServiceHandle(scm);
            return 1;
        }
    }

    if (!StartService(service, 0, NULL)) {
        DWORD err = GetLastError();
        if (err != ERROR_SERVICE_ALREADY_RUNNING) {
            _tprintf(_T("StartService failed (%d)\n"), err);
            CloseServiceHandle(service);
            CloseServiceHandle(scm);
            return 1;
        }
    }

    _tprintf(_T("Driver installed and started successfully\n"));

    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    return 0;
}