@echo off
chcp 65001
REM 设置虚拟机的路径
set VM_PATH="D:\神盾虚拟机\神盾WIN7随机硬件版.vmx"

REM 启动虚拟机
"C:\Program Files (x86)\VMware\VMware Workstation\vmrun.exe" start %VM_PATH%

REM 等待虚拟机启动
timeout /t 5

REM 使用 PowerShell 调整窗口
powershell -command "& {
    Add-Type -AssemblyName System.Windows.Forms
    Add-Type -AssemblyName System.Drawing

    function Get-WindowByTitle {
        param ([string]$title)
        $shell = New-Object -ComObject Shell.Application
        $windows = $shell.Windows() | Where-Object { $_.LocationName -like $title }
        return $windows
    }

    function Set-WindowToScreen {
        param ([System.Windows.Forms.Screen]$screen, [System.Windows.Forms.IWin32Window]$window)
        $rect = $screen.Bounds
        $window.Left = $rect.Left
        $window.Top = $rect.Top
        $window.Width = $rect.Width
        $window.Height = $rect.Height
    }

    $title = "*VMware Workstation*"
    $window = Get-WindowByTitle -title $title
    if ($window) {
        $screen = [System.Windows.Forms.Screen]::AllScreens[2]
        Set-WindowToScreen -screen $screen -window $window
        $window.FullScreen = $true
    } else {
        Write-Output "未找到 VMware Workstation 窗口"
    }
}"

REM 提示完成
echo 虚拟机已启动并设置为全屏显示在第三显示屏
pause
