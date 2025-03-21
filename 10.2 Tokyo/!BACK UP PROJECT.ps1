$MyVersion = "ver 1.0.0"
# ------------------------------------------------
# 제외되어야할 항목들 나열. (폴더명이나 파일명 (와일드 카드 사용 가능)
$My_Exclude = @('Debug', 
                'Logs',
                '__history', 
                '.svn', 
                '*.ilc', 
                '*.ild', 
                '*.ilf', 
                '*.ils', 
                '*.map', 
                '*.tds', 
                '*.log', 
                '*.bak',
                '*.exe',
                'SoftResetData', 
                'PackageData')

# ------------------------------------------------
# Colro Names
# Black
# Blue
# Cyan
# DarkBlue
# DarkCyan
# DarkGray
# DarkGreen
# DarkMagenta
# DarkRed
# DarkYellow
# Gray
# Green
# Magenta
# Red
# White
# Yellow

# ------------------------------------------------
# 복사 함수. 

function Copy-ToCreateFolder
{
    param(
        [string]$src,
        [string]$dest,
        $exclude,
        [switch]$Recurse,
        [int]$CopyCnt
    )

    $CopyCnt = 0
    
    # The promlem with Copy-Item -Rec -Exclude is that -exclude effects only top-level files
    # Copy-Item $src $dest    -Exclude $exclude       -EA silentlycontinue -Recurse:$recurse
    # http://stackoverflow.com/questions/731752/exclude-list-in-powershell-copy-item-does-not-appear-to-be-working

    if (Test-Path($src))
    {
        # nonstandard: I create destination directories on the fly
        [void](New-Item $dest -itemtype directory -EA silentlycontinue )
        Get-ChildItem -Path $src -Force -exclude $exclude | % {
        
            if ($_.psIsContainer)
            {
                if ($Recurse) # non standard: I don't want to copy empty directories
                {
                    $currentfolder = Split-Path $_ -leaf
                    $SubDest = $dest + "\" + $currentfolder
                    $SubCopyCnt = Copy-ToCreateFolder $_ $SubDest $exclude -Recurse:$Recurse
                    $CopyCnt = $CopyCnt + $SubCopyCnt
                }
            }
            else
            {
                #"{0}    {1}" -f (split-path $_.fullname), (split-path $_.fullname -leaf)
		Clear
                write-host $_.FullName -foregroundcolor "White" # 여기서 echo를 사용하면 pipe line에 출력되어 return 값에 포함된다. 그럼 함소를 호출한 쪽에서는 리턴 받은 값이 System.Object 형으로 받게 된다. 
                Copy-Item $_ $dest -Force -Recurse
                $CopyCnt = $CopyCnt + 1
            }
        }
    }

    Write-Output $CopyCnt
}
# ------------------------------------------------
# 폴더의 용량을 얻어오는 함수. 

function Get-FolderLength
{
    param(
        [string]$src,
        $exclude,
        [switch]$Recurse,
        [int]$SumSpace
    )
    
    $SumSpace = 0
   
    if (Test-Path($src))
    {
        Get-ChildItem -Path $src -Force -exclude $exclude | % {
        
            if ($_.psIsContainer)
            {
                if ($Recurse) # non standard: I don't want to copy empty directories
                {
                    $currentfolder = Split-Path $_ -leaf
                    $SubSum = Get-FolderLength $_ $exclude -Recurse:$Recurse
                    $SumSpace += $SubSum
                }
            }
            else
            {
                $SumSpace = $SumSpace + (Get-Item -Force $_.FullName).length
            }
        }
    }
    
    return $SumSpace
}
# ------------------------------------------------
# "현재 스크립트가 존재하는 위치 '경로' 얻어오기. "
function Get-ScriptDirectory
{
    $Invocation = (Get-Variable MyInvocation -Scope 1).Value
    split-Path $Invocation.MyCommand.Path
}

$CurrentPath = Get-ScriptDirectory
# ------------------------------------------------
# "현재 스크립트가 존재하는 위치의 '드라이브명' 얻어오기. "
function Get-ScriptDrive
{
    $Invocation = (Get-Variable MyInvocation -Scope 1).Value
    split-Path -Qualifier $Invocation.MyCommand.Path
}

# ------------------------------------------------
# 현재 드라이브의 사용 가능한 용량 얻어오기. 
function Get-FreeSpace
{
    param(
        [string]$Path
    )

    $Drive = split-Path -Qualifier $Path
    
    $Drives = Get-WmiObject -Class Win32_LogicalDisk -Filter "DriveType=3" -ComputerName . | Select-Object -Property DeviceID, FreeSpace

    $Space = 0
    
     # 비교 연산자  의미                                     예제(true 반환)  
     # -eq           같음                                    1 -eq 1
     # -ne           같지 않음                               1 -ne 2
     # -lt           보다 작음                               1 -lt 2
     # -le           작거나 같음                             1 -le 2
     # -gt           보다 큼                                 2 -gt 1
     # -ge           크거나 같음                             2 -ge 1
     # -like         비슷함(텍스트의 와일드카드 비교)        "file.doc" -like "f*.do?"
     # -notlike      비슷하지 않음(텍스트의 와일드카드 비교) "file.doc" -notlike "p*.doc"
     # -contains     포함                                    1,2,3 -contains 1
     # -notcontains  포함 안 함                              1,2,3 -notcontains 4

    for ($i = 0; $i -lt $Drives.Count; $i += 1) {
        if($Drives[$i].DeviceID.Equals($Drive)) {
            $Space = $Drives[$i].FreeSpace
            break
        }  
    }
    
    $Space
}


# ------------------------------------------------
# "경로를 현재 스크립트가 존재하는 폴더로 지정. "
Set-Location $CurrentPath
Get-Location # 현재 위치 확인. 

# ------------------------------------------------
# 현재 폴더 이름 얻어오기
#$idx = $CurrentPath.LastIndexOf("\")
#$RootName = $CurrentPath.Substring($idx+1)
#$RootName 
# ------------------------------------------------
# 복사될 폴더 이름 만들기. 
$CurDate = Get-Date
$RootNameCopy = $CurrentPath + "_" + $CurDate.ToString("yyyy-MM-dd-HHmmss") #월 표기는 대문자 MM을 사용한다. 
# ------------------------------------------------
# 화면 Clear
cls

echo $CurrentPath
echo $RootNameCopy

# ------------------------------------------------
# 현재 드라이브의 여유 공간과 복사할 파일의 용량을 비교하여 충분한 여유 공간이 있는지 확인 후 복사를 진행한다. 
echo ""
write-host  "복사할 공간을 확인 중입니다." -fore "Yellow"

$Drive = Get-ScriptDrive
$FreeSpaceByte = Get-FreeSpace $Drive
$CopyNeedSpace = Get-FolderLength $CurrentPath $My_Exclude -Recurse

cls 
   
if($FreeSpaceByte -le  $CopyNeedSpace) {
    write-host "Source Dir : " $CurrentPath   -fore "Yellow"
    write-host "Backup Dir : " $RootNameCopy  -fore "Yellow"
    echo       "-------------------------------------------------" 
    write-host "[AZ Co. LTD. sysong@az1.co.kr]" $MyVersion -fore "Green"
    write-host "-------------------------------------------------"
    write-host " 경고 : 복사할 공간이 부족합니다!" -fore "Red" -back "Black"
    write-host "-------------------------------------------------"
    write-host "- Drive free space ["$Drive "]    : "($FreeSpaceByte/1024/1024).ToString("#,##0.00")" MByte" -fore "Red"
    write-host "- Space required for copying : " ($CopyNeedSpace/1024/1024).ToString("#,##0.00")" MByte" -fore "Cyan"
    write-host "-------------------------------------------------"
    Read-Host 'Press Enter to continue...' | Out-Null
}
else {
# ////////////////////////////////////////////////////////////////////////
# 실제 복사 진행.
    $CopyCount = 0

    $LapTime = measure-command {
        $CopyCount = Copy-ToCreateFolder $CurrentPath  $RootNameCopy  $My_Exclude -Recurse
    }
    
# ////////////////////////////////////////////////////////////////////////    
    cls 
    write-host "Source Dir : " $CurrentPath   -fore "Yellow"
    write-host "Backup Dir : " $RootNameCopy  -fore "Yellow"
    
    echo        "-------------------------------------------------" 
    write-host  "[AZ Co. LTD. sysong@az1.co.kr]" $MyVersion -foregroundcolor "Green"
    write-host  "-------------------------------------------------"    
    write-host  "- Copy Size    :" ($CopyNeedSpace/1024/1024).ToString("#,##0.00") " MByte" -Fore "Cyan"
    write-host  "- File Count   :" $CopyCount.ToString() -Fore "Cyan"
    write-host  "- Total Time   :" $LapTime.TotalSeconds.ToString("#0.000") "sec." -Fore "Cyan"
    write-host  "-------------------------------------------------"   
    write-host  " Back-up을 완료하였습니다." -foregroundcolor "Yellow"
    write-host  "-------------------------------------------------"
    Read-Host 'Press Enter to continue...' | Out-Null  
}

# ------------------------------------------------
# 압축. 
# ------------------------------------------------



# 압축할 파일 이름을 지정한다. 
# $zipfilename = $RootNameCopy + ".zip"

# 비어있는 Zip파일을 만든다.
# if(-not (test-path($zipfilename)))
# {
# 	set-content $zipfilename ("PK" + [char]5 + [char]6 + ("$([char]0)" * 18))
# 	(dir $zipfilename).IsReadOnly = $false  
# }

# 압축 준비
#$shellApplication = new-object -com shell.application
#$zipPackage = $shellApplication.NameSpace($zipfilename)

# 압축 시작
# $Files = Get-ChildItem $CurrentPath -exclude $exclude -Recurse
# foreach($file in $Files){
# 	$zipPackage.CopyHere($file.FullName)
# 	Start-sleep -milliseconds 500
# }

#$zipPackage.CopyHere($RootNameCopy)

echo "END"