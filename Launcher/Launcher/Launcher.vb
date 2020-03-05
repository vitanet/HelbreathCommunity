Imports System.IO
Imports SharpCompress.Archive
Imports SharpCompress.Common
Imports System.Net
Imports System.ComponentModel
Public Class Launcher
    Private drag As Boolean
    Private mousex As Integer
    Private mousey As Integer
    Private fileName1 As String = "Client.exe"
    Private EspFile As String = "Esp"
    Private TitleMsgError As String = "Launcher Error"
    Private MsgError As String = "Reboot Launcher and dont close Update Console."
    Private MsgErrorEsp As String = "Por favor no cierre la consola de actualizacion. Una vez que termine inicie el juego"
    Private p() = Process.GetProcessesByName("Client")
    Private Tasks As New ProgramTasks
    Private Update1 As String = "Update.rar"
    Private WithEvents WC As New WebClient
    Private Sub Launcher_Load(ByVal sender As Object, ByVal e As EventArgs) Handles MyBase.Load
        Tasks.StartupCheckAndDeleteFiles()
        Tasks.AutoDownloadAndRunUpdate()
        If p.Count > 0 Then
            showinfo.Clear()
            showinfo.AppendText("      - Ready to start -")
            progress.Value = 100
        Else
            Try
                Dim fileReader As String = My.Computer.FileSystem.ReadAllText("version.txt")
                Dim request As System.Net.HttpWebRequest = System.Net.HttpWebRequest.Create("http://helbreathss.net/web/download/version.txt")
                Dim response As System.Net.HttpWebResponse = request.GetResponse()
                Dim sr As System.IO.StreamReader = New System.IO.StreamReader(response.GetResponseStream())
                Dim newestversion As String = sr.ReadToEnd()
                Dim currentversion As String = fileReader
                If newestversion.Contains(currentversion) Then
                    showinfo.Clear()
                    showinfo.AppendText("      - Ready to start -")
                    progress.Value = 100
                Else
                    backgroundWorker1.RunWorkerAsync()
                End If
            Catch ex As Exception
                MsgBox(ex.ToString, MsgBoxStyle.Critical, TitleMsgError)
            End Try
        End If
    End Sub
    Private Sub minimizeBtn_Click(sender As Object, e As EventArgs) Handles minimizeBtn.Click
        Me.WindowState = FormWindowState.Minimized
    End Sub
    Private Sub closeBtn_Click(sender As Object, e As EventArgs) Handles closeBtn.Click
        Me.Close()
    End Sub
    Private Sub strtBtn_Click(sender As Object, e As EventArgs) Handles strtBtn.Click
        Tasks.RegistryKeyFix()
        If Not File.Exists(fileName1) Then
            If Not File.Exists(EspFile) Then
                MsgBox(MsgError, MsgBoxStyle.Critical, TitleMsgError)
            Else
                MsgBox(MsgErrorEsp, MsgBoxStyle.Critical, TitleMsgError)
                Process.Start("tasks.exe")
            End If
        End If
    End Sub
    Private Sub backgroundWorker1_DoWork(sender As Object, e As DoWorkEventArgs) Handles backgroundWorker1.DoWork
        If p.Count = 0 Then
            If File.Exists("Client.exe") Then
                File.Delete("Client.exe")
            End If
        End If
        WC.DownloadFileAsync(New Uri("http://helbreathss.net/web/download/Update.rar"), Update1)
        showinfo.Clear()
        showinfo.AppendText("Downloading Updates...")
    End Sub
    Private Sub WC_DownloadProgressChanged(ByVal sender As Object, ByVal e As DownloadProgressChangedEventArgs) Handles WC.DownloadProgressChanged
        progress.Value = e.ProgressPercentage
    End Sub
    Private Sub WC_DownloadFileCompleted(ByVal sender As Object, ByVal e As System.ComponentModel.AsyncCompletedEventArgs) Handles WC.DownloadFileCompleted
        showinfo.Clear()
        showinfo.AppendText("Installing Updates...")
        Dim appPath As String = AppDomain.CurrentDomain.BaseDirectory()
        Dim archive As IArchive = ArchiveFactory.Open(Update1)
        For Each entry In archive.Entries
            If Not entry.IsDirectory Then
                entry.WriteToDirectory(appPath, ExtractOptions.ExtractFullPath Or ExtractOptions.Overwrite)
            End If
        Next
        showinfo.Clear()
        showinfo.AppendText("      - Ready to start -")
    End Sub
    Private Sub Button1_Click(sender As Object, e As EventArgs)
        BackgroundWorker2.RunWorkerAsync()
    End Sub
    Private Sub Launcher_MouseDown(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Me.MouseDown
        drag = True
        mousex = Windows.Forms.Cursor.Position.X - Me.Left
        mousey = Windows.Forms.Cursor.Position.Y - Me.Top
    End Sub
    Private Sub Launcher_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Me.MouseMove
        If drag Then
            Me.Top = Windows.Forms.Cursor.Position.Y - mousey
            Me.Left = Windows.Forms.Cursor.Position.X - mousex
        End If
    End Sub
    Private Sub Launcher_MouseUp(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Me.MouseUp
        drag = False
    End Sub
End Class