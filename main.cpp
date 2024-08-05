#include <windows.h>
#include <vector>
#include <iostream>
#include <string>

struct WindowInfo {
    HWND hwnd;
    std::string className;
    std::vector<WindowInfo> children; 
};

HWND FindWindowByClassName(const char* className, HWND parent = nullptr, HWND after = nullptr) {
    return FindWindowExA(parent, after, className, nullptr);
}

HWND FindFirstChildWindow(HWND parent) {
    return FindWindowExW(parent, nullptr, nullptr, nullptr);
}

void CollectWindows(HWND hwnd, WindowInfo& windowInfo) {
    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child != NULL) {
        char className[256];
        GetClassNameA(child, className, sizeof(className));

        WindowInfo childInfo = { child, className };

        CollectWindows(child, childInfo);

        windowInfo.children.push_back(childInfo);

        child = GetWindow(child, GW_HWNDNEXT);
    }
}

void GrabSpecificWindows(const WindowInfo& windowInfo) {
    if (windowInfo.className == "AfxFrameOrView90u" && windowInfo.children.empty()) {
        std::cout << "AfxFrameOrView90u:" << windowInfo.className << " (" << windowInfo.hwnd << ")" << std::endl;
        SendMessageW(windowInfo.hwnd, 0x111u, 0xE107u, 0);
    }
    for (const auto& child : windowInfo.children) {
        GrabSpecificWindows(child);
    }
}

HWND FindInterfaceHandles() {
    auto mainFrameHandle = FindWindowByClassName("EngWBMainFrameV3");

    auto mdiClientHandle = FindWindowByClassName("MDIClient", mainFrameHandle);

    std::cout << "MDIClient HWND: " << mdiClientHandle << std::endl;

    auto firstChildHandle = FindFirstChildWindow(mdiClientHandle);
    std::cout << "First Child HWND: " << firstChildHandle << std::endl;

    auto secondChildHandle = FindFirstChildWindow(mdiClientHandle);
    std::cout << "Second Child HWND: " << secondChildHandle << std::endl;

    return secondChildHandle;
}

int main() {

    HWND interfaceHandele = FindInterfaceHandles();

    char className[256];
    GetClassNameA(interfaceHandele, className, sizeof(className));
    WindowInfo mainWindow = { interfaceHandele, className };

    CollectWindows(interfaceHandele, mainWindow);

    GrabSpecificWindows(mainWindow);

    return 0;
}
