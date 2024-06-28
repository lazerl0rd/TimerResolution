#include <stdio.h>
#include <windows.h>

typedef NTSTATUS(CALLBACK *NTQUERYTIMERRESOLUTION)(
    OUT PULONG MaximumInterval,
    OUT PULONG MinimumInterval,
    OUT PULONG CurrentInterval);

typedef NTSTATUS(CALLBACK *NTSETTIMERRESOLUTION)(
    IN ULONG DesiredResolution,
    IN BOOLEAN SetResolution,
    OUT PULONG CurrentInterval);

int main() {
    FreeConsole(); // hides console

    ULONG MaximumInterval, MinimumInterval, CurrentInterval;
    PROCESS_POWER_THROTTLING_STATE PowerThrottling;

    HMODULE hNtDll = LoadLibraryA("NtDll.dll");

    if (!hNtDll) {
        printf("LoadLibrary failed");
        return 1;
    }

    NTQUERYTIMERRESOLUTION NtQueryTimerResolution = (NTQUERYTIMERRESOLUTION)GetProcAddress(hNtDll, "NtQueryTimerResolution");
    NTSETTIMERRESOLUTION NtSetTimerResolution = (NTSETTIMERRESOLUTION)GetProcAddress(hNtDll, "NtSetTimerResolution");

    RtlZeroMemory(&PowerThrottling, sizeof(PowerThrottling));

    PowerThrottling.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
    PowerThrottling.ControlMask = PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION;
    PowerThrottling.StateMask = 0;

    SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, &PowerThrottling, sizeof(PowerThrottling));

    if (NtQueryTimerResolution(&MaximumInterval, &MinimumInterval, &CurrentInterval)) {
        printf("NtQueryTimerResolution failed");
        return 1;
    }

    if (NtSetTimerResolution(MinimumInterval, true, &CurrentInterval)) {
        printf("NtSetTimerResolution failed");
        return 1;
    }

    printf("Current timer interval: %lf ms\n", (double)CurrentInterval / 10000);
    Sleep(INFINITE);
}
