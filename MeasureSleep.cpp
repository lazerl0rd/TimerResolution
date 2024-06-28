#include <iostream>
#include <windows.h>

typedef NTSTATUS(CALLBACK *NTQUERYTIMERRESOLUTION)(
    OUT PULONG MaximumInterval,
    OUT PULONG MinimumInterval,
    OUT PULONG CurrentInterval);

int main() {
    ULONG MaximumInterval, MinimumInterval, CurrentInterval;
    LARGE_INTEGER start, end, freq;

    QueryPerformanceFrequency(&freq);

    HMODULE hNtDll = LoadLibraryA("NtDll.dll");

    if (!hNtDll) {
        printf("LoadLibrary failed");
        return 1;
    }

    NTQUERYTIMERRESOLUTION NtQueryTimerResolution = (NTQUERYTIMERRESOLUTION)GetProcAddress(hNtDll, "NtQueryTimerResolution");

    for (;;) {
        // get current resolution
        if (NtQueryTimerResolution(&MaximumInterval, &MinimumInterval, &CurrentInterval)) {
            printf("NtQueryTimerResolution failed");
            return 1;
        }

        // benchmark Sleep(1)
        QueryPerformanceCounter(&start);
        Sleep(1);
        QueryPerformanceCounter(&end);

        double delta_s = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
        double delta_ms = delta_s * 1000;

        printf("Current timer interval: %lf ms\n Sleep(1) interval: %lf ms (delta: %lf)\n",
               CurrentInterval / 10000.0, delta_ms, delta_ms - 1);

        Sleep(1000); // pause for a second between iterations
    }
}
