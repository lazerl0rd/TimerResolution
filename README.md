# TimerResolution

Use the minimum timer interval in Windows.

See [Fixing timing precision in Windows after the "The Great Rule Change"](https://web.archive.org/web/20230313101711/https://github.com/amitxv/PC-Tuning/blob/main/docs/research.md#fixing-timing-precision-in-windows-after-the-great-rule-change) for a detailed explanation regarding restoring the old timer resolution implementation otherwise nothing below will make sense.

`MeasureSleep` is used to measure the precision of `Sleep(1)`. It is preferable to keep this close to 1ms.

`SetTimerResolution` queries and sets the minimum timer interval applicable to the system (usually 0.5ms). To start it automatically on boot, you can place the binary in `shell:startup` (use the Run utility to open this folder).

On Windows Server 21H2+ and Windows 11+, apply `EnableGlobalTimerResolution.reg` so that requesting a higher resolution is effective on a system-wide level rather than only the calling process. Use `DisableGlobalTimerResolution.reg` to revert to default behaviour.
