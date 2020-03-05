Imports System.IO
Imports System.Diagnostics.Process
Imports SharpCompress.Archive
Imports SharpCompress.Common
Imports System.Net.WebClient
Imports System.Net
Imports System.ComponentModel
Imports System.Runtime.InteropServices
Imports System.Text
Imports System.Threading
Imports System.IO.Compression

'Launcher/Updater coded by prlucas

Public Class Update

    Dim process1 As String = "Launcher"
    Dim Update1 As String = "Update.rar"
    Dim RequiredFile As String = "search.dll"
    Dim LangFile As String = "Esp"
    Dim webAddress As String = "http://www.helbreathss.net/web/download.php"
    Dim FileDownload As String = "http://helbreathssupdate.no-ip.info/Update.rar"
    Dim NewLine1 As String = "(!) Update completed, now close this windows and Play Game in Launcher"
    Dim NewLine2 As String = "* Thanks for play HelbreathSS Server *"
    Dim NewLine3 As String = "(!) Operación terminada, ahora cierre esta ventana e inicie el juego desde el Launcher!"
    Dim NewLine4 As String = "* Gracias por jugar HelbreathSS Server *"
    Dim Requiredfullgame As String = "You need Full game for play HelbreathSS Server"

    Dim WithEvents WC As New WebClient

    Private Sub Update_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        If File.Exists(LangFile) = False Then
            Console.AppendText("(!) Important: You must close this window once the update process is completed." & Environment.NewLine)
            Console.AppendText("(!) Once completed you can start the Game." & Environment.NewLine)
            Console.AppendText("(!) Performing tasks, please wait..." & Environment.NewLine)
            Console.AppendText("(!) This operation will take a few seconds!" & Environment.NewLine)
            Console.AppendText("(!) Please dont close applications." & Environment.NewLine)
            Console.AppendText("(!) Checking files in progress... Please wait!" & Environment.NewLine)
        Else
            Console.AppendText("(!) Importante: Debe cerrar esta ventana una vez terminado el proceso de actualizacion." & Environment.NewLine)
            Console.AppendText("(!) Una vez finalizado usted podra iniciar el Juego." & Environment.NewLine)
            Console.AppendText("(!) Realizando tareas, por favor espere..." & Environment.NewLine)
            Console.AppendText("(!) Esta operacion puede tardar algunos segundos!" & Environment.NewLine)
            Console.AppendText("(!) Por favor no cierre esta ventana." & Environment.NewLine)
            Console.AppendText("(!) Chequeando que todos los archivos esten actualizados... Por favor espere!" & Environment.NewLine)
        End If


        If File.Exists(RequiredFile) = False Then
            MessageBox.Show(Requiredfullgame, "Download", MessageBoxButtons.OK, MessageBoxIcon.Information)
            Process.Start(webAddress)
            System.Threading.Thread.Sleep(1000)
            Dim Launcher As Process
            Dim Launcher2 As Process() = Process.GetProcessesByName(process1)

            For Each Launcher In Launcher2
                Launcher.Kill()
            Next
            Application.Exit()
        Else
            WC.DownloadFileAsync(New Uri(FileDownload), Update1)
        End If

        If File.Exists(LangFile) = False Then
            Console.AppendText("(!) Checking done." & Environment.NewLine)
            Console.AppendText("(!) Please Wait, performing operations!" & Environment.NewLine)
        Else
            Console.AppendText("(!) Chequeo terminado." & Environment.NewLine)
            Console.AppendText("(!) Por favor espere, realizando operaciones!" & Environment.NewLine)
        End If

    End Sub


    Private Sub WC_DownloadProgressChanged(ByVal sender As Object, ByVal e As DownloadProgressChangedEventArgs) Handles WC.DownloadProgressChanged

        progress.Value = e.ProgressPercentage


    End Sub



    Private Sub WC_DownloadFileCompleted(ByVal sender As Object, ByVal e As System.ComponentModel.AsyncCompletedEventArgs) Handles WC.DownloadFileCompleted

      
        Dim appPath As String = AppDomain.CurrentDomain.BaseDirectory()

        Dim archive As IArchive = ArchiveFactory.Open(Update1)
        For Each entry In archive.Entries
            If Not entry.IsDirectory Then
                entry.WriteToDirectory(appPath, ExtractOptions.ExtractFullPath Or ExtractOptions.Overwrite)
            End If
        Next

        If File.Exists(LangFile) = False Then
            Console.AppendText(NewLine1 & Environment.NewLine)
            Console.AppendText(NewLine2 & Environment.NewLine)
        Else
            Console.AppendText(NewLine3 & Environment.NewLine)
            Console.AppendText(NewLine4 & Environment.NewLine)
        End If

    End Sub


    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Application.Exit()
    End Sub


End Class
