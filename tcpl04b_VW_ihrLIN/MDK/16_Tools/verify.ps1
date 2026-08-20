param(
    [switch]$Build,
    [string]$Keil = 'C:\Keil_v5\UV4\UV4.exe'
)

$ErrorActionPreference = 'Stop'
$mdk = Split-Path -Parent $PSScriptRoot
$packageRoot = Split-Path -Parent $mdk
$project = Join-Path $mdk 'tcpl04b_swsc_lean.uvprojx'
$failures = [System.Collections.Generic.List[string]]::new()

function Add-Failure([string]$Message) {
    $script:failures.Add($Message)
}

$expectedDirectories = @(
    '01_ASW', '02_RTE', '03_BSW_UDS', '04_BSW_COM', '05_BSW_NVM',
    '06_BSW_DEM', '07_BSW_SYSTEM', '08_BSW_ECUAL', '09_MCAL',
    '10_Device', '11_Vendor_SDK', '12_Config', '13_Common', '14_Docs',
    '15_Tests', '16_Tools'
)
foreach ($directory in $expectedDirectories) {
    $directoryPath = Join-Path $mdk $directory
    if (-not (Test-Path -LiteralPath $directoryPath -PathType Container)) {
        Add-Failure "Missing architecture directory: $directory"
    }
}

$ihrRoot = Join-Path $mdk '11_Vendor_SDK\IHR'
$ihrSources = @(Get-ChildItem -LiteralPath $ihrRoot -Recurse -Filter '*.c')
if ($ihrSources.Count -ne 0) {
    Add-Failure "IHR C source must be integrated into AUTOSAR modules: $($ihrSources.FullName -join ', ')"
}

$forbiddenDirectories = @(
    '01_App', '02_System', '03_Algorithm_FixedPoint', '04_EcuAbstraction',
    '05_LinService', '06_TCPL04B_Device', '07_TCPL04B_MCAL',
    '08_TCPL04B_PAL', '09_Config', '10_Common', '11_Docs', '12_Tests',
    '13_Tools'
)
foreach ($directory in $forbiddenDirectories) {
    if (Test-Path -LiteralPath (Join-Path $mdk $directory)) {
        Add-Failure "Obsolete architecture directory remains: $directory"
    }
}

$forbiddenSourceNames = @(
    'LinDiag.c', 'Lin_UpperInterface.c', 'PduR_Lin.c', 'SystemDcm.c',
    'ModuleFlash.c', 'LinDiag.h', 'Lin_UpperInterface.h', 'PduR_Lin.h'
)
foreach ($name in $forbiddenSourceNames) {
    if (Get-ChildItem -LiteralPath $mdk -Recurse -File -Filter $name) {
        Add-Failure "Forbidden legacy module remains: $name"
    }
}

$requiredModuleFiles = @(
    '03_BSW_UDS\Dcm.c', '03_BSW_UDS\Dcm_LinNodeConfig.c',
    '03_BSW_UDS\LinTp.c', '04_BSW_COM\Com.c',
    '04_BSW_COM\LinIf.c', '04_BSW_COM\LinIf_Init.c',
    '04_BSW_COM\LinIf_Compat.c', '04_BSW_COM\LinIf_Cfg.c',
    '05_BSW_NVM\NvM.c',
    '05_BSW_NVM\NvM_Cfg.c', '05_BSW_NVM\MemIf.c',
    '05_BSW_NVM\Fee.c', '06_BSW_DEM\Dem.c',
    '06_BSW_DEM\Dem_Cfg.c', '07_BSW_SYSTEM\EcuM.c',
    '07_BSW_SYSTEM\BswM.c', '09_MCAL\Lin.c',
    '09_MCAL\Lin_Hw.c', '09_MCAL\Fls.c'
)
foreach ($file in $requiredModuleFiles) {
    $modulePath = Join-Path $mdk $file
    if (-not (Test-Path -LiteralPath $modulePath -PathType Leaf)) {
        Add-Failure "Missing AUTOSAR module source: $file"
    }
}

foreach ($unneededConfig in @('PduR_Cfg.c', 'PduR_Cfg.h',
                              'LinTp_Cfg.c', 'LinTp_Cfg.h',
                              'Lin_Cfg.c', 'Lin_Cfg.h')) {
    if (Get-ChildItem -LiteralPath $mdk -Recurse -File |
        Where-Object { $_.Name -eq $unneededConfig }) {
        Add-Failure "Fixed single-slave topology exposes unnecessary config: $unneededConfig"
    }
}

$communicationConfigs = @(Get-ChildItem -LiteralPath (Join-Path $mdk '04_BSW_COM') -File |
    Where-Object { $_.Name -like '*_Cfg.c' } |
    ForEach-Object { $_.Name } | Sort-Object)
$expectedCommunicationConfigs = @('LinIf_Cfg.c')
if (@(Compare-Object $expectedCommunicationConfigs $communicationConfigs).Count -ne 0) {
    Add-Failure 'Restored IHR communication may only add LinIf_Cfg.c as integration configuration'
}

$dcmHeader = Get-Content -LiteralPath (Join-Path $mdk '03_BSW_UDS\Dcm.h') -Raw
if (($dcmHeader -match 'ApplicationDataType') -or
    ($dcmHeader -match 'ApplicationDataId')) {
    Add-Failure 'DCM configuration leaks ASW data type/id mappings'
}

$nodeConfig = Get-Content -LiteralPath (Join-Path $mdk '03_BSW_UDS\Dcm_LinNodeConfig.c') -Raw
foreach ($sid in @('case(0xB0u)', 'case(0xB1u)', 'case(0xB2u)',
                   'case(0xB3u)', 'case(0xB4u)', 'case(0xB5u)',
                   'case(0xB6u)', 'case(0xB7u)')) {
    if ($nodeConfig -notmatch [regex]::Escape($sid)) {
        Add-Failure "Restored IHR node configuration lacks service: $sid"
    }
}

$dcmCfg = Get-Content -LiteralPath (Join-Path $mdk '01_ASW\03_Swc_Diagnostic\Swc_Diagnostic.c') -Raw
foreach ($did in @('0x0002', '0x0003', '0x0004', '0x0005',
                   '0x0006', '0x0007', '0x0008', '0x0009',
                   '0x000A', '0x0100', '0x6269', '0x6469',
                   '0x6869', '0x6A69')) {
    if ($dcmCfg -notmatch [regex]::Escape($did)) {
        Add-Failure "SWC diagnostic DID table lacks DID: $did"
    }
}

$aswDirectBsw = Get-ChildItem -LiteralPath (Join-Path $mdk '01_ASW') -Recurse -File |
    Select-String -Pattern '#include\s+"(Dcm|Com|PduR|LinTp|LinIf|Lin|NvM|Dem|Fee|MemIf|Fls)(_|\.)'
if ($aswDirectBsw) {
    Add-Failure 'ASW directly includes a BSW module instead of RTE/IoHwAb'
}

$bswDirectAsw = Get-ChildItem -LiteralPath (Join-Path $mdk '03_BSW_UDS'),
    (Join-Path $mdk '04_BSW_COM'), (Join-Path $mdk '05_BSW_NVM'),
    (Join-Path $mdk '06_BSW_DEM'), (Join-Path $mdk '07_BSW_SYSTEM') -Recurse -File |
    Select-String -Pattern '#include\s+"Swc_'
if ($bswDirectAsw) {
    Add-Failure 'BSW directly includes ASW instead of using configured callbacks/RTE'
}

[xml]$xml = [IO.File]::ReadAllText($project, [Text.Encoding]::UTF8)
$requiredAutosarLinSources = @(
    '.\03_BSW_UDS\Dcm_LinNodeConfig.c',
    '.\03_BSW_UDS\LinTp.c',
    '.\04_BSW_COM\LinIf.c',
    '.\04_BSW_COM\LinIf_Init.c',
    '.\04_BSW_COM\LinIf_Compat.c',
    '.\04_BSW_COM\LinIf_Cfg.c'
)
$expectedGroups = @(
    '01_ASW', '02_RTE', '03_BSW_UDS', '04_BSW_COM', '05_BSW_NVM',
    '06_BSW_DEM', '07_BSW_SYSTEM', '08_BSW_ECUAL', '09_MCAL',
    '10_Device', '11_Vendor_SDK'
)
foreach ($target in $xml.Project.Targets.Target) {
    $actualGroups = @($target.Groups.Group | ForEach-Object { $_.GroupName })
    $expectedSorted = @($expectedGroups | Sort-Object)
    $actualSorted = @($actualGroups | Sort-Object)
    $groupDifference = @(Compare-Object -ReferenceObject $expectedSorted -DifferenceObject $actualSorted)
    if ($groupDifference.Count -ne 0) {
        Add-Failure "Keil group hierarchy differs: $($target.TargetName)"
    }
    foreach ($file in $target.Groups.Group.Files.File) {
        $resolved = [IO.Path]::GetFullPath((Join-Path $mdk $file.FilePath))
        if (-not (Test-Path -LiteralPath $resolved -PathType Leaf)) {
            Add-Failure "Keil source is missing: $($file.FilePath)"
        }
        if ($file.FileName -in $forbiddenSourceNames) {
            Add-Failure "Keil still lists forbidden module: $($file.FileName)"
        }
    }
    $targetSources = @($target.Groups.Group.Files.File |
        ForEach-Object { $_.FilePath })
    foreach ($autosarSource in $requiredAutosarLinSources) {
        if ($autosarSource -notin $targetSources) {
            Add-Failure "Keil target lacks AUTOSAR LIN source: $($target.TargetName), $autosarSource"
        }
    }
    if ($targetSources | Where-Object { $_ -match '\\IHR\\.*\.c$' }) {
        Add-Failure "Keil target still compiles an IHR C source: $($target.TargetName)"
    }
}

$optionFile = Join-Path $mdk 'tcpl04b_swsc_lean.uvoptx'
if (Test-Path -LiteralPath $optionFile -PathType Leaf) {
    $optionText = Get-Content -LiteralPath $optionFile -Raw
    foreach ($staleName in @('PduR_Cfg.c', 'LinTp_Cfg.c',
                             'Lin_Cfg.c', 'Swc_Communication_Cfg.c',
                             'genLinConfig.c', 'lin_config.c', 'lin_diag_raw.c',
                             'lin_diag_tp.c', 'lin_driver_api.c', 'lin_main.c',
                             'lin_master_task.c', 'lin_slave_task.c')) {
        if ($optionText -match [regex]::Escape($staleName)) {
            Add-Failure "Keil option file still references obsolete source: $staleName"
        }
    }
}

if ($Build) {
    if (-not (Test-Path -LiteralPath $Keil -PathType Leaf)) {
        throw "Keil executable not found: $Keil"
    }
    foreach ($target in @('swsc_normal', 'swsc_match_boot')) {
        $log = Join-Path $mdk "verify_$target.log"
        if (Test-Path -LiteralPath $log) {
            Remove-Item -LiteralPath $log -Force
        }
        $null = Start-Process -FilePath $Keil -ArgumentList @(
            '-b', "`"$project`"", '-t', $target, '-j0', '-o', "`"$log`""
        ) -PassThru -WindowStyle Hidden
        # UV4 forwards command-line builds to an already-open debugger and the
        # launcher process may then stay attached to that desktop instance.
        # The target log is the authoritative completion/result indication.
        $deadline = [DateTime]::UtcNow.AddSeconds(120)
        do {
            Start-Sleep -Milliseconds 250
            $logText = if (Test-Path -LiteralPath $log) {
                Get-Content -LiteralPath $log -Raw -ErrorAction SilentlyContinue
            } else {
                ''
            }
            $logComplete = ($logText -match 'Build Time Elapsed:') -and
                           ($logText -match
                            '\d+ Error\(s\), \d+ Warning\(s\)')
        } while ((-not $logComplete) -and
                 ([DateTime]::UtcNow -lt $deadline))
        $cleanLog = (Test-Path -LiteralPath $log) -and
                    ($logText -match '0 Error\(s\), 0 Warning\(s\)')
        if (-not $cleanLog) {
            Add-Failure "Keil build is not clean: $target"
        }
    }
    foreach ($mapFile in @(
        (Join-Path $packageRoot 'Build\Listings\normal\tcpl04b_swsc_lean.map'),
        (Join-Path $packageRoot 'Build\Listings\boot\tcpl04b_swsc_lean_boot.map'))) {
        if (-not (Test-Path -LiteralPath $mapFile -PathType Leaf)) {
            Add-Failure "Missing linked image map: $mapFile"
            continue
        }
        $mapText = Get-Content -LiteralPath $mapFile -Raw
        foreach ($symbol in @('lin_main_init', 'lin_handle_config',
                              'lin_slave_task_rx_pid', 'ld_receive_message',
                              'ld_send_message')) {
            if ($mapText -notmatch [regex]::Escape($symbol)) {
                Add-Failure "Linked image lacks IHR symbol: $symbol"
            }
        }
    }
}

if ($failures.Count -ne 0) {
    $failures | ForEach-Object { Write-Error $_ }
    exit 1
}

Write-Host 'PASS: restored IHR diagnostics/communication with AUTOSAR outer-layer ownership and Keil project.'
