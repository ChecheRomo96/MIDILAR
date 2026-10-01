. "$PSScriptRoot/romodular-adapter.ps1"
. (Join-Path $script:MIDILARRoModularScripts "common.ps1")

function Invoke-MIDILARCMake {
    param([Parameter(Mandatory = $true)][string[]]$Arguments)

    Invoke-RoModularCMake -Arguments $Arguments
}

function Get-MIDILARBuildDirectory {
    param([Parameter(Mandatory = $true)][string]$Preset)
    return Get-RoModularBuildDirectory -Preset $Preset
}

function Assert-MIDILARPreset {
    param([Parameter(Mandatory = $true)][string]$Preset)

    Assert-RoModularPreset -Preset $Preset
}

function Get-MIDILARConfiguration {
    param(
        [Parameter(Mandatory = $true)][string]$Preset,
        [string]$Configuration = "",
        [string]$DefaultConfiguration = "Debug"
    )

    return Get-RoModularConfiguration `
        -Preset $Preset `
        -Configuration $Configuration `
        -DefaultConfiguration $DefaultConfiguration
}

function Assert-MIDILARConfiguration {
    param([Parameter(Mandatory = $true)][string]$Configuration)

    Assert-RoModularConfiguration -Configuration $Configuration
}

function Assert-MIDILARConfigured {
    param([Parameter(Mandatory = $true)][string]$Preset)

    Assert-RoModularConfigured -Preset $Preset
}

function Resolve-MIDILARPath {
    param([Parameter(Mandatory = $true)][string]$Path)

    return Resolve-RoModularPath -Path $Path
}

function Assert-MIDILARDistChild {
    param([Parameter(Mandatory = $true)][string]$Path)

    Assert-RoModularDistChild -Path $Path
}
