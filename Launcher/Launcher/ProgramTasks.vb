Imports System.IO
Imports SharpCompress.Archive
Imports SharpCompress.Common
Imports System.Net
Public Class ProgramTasks
    Private fileName2 As String = "Full.rar"
    Private fileName4 As String = "Files.rar"
    Private FileName5 As String = "Update.exe"
    Private fileName7 As String = "\Client.exe"
    Private fileName8 As String = "LauncherUpdate.rar"
    Public Sub StartupCheckAndDeleteFiles()
        If File.Exists(fileName2) Then
            File.Delete(fileName2)
        End If
        If File.Exists(fileName4) Then
            File.Delete(fileName4)
        End If
        If File.Exists(FileName5) Then
            File.Delete(FileName5)
        End If
        If File.Exists(fileName8) Then
            File.Delete(fileName8)
        End If
    End Sub
    Public Sub AutoDownloadAndRunUpdate()
        Dim myWebClient As New WebClient()
        Try
            If Not File.Exists(fileName8) Then
                Dim myStringWebResource As String = "http://23489sdasd.com/" & fileName8
                myWebClient.DownloadFile(myStringWebResource, fileName8)
                Dim archive As IArchive = ArchiveFactory.Open(fileName8)
                For Each entry In archive.Entries
                    If Not entry.IsDirectory Then
                        entry.WriteToDirectory(AppDomain.CurrentDomain.BaseDirectory, ExtractOptions.ExtractFullPath Or ExtractOptions.Overwrite)
                    End If
                Next
            End If
        Catch ex As Exception
            MsgBox("No se encuentra el archivo solicitado.", MsgBoxStyle.Critical, "Launcher Error")
        Finally
            Process.Start(FileName5)
        End Try
    End Sub
    Public Sub RegistryKeyFix()
        Dim osversion As Version = Environment.OSVersion.Version
        If osversion.Major = 6 And osversion.Minor > 1 Then
            ' DetectorDWM8And16Bit              This Fix offeres mitigation for applications that work in 8/16-bit display color mode because these legacy color modes are Not supported in Windows 8 .
            ' DWM8And16BitMitigation            The fix offers mitigation for applications that work in 8/16-bit display color mode because these legacy color modes are not supported in Windows 8.
            ' 8And16BitAggregateBlts            Applications that are mitigated by 8/16-bit mitigation can exhibit performance issues. This layer aggregates all the blt operations and improves performance.
            ' 8And16BitDXMaxWinMode             Applications that use DX8/9 and are mitigated by the 8/16-bit mitigation are run in a maximized windowed mode. This layer mitigates applications that exhibit graphical corruption in full screen mode.
            My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers", Application.StartupPath & fileName7, "$ DWM8And16BitMitigation 8And16BitTimedPriSync 8And16BitAggregateBlts Layer_ForceDirectDrawEmulation")
        End If
        If osversion.Major = 10 And osversion.Minor = 0 Then
            My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers", Application.StartupPath & fileName7, "$ DWM8And16BitMitigation 8And16BitTimedPriSync 8And16BitAggregateBlts Layer_ForceDirectDrawEmulation")
        End If
    End Sub
End Class
