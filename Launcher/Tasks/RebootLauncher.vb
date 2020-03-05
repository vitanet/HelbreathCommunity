Module RebootLauncher
    Sub Main()
        Dim Launcher As Process
        Dim Launcher2 As Process() = Process.GetProcessesByName("Launcher")
        For Each Launcher In Launcher2
            Launcher.Kill()
        Next
        System.Diagnostics.Process.Start("Launcher.exe")
    End Sub
End Module
